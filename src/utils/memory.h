#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdint>
#include <string>

// memory
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
	void close_handle() noexcept;

	// rpm wrapper
	template <typename T>
	constexpr T read(const std::uintptr_t& addr) noexcept {
		T value;
		ReadProcessMemory(handle, reinterpret_cast<const void*>(addr), &value, sizeof(T), 0);
		return value;
	}

	// rpm wrapper
	template <typename T>
	constexpr void write(const std::uintptr_t& addr, const T& value) noexcept {
		WriteProcessMemory(handle, reinterpret_cast<void*>(addr), &value, sizeof(T), 0);
	}
}
