#include <iostream>
#include <format>
#include <thread>

#include "memory.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void print(const std::string& text) noexcept {
	std::cout << text;
}

void sleep(const int time) noexcept {
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

int main() {
	print("Welcome to externalia - by cazz\n\n");

	// get proc id
	m::process_id("csgo.exe");

	// check if get proc id failed
	if (m::id == 0) {
		print("[ERROR] Failed to get proc id, please open csgo and try again...");
		std::cin.get();
		return 1;
	}

	print(std::format("[INFO] Process ID -> {}\n", m::id));

	// wait for csgo to load modules
	while (!m::module_address("serverbrowser.dll")) {
		sleep(200);
	}

	// module addrs
	m::client = m::module_address("client.dll");
	m::engine = m::module_address("engine.dll");

	if (m::client == 0 || m::engine == 0) {
		print("[ERROR] Failed to get module addresses, please open csgo and try again...");
		std::cin.get();
		return 1;
	}

	print(std::format("[INFO] \"client.dll\" = {:#x}\n", m::client));
	print(std::format("[INFO] \"engine.dll\" = {:#x}\n", m::engine));

	// create our handle
	if (!m::open_handle()) {
		print("[ERROR] Failed to open handle, please open csgo and try again...");
		std::cin.get();
		return 1;
	}

	// gui loop
	while (true) {

	}

	// free handle resources
	if (!m::close_handle()) {
		print("[ERROR] Failed to close handle...");
		std::cin.get();
		return 1;
	}

	return 0;
}
