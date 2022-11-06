#pragma once

namespace big
{
	typedef uint32_t(__fastcall* uint_reg_ytd)(uint32_t*, const char*, bool, const char*, bool);

	class ytd_loader
	{

	public:

		ytd_loader();
		~ytd_loader();

		uint32_t m_reg_int;
		uint32_t load_ytd(const std::string& path, const std::string& file);

	};
	inline ytd_loader ytd_file;
}
