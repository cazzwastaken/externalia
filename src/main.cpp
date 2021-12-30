#include <iostream>
#include <format>
#include <thread>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "globals.h"
#include "ui.h"
#include "hacks/hacks.h"

const int dialog(const std::string& text) noexcept {
	MessageBeep(MB_ICONERROR);
	return MessageBox(
		u::window ? u::window : NULL,
		text.c_str(),
		"Externalia",
		MB_ICONERROR | MB_OK
	);
}

int __stdcall WinMain(
	const HINSTANCE instance,
	const HINSTANCE prev_instance,
	const LPSTR args,
	const int cmd_show
) {
	m::process_id("csgo.exe");

	if (m::id == 0) {
		dialog("Please open cs:go!");
		return 1;
	}

	// wait for csgo to load modules
	while (!m::module_address("serverbrowser.dll")) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	m::client = m::module_address("client.dll");
	m::engine = m::module_address("engine.dll");

	if (m::client == 0 || m::engine == 0) {
		return dialog("Failed to get module addresses.");
	}

	// create ui
	if (!m::open_handle()) {
		return dialog("Failed to open a handle to the game.");
	}

	if (!u::create_window("externalia")) {
		return dialog("Failed to create window.");
	}

	if (!u::create_device()) {
		u::destroy_window();
		return dialog("Failed to create device.");
	}

	u::create_menu();

	std::thread{ g::entities }.detach();
	std::thread{ h::visuals }.detach();

	while (g::run) {
		u::render();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	// cleanup
	u::destroy_menu();
	u::destroy_device();
	u::destroy_window();

	m::close_handle();

	return 0;
}
