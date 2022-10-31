#include "lua_script.hpp"
#include "lua_memory.hpp"
#include "memory/module.hpp"
#include "memory/pattern.hpp"

namespace big::lua::memory
{
	class lua_address
	{
		std::uint64_t m_address;

	public:
		lua_address(std::uint64_t address) :
			m_address(address)
		{
		}

		lua_address add(std::ptrdiff_t value)
		{
			return lua_address(m_address + value);
		}

		lua_address sub(std::ptrdiff_t value)
		{
			return lua_address(m_address - value);
		}

		lua_address rip()
		{
			return add(*(std::int32_t*)m_address).add(4);
		}

		int get_byte()
		{
			return (int)(*(char*)m_address);
		}

		int get_word()
		{
			return (int)(*(std::uint16_t*)m_address);
		}

		int get_dword()
		{
			return (*(int*)m_address);
		}

		uint64_t get_qword()
		{
			return (uint64_t)(*(std::uint64_t*)m_address);
		}

		lua_address get_address()
		{
			return lua_address(get_qword());
		}

		bool is_valid()
		{
			return m_address != 0;
		}

		void write_byte(int byte)
		{
			*((char*)m_address) = (char)byte;
		}

		void write_word(int word)
		{
			*((std::uint16_t*)m_address) = (uint16_t)word;
		}

		void write_dword(int dword)
		{
			*((std::uint32_t*)m_address) = (uint32_t)dword;
		}

		void write_qword(std::uint64_t qword)
		{
			*((std::uint64_t*)m_address) = qword;
		}
	};

	lua_address scan_pattern(const std::string& pattern)
	{
		auto module = ::memory::module(nullptr);
		auto result = module.scan(::memory::pattern(pattern));
		if (result == nullptr)
			return lua_address(0);
		return lua_address(result.as<std::uintptr_t>());
	}

	void load(sol::state& state)
	{
		auto ns = state["memory"].get_or_create<sol::table>();
		auto address_type = ns.new_usertype<lua_address>("address", sol::constructor_list<lua_address(std::uint64_t)>());
		address_type["add"] = &lua_address::add;
		address_type["sub"] = &lua_address::sub;
		address_type["rip"] = &lua_address::rip;
		address_type["get_byte"] = &lua_address::get_byte;
		address_type["get_word"] = &lua_address::get_word;
		address_type["get_dword"] = &lua_address::get_dword;
		address_type["get_qword"] = &lua_address::get_qword;
		address_type["get_address"] = &lua_address::get_address;
		address_type["is_valid"] = &lua_address::is_valid;
		address_type["write_byte"] = &lua_address::write_byte;
		address_type["write_word"] = &lua_address::write_word;
		address_type["write_dword"] = &lua_address::write_dword;
		address_type["write_qword"] = &lua_address::write_qword;

		ns["scan_pattern"] = scan_pattern;
	}
}