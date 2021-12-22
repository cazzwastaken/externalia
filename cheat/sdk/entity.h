#pragma once
#include "../memory.h"

class c_entity {
	// offsets
	constexpr std::uintptr_t team() const noexcept { return 0xF4; }
	constexpr std::uintptr_t spotted() const noexcept { return 0x93D; }

public:
	// constructor that gets entity ptr from addr
	c_entity(const std::uintptr_t& addr) noexcept :
		_addr(m::read<std::uintptr_t>(addr)) { }

	int get_team() const noexcept {
		return m::read<int>(_addr + team());
	}

	void set_spotted(const bool is_spotted) const noexcept {
		m::write(_addr + spotted(), is_spotted);
	}

private:
	const std::uintptr_t _addr;
};
