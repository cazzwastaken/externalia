#pragma once

#include <cstdint>
#include <string>

namespace m {
	inline int id = 0;
	inline void* handle = nullptr;

	// module addresses
	inline std::uintptr_t client = 0;
	inline std::uintptr_t engine = 0;

	// get process id from process name
	void process_id(const std::string name) noexcept;

	// get module base addr from module name
	std::uintptr_t module_address(const std::string name) noexcept;

	// proc handle stuff
	bool open_handle() noexcept;
	bool close_handle() noexcept;

	// rpm wrapper
	template <typename T>
	T read(const std::uintptr_t& addr) noexcept;

	// rpm wrapper
	template <typename T>
	void write(const std::uintptr_t& addr, const T& value) noexcept;
}
