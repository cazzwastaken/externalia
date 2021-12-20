#include "memory.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

void m::process_id(const std::string name) noexcept {
	auto entry = PROCESSENTRY32{ };
	entry.dwSize = sizeof(PROCESSENTRY32);

	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// loop through processes
	do {
		if (!name.compare(entry.szExeFile)) {
			m::id = entry.th32ProcessID;
			break;
		}
	} while (Process32Next(snap, &entry));

	if (snap)
		CloseHandle(snap);
}

std::uintptr_t m::module_address(const std::string name) noexcept {
	auto entry = MODULEENTRY32{ };
	entry.dwSize = sizeof(MODULEENTRY32);

	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);

	std::uintptr_t addr = 0;

	// loop through modules
	do {
		if (!name.compare(entry.szModule)) {
			addr = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
			break;
		}
	} while (Module32Next(snap, &entry));

	if (snap)
		CloseHandle(snap);

	return addr;
}

bool m::open_handle() noexcept {
	return m::handle = OpenProcess(PROCESS_ALL_ACCESS, 0, m::id);
}

bool m::close_handle() noexcept {
	if (m::handle)
		return CloseHandle(m::handle);

	return false;
}

template <typename T>
T m::read(const std::uintptr_t& addr) noexcept {
	T value;
	ReadProcessMemory(handle, reinterpret_cast<const void*>(addr), &value, sizeof(T), 0);
	return value;
}

template <typename T>
void m::write(const std::uintptr_t& addr, const T& value) noexcept {
	WriteProcessMemory(handle, reinterpret_cast<void*>(addr), &value, sizeof(T), 0);
}
