#ifdef ENABLE_ASI_LOADER
	#include "pools.h"
	#include "util/pools.hpp"

namespace rage
{
	struct EntityPoolTask
	{
		inline bool TypeHasFlag(uint16_t flag)
		{
			assert(!(_type & ~31));
			assert(!(flag & ~31));
			return (_type & flag) != 0;
		}

		EntityPoolTask(uint16_t type) :
		    _type(type)
		{
		}

		void Run(std::vector<uintptr_t>& _pointers)
		{
			if (TypeHasFlag(PoolTypePickup))
			{
				for (int i = 0; i < big::g_pointers->m_gta.m_pickup_pool->size; i++)
				{
					if (uintptr_t address = big::g_pointers->m_gta.m_pickup_pool->getAddress(i))
					{
						_pointers.push_back(address);
					}
				}
			}

			if (TypeHasFlag(PoolTypeCamera))
			{
				for (int i = 0; i < big::g_pointers->m_gta.m_camera_pool->size; i++)
				{
					if (uintptr_t address = big::g_pointers->m_gta.m_camera_pool->getAddress(i))
					{
						_pointers.push_back(address);
					}
				}
			}
		}

	private:
		uint16_t _type;
	};

	void GetEntityPointers(EntityPoolType type, std::vector<uintptr_t>& result)
	{
		EntityPoolTask(type).Run(result);
	}

	/**
	 * @brief Get the All World objects
	 * 
	 * @param type Entity type to return.
	 * @param max Max entitys to include is a vector.
	 * @return std::vector<Entity> Found entitys.
	 * @todo This is awful, but it should work for now.
	 * Make it all use the same method.
	 */
	std::vector<Entity> GetAllWorld(EntityPoolType type, int max)
	{
		std::vector<Entity> entities;
		switch (type)
		{
		case EntityPoolType::PoolTypeVehicle:
		{
			entities = big::pools::get_all_vehicles_array();
			break;
		}
		case EntityPoolType::PoolTypePed:
		{
			entities = big::pools::get_all_peds_array();
			break;
		}
		case EntityPoolType::PoolTypeObject:
		{
			entities = big::pools::get_all_props_array();
			break;
		}
		default:
		{
			int count = 0;
			std::vector<uintptr_t> pointers;
			GetEntityPointers(type, pointers);

			for (const auto& cEntity : pointers)
			{
				if (count == max)
					break;
				auto entity = big::g_pointers->m_gta.m_handle_to_ptr(cEntity);
				if (entity)
				{
					entities.push_back((const Entity&)entity);
					count++;
				}
			}
			break;
		}
		}

		if (entities.size() > max)
			entities.resize(max);

		return entities;
	}

	int GetAllWorld(EntityPoolType type, int max, int* arr)
	{
		auto entities = GetAllWorld(type, max);

		for (int i = 0; i < entities.size(); ++i)
		{
			arr[i] = entities[i];
		}

		return (int)entities.size();
	}
}
#endif // ENABLE_ASI_LOADER
