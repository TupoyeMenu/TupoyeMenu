#ifdef ENABLE_ASI_LOADER
#include "pe_image.hpp"

namespace big::pe_utils
{
	pe_image::pe_image()
	{
	}

	pe_image::~pe_image()
	{
	}

	bool pe_image::load(const std::string& path)
	{
		filePath = path;

		std::ifstream inputFile(path, std::ios::binary);
		if (inputFile.fail()) {
			return false;
		}
		std::vector<char> bufferTemp((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
		fileBuffer.swap(bufferTemp);

		bool parseSuccess = image_pe();
		if (!parseSuccess) {
			return false;
		}

		return true;
	}

	bool pe_image::image_pe()
	{
		// Get DOS header
		const IMAGE_DOS_HEADER* dosHeader = reinterpret_cast<const IMAGE_DOS_HEADER*>(fileBuffer.data());

		// Not a valid PE
		if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
			return false;
		}

		// Get NT header
		ntHeader = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<const uint8_t*>(dosHeader) + dosHeader->e_lfanew);

		return true;
	}

	uint64_t pe_image::get_directory_address(int index)
	{

		const IMAGE_DATA_DIRECTORY* dataDirectory = ntHeader->OptionalHeader.DataDirectory;

		return rva_to_va(dataDirectory[index].VirtualAddress);
	}

	uint64_t pe_image::rva_to_va(uint32_t rva) const
	{
		const IMAGE_SECTION_HEADER* sectionHeader = reinterpret_cast<const IMAGE_SECTION_HEADER*>(ntHeader + 1);
		for (int i = 0; i < ntHeader->FileHeader.NumberOfSections; ++i, ++sectionHeader) {

			if (rva >= sectionHeader->VirtualAddress && rva <= sectionHeader->VirtualAddress + sectionHeader->Misc.VirtualSize) {

				return reinterpret_cast<uint64_t>(fileBuffer.data()) + (rva - sectionHeader->VirtualAddress + sectionHeader->PointerToRawData);
			}
		}

		return 0;
	}

	bool pe_image::is_openvhook_compatible()
	{
		auto* importTable = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(get_directory_address(IMAGE_DIRECTORY_ENTRY_IMPORT));
		for (; importTable->Name; ++importTable) {

			char* dllName = reinterpret_cast<char*>(rva_to_va(importTable->Name));

			if (strcmp(dllName, "ScriptHookV.dll") == 0) {
				return false;
			}
		}

		return true;
	}

	bool pe_image::patch_compatibility()
	{
		// Find ScriptHooKV import descriptor
		auto* importTable = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(get_directory_address(IMAGE_DIRECTORY_ENTRY_IMPORT));
		for (; importTable->Name; ++importTable) {

			char* dllName = reinterpret_cast<char*>(rva_to_va(importTable->Name));

			if (strcmp(dllName, "ScriptHookV.dll") == 0) {

				// Found it, patch that shit
				ZeroMemory(dllName, strlen(dllName));
				strcpy(dllName, "TupoyeMenu.dll");

				// Overwrite original file with changes
				std::ofstream file(filePath, std::ios::binary | std::ios::out);
				file.write(reinterpret_cast<char*>(fileBuffer.data()), fileBuffer.size());
				file.close();

				return true;
			}
		}

		return false;
	}

}
#endif // ENABLE_ASI_LOADER
