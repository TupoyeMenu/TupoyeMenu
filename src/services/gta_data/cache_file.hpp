#pragma once
#include "file_manager/file.hpp"

namespace big
{
	class cache_header final
	{
	public:
		uint32_t m_cache_version;
		uint32_t m_file_version;
		uint64_t m_data_size;
	};

	using cache_data = std::unique_ptr<uint8_t[]>;
	class cache_file final
	{
	public:
		cache_file() = default;
		/**
		 * @param cache_file FileMgr file object
		 */
		cache_file(file cache_file);

		/**
		 * @param cache_file FileMgr file object
		 * @param cache_version Internal version, use this to invalidate the cache when changing the structure of the data
		 */
		cache_file(file cache_file, uint32_t cache_version);

		/**
		 * @brief Frees any memory used to hold the cached data.
		 */
		void free();

		/**
		 * @brief Attempts to load the cache from disk
		 * 
		 * @return True after successfully loading the data, false if the file didn't exist.
		 */
		bool load();

		/**
		 * @brief Writes the cache to disk
		 */
		bool write() const;

		uint8_t* data() const;
		uint64_t data_size() const;

		/**
		 * @brief Check if the cache file is up to date with the expected versions
		 * 
		 * @return bool True if cache is up to date, false otherwise.
		 */
		bool up_to_date(uint32_t file_version) const;


		void set_data(cache_data&& data, uint64_t data_size);

		/**
		 * @brief Sets the version information of the cache header.
		 */
		void set_header_version(uint32_t file_version);

		void set_cache_version(uint32_t cache_version);

	private:
		file m_cache_file;

		uint32_t m_cache_version;

		cache_header m_cache_header;
		cache_data m_data;
	};
}
