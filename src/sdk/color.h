#pragma once
#include <array>

template <typename T>
struct color3_t {
	T r = { }, g = { }, b = { };
};

template <typename T>
struct color4_t {
	constexpr color4_t(const std::array<T, 4U>& arr) noexcept :
		_r(arr[0]), _g(arr[1]), _b(arr[2]), _a(arr[3]) { }

private:
	T _r = { }, _g = { }, _b = { }, _a = { };
};
