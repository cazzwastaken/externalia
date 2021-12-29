#include "hacks.h"

#include <thread>

void h::visuals() noexcept {
	while (g::run) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (g::entity_list.empty()) {
			continue;
		}

		for (const auto& entity : g::entity_list) {
			if (!entity.is_alive()) {
				continue;
			}

			if (entity.is_dormant()) {
				continue;
			}

			if (entity.get_team() == g::local_player.get_team()) {
				// glow
				m::write(
					g::glow_object_manager() + (0x38 * entity.get_glow_index()) + 0x8,
					color4_t<float>{ 0.f, 0.f, 1.f, 1.f }
				);

				constexpr struct visible_t { bool a{ true }, b{ false }; }vis;
				m::write(g::glow_object_manager() + (0x38 * entity.get_glow_index()) + 0x28, vis);
			} else {
				// glow
				m::write(
					g::glow_object_manager() + (0x38 * entity.get_glow_index()) + 0x8,
					color4_t<float>{ 1.f, 0.f, 0.f, 1.f }
				);

				constexpr struct visible_t { bool a{ true }, b{ false }; }vis;
				m::write(g::glow_object_manager() + (0x38 * entity.get_glow_index()) + 0x28, vis);

				// radar
				entity.set_spotted(true);
			}
		}
	}
}
