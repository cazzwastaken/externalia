#pragma once
#include <vector>
#include <thread>

#include "sdk/entity.h"

// offsets
namespace o {
	constexpr std::ptrdiff_t dw_client_state = 0x589FC4;
	constexpr std::ptrdiff_t dw_entity_list = 0x4DD0AB4;
	constexpr std::ptrdiff_t dw_force_jump = 0x527A9AC;
	constexpr std::ptrdiff_t dw_glow_object_manager = 0x5318E50;
	constexpr std::ptrdiff_t dw_local_player = 0xDB558C;
}

// globals
namespace g {
	inline bool run = true;

	inline std::vector<c_entity> entity_list;
	inline c_entity local_player;

	void entities() noexcept;

	constexpr const std::uintptr_t client_state() noexcept {
		return m::read<std::uintptr_t>(m::engine + o::dw_client_state);
	}

	constexpr const std::int32_t max_players() noexcept {
		return m::read< std::int32_t>(client_state() + 0x388);
	}

	constexpr const std::uintptr_t glow_object_manager() noexcept {
		return m::read<std::uintptr_t>(m::client + o::dw_glow_object_manager);
	}
}
