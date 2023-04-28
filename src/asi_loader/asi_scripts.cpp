/**
 * @file asi_scripts.cpp
 * @brief Find and load any asi plugins in the OpenHookV directory.
 * @note Skidded from https://github.com/YimMenu/YimMenu/pull/135.
 */

#ifdef ENABLE_ASI_LOADER
#include "asi_scripts.hpp"

#include "pe_image.hpp"

namespace big::asi_loader
{
	void initialize()
	{
		LOG(INFO) << "Loading *.asi plugins.";

		const auto asi_folder = g_file_manager->get_project_folder("./OpenHookV");

		for (const auto& item : std::filesystem::directory_iterator(asi_folder.get_path()))
		{
			if (item.is_directory())
				continue;

			const auto path = item.path();
			if (path.extension() != ".asi")
				continue;

			pe_utils::pe_image plugin_image;
			if (!plugin_image.load(path.string()))
			{
				LOG(WARNING) << "Failed to load image: " << path.filename();

				continue;
			}

			if (!plugin_image.is_openvhook_compatible())
			{
				LOG(INFO) << "ASI is not compatible, patching imports...";

				if (!plugin_image.patch_compatibility())
				{
					LOG(WARNING) << "Failed to patch image: " << path.filename();

					continue;
				}
			}
			const auto hmod = LoadLibraryA(path.string().c_str());
			if (!hmod)
			{
				LOG(WARNING) << "Failed to load image: " << path.filename();

				continue;
			}
			LOG(INFO) << "Loaded image: " << path.filename() << " -> " << HEX_TO_UPPER(hmod);
		}

		LOG(INFO) << "Finished loading *.asi plugins.";
	}
}
#endif // ENABLE_ASI_LOADER
