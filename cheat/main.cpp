#include <iostream>
#include <format>
#include <thread>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "globals.h"

void print(const std::string& text) noexcept {
	std::cout << text;
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
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
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

	g::entity_list.reserve(32);

	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		g::local_player.set_address(m::client + o::dw_local_player);

		// not in game
		if (g::max_players() == 0) {
			g::entity_list.clear();
			continue;
		}

		// populate entity list
		for (std::uintptr_t i = 1; i <= 32; ++i) {
			const auto entity = c_entity{ m::client + o::dw_entity_list + i * 0x10 };

			if (!entity.is_valid()) {
				continue;
			}

			if (entity == g::local_player) {
				continue;
			}

			const auto result = std::find_if(
				g::entity_list.begin(),
				g::entity_list.end(),
				[&entity](const c_entity& ent) noexcept -> bool { return ent == entity; }
			);

			// it exists in the list already
			if (result != std::end(g::entity_list)) {
				continue;
			}

			g::entity_list.emplace_back(entity);
		}

		// this temporay, for testing the entity list vector
		for (const auto& entity : g::entity_list) {
			if (entity.get_team() == g::local_player.get_team())
				continue;

			entity.set_spotted(true);
		}
	}

	// free handle resources
	if (!m::close_handle()) {
		print("[ERROR] Failed to close handle...");
		std::cin.get();
		return 1;
	}

	return 0;
}
