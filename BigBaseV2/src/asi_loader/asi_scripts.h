#ifndef __ASI_LOADER_H__
#define __ASI_LOADER_H__

namespace ASILoader 
{
	void				Initialize();
	extern				std::vector<std::string>	LoadedPlugins;
	static big::folder check_asi_folder();
	
};

#endif // __ASI_LOADER_H__