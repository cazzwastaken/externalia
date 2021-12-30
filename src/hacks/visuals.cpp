#include "hacks.h"

#include <thread>

void h::visuals() noexcept {
	while (g::run) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2));

		for (const auto& entity : g::entity_list) {
			if (!entity.is_alive()) {
				continue;
			}

			if (entity.is_dormant()) {
				continue;
			}

			if (entity.get_team() == g::local_player.get_team()) {
				if (v::team_glow.first) {
					m::write(
						g::glow_object_manager() + (0x38 * entity.get_glow_index()) + 0x8,
						color4_t{ v::team_glow.second }
					);

					constexpr struct visible_t { bool a{ true }, b{ false }; }vis;
					m::write(g::glow_object_manager() + (0x38 * entity.get_glow_index()) + 0x28, vis);
				}
			} else {
				if (v::enemy_glow.first) {
					m::write(
						g::glow_object_manager() + (0x38 * entity.get_glow_index()) + 0x8,
						color4_t{ v::enemy_glow.second }
					);

					constexpr struct visible_t { bool a{ true }, b{ false }; }vis;
					m::write(g::glow_object_manager() + (0x38 * entity.get_glow_index()) + 0x28, vis);
				}

				if (v::radar) {
					entity.set_spotted(true);
				}
			}
		}
	}
}
