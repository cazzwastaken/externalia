#pragma once
#include "../utils/memory.h"

#include "color.h"

class c_entity {
	constexpr const std::uintptr_t team() const noexcept { return 0xF4; }
	constexpr const std::uintptr_t spotted() const noexcept { return 0x93D; }
	constexpr const std::uintptr_t life_state() const noexcept { return 0x25F; }
	constexpr const std::uintptr_t flags() const noexcept { return 0x104; }
	constexpr const std::uintptr_t render() const noexcept { return 0x70; }
	constexpr const std::uintptr_t glow_index() const noexcept { return 0x10488; }
	constexpr const std::uintptr_t dormant() const noexcept { return 0xED; }

public:
	c_entity() = default;

	c_entity(const std::uintptr_t& addr_to_read) noexcept :
		_addr(m::read<std::uintptr_t>(addr_to_read)) { }

	void set_address(const std::uintptr_t& addr_to_read) noexcept {
		_addr = m::read<std::uintptr_t>(addr_to_read);
	}

	constexpr const bool is_valid() const noexcept {
		return _addr != 0;
	}

public:
	constexpr bool operator==(const c_entity& other_ent) const noexcept {
		return _addr == other_ent._addr;
	}

public:
	constexpr const int get_team() const noexcept {
		return m::read<int>(_addr + team());
	}

	constexpr const int get_glow_index() const noexcept {
		return m::read<int>(_addr + glow_index());
	}

	constexpr const bool is_dormant() const noexcept {
		return m::read<bool>(_addr + dormant());
	}

	constexpr const bool is_alive() const noexcept {
		return m::read<int>(_addr + life_state()) == 0;
	}

	constexpr void set_spotted(const bool& is_spotted) const noexcept {
		m::write(_addr + spotted(), is_spotted);
	}

	constexpr void set_render(const color3_t<std::uint8_t>& clr) const noexcept {
		m::write(_addr + render(), clr);
	}

protected:
	std::uintptr_t _addr;
};
