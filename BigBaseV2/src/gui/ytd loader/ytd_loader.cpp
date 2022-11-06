#include "ytd_loader.hpp"
#include "pointers.hpp"

namespace big
{
	ytd_loader::ytd_loader() : m_reg_int(-1)
	{
	}
	ytd_loader::~ytd_loader() = default;

	std::uint32_t ytd_loader::load_ytd(
		const std::string& path,
		const std::string& file)
	{
		if (std::ifstream(path))
		{
			g_pointers->m_reg_ytd(&m_reg_int, path.c_str(), true, file.c_str(), false);
			LOG(INFO) << "TEXTURE INFO: " << path.c_str() << "FILE: " << file.c_str() << m_reg_int;
			return 0;
		}
		return 0;
	}

}