#pragma once

namespace big::pe_utils
{
	typedef const IMAGE_NT_HEADERS64 NT64H;
	typedef std::vector<char> bufferVec;

	class pe_image
	{
	public:
		pe_image();
		~pe_image();

		bool load(const std::string& path);

		bool is_openvhook_compatible();

		bool patch_compatibility();

	private:
		uint64_t get_directory_address(int index);
		uint64_t rva_to_va(uint32_t rva) const;

		bool image_pe();

	private:
		std::string filePath;
		bufferVec fileBuffer;
		NT64H* ntHeader = nullptr;
	};

}
