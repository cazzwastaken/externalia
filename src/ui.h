#pragma once
#include <d3d9.h>

// user interface
namespace u {
	constexpr struct window_size_t {
		const int width = 500;
		const int height = 300;
	}size;

	inline HWND window = nullptr;
	inline WNDCLASSEX window_class = { };

	inline POINTS window_pos = { };

	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS params = { };

	const bool create_window(const char* name) noexcept;
	void destroy_window() noexcept;

	const bool create_device() noexcept;
	void reset_device() noexcept;
	void destroy_device() noexcept;

	void create_menu() noexcept;
	void destroy_menu() noexcept;

	void render() noexcept;
}
