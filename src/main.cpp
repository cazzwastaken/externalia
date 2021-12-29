#include <iostream>
#include <format>
#include <thread>
#include <algorithm>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "globals.h"
#include "ui.h"
#include "hacks/hacks.h"

void dialog(const std::string& text) noexcept {
	MessageBeep(MB_ICONERROR);
	MessageBox(
		NULL,
		text.c_str(),
		"Externalia",
		MB_OK
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
		dialog("Failed to get module addresses.");
		return 1;
	}

	// create ui
	if (!m::open_handle()) {
		dialog("Failed to open a handle to the game.");
		return 1;
	}

	if (!u::create_window("externalia")) {
		dialog("Failed to create window.");
		return 1;
	}

	if (!u::create_device()) {
		dialog("Failed to create device.");
		u::destroy_window();
		return 1;
	}

	u::create_menu();

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
