#include "globals.h"

void g::entities() noexcept {
	entity_list.reserve(32);

	while (run) {
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		// set our local player
		local_player.set_address(m::client + o::dw_local_player);

		// clear ent list when not in-game
		if (max_players() == 0) {
			entity_list.clear();
			continue;
		}

		// populate entity list
		for (std::uintptr_t i = 1; i <= 32; ++i) {
			const auto entity = c_entity{ m::client + o::dw_entity_list + i * 0x10 };

			if (!entity.is_valid()) {
				continue;
			}

			// skip local player
			if (entity == local_player) {
				continue;
			}

			const auto result = std::find_if(
				entity_list.begin(),
				entity_list.end(),
				[&entity](const c_entity& ent) noexcept -> bool { return ent == entity; }
			);

			// if it is already in the list
			if (result != std::end(g::entity_list)) {
				continue;
			}

			entity_list.emplace_back(entity);
		}
	}
}
