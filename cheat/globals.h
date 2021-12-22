#pragma once
#include <vector>

namespace o {
	constexpr std::ptrdiff_t dw_client_state = 0x589FC4;
	constexpr std::ptrdiff_t dw_entity_list = 0x4DCFA94;
	constexpr std::ptrdiff_t dw_force_jump = 0x527998C;
	constexpr std::ptrdiff_t dw_glow_object_manager = 0x5317E28;
	constexpr std::ptrdiff_t dw_local_player = 0xDB458C;
}

namespace g {
	inline std::vector<class c_entity> entity_list;
}
