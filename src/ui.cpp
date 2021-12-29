#include "ui.h"

#include "globals.h"

#include "../ext/imgui/imgui.h"
#include "../ext/imgui/imgui_impl_dx9.h"
#include "../ext/imgui/imgui_impl_win32.h"

long __stdcall window_process(
	const HWND window,
	const UINT message,
	const WPARAM w_param,
	const LPARAM l_param
);

const bool u::create_window(const char* name) noexcept {
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_CLASSDC;
	window_class.lpfnWndProc = window_process;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = GetModuleHandle(0);
	window_class.hIcon = 0;
	window_class.hCursor = 0;
	window_class.hbrBackground = 0;
	window_class.lpszMenuName = 0;
	window_class.lpszClassName = "externalia001";
	window_class.hIconSm = 0;

	if (!RegisterClassEx(&window_class)) {
		return false;
	}

	window = CreateWindowEx(
		0,
		window_class.lpszClassName,
		name,
		WS_POPUP,
		100,
		100,
		size.width,
		size.height,
		0,
		0,
		window_class.hInstance,
		0
	);

	if (!window) {
		return false;
	}

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

	return true;
}

void u::destroy_window() noexcept {
	DestroyWindow(window);
	UnregisterClass(window_class.lpszClassName, window_class.hInstance);
}

const bool u::create_device() noexcept {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d) {
		return false;
	}

	ZeroMemory(&params, sizeof(params));

	params.Windowed = TRUE;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.EnableAutoDepthStencil = TRUE;
	params.AutoDepthStencilFormat = D3DFMT_D16;
	params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&params,
		&device) < 0) {
		return false;
	}

	return true;
}

void u::reset_device() noexcept {
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&params);

	if (result == D3DERR_INVALIDCALL) {
		IM_ASSERT(0);
	}

	ImGui_ImplDX9_CreateDeviceObjects();
}

void u::destroy_device() noexcept {
	if (device) {
		device->Release();
		device = nullptr;
	}

	if (d3d) {
		d3d->Release();
		d3d = nullptr;
	}
}

void u::create_menu() noexcept {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void u::destroy_menu() noexcept {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void u::render() noexcept {
	MSG message;
	while (PeekMessage(
		&message,
		0,
		0,
		0,
		PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT) {
			g::run = !g::run;
			return;
		}
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ size.width, size.height });
	ImGui::Begin(
		"subscribe to cazzy",
		&g::run,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	ImGui::Button("subscribe");
	ImGui::End();

	ImGui::EndFrame();
	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		reset_device();
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM w_param,
	LPARAM l_param
);

long __stdcall window_process(
	const HWND window,
	const UINT message,
	const WPARAM w_param,
	const LPARAM l_param
) {
	if (ImGui_ImplWin32_WndProcHandler(
		window,
		message,
		w_param,
		l_param))
		return 1L;

	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0L;

	case WM_LBUTTONDOWN:
		u::window_pos = MAKEPOINTS(l_param);
		return 0L;

	case WM_MOUSEMOVE:
		if (w_param == MK_LBUTTON) {
			const auto points = MAKEPOINTS(l_param);

			auto rect = RECT{ };
			GetWindowRect(u::window, &rect);

			rect.left += points.x - u::window_pos.x;
			rect.top += points.y - u::window_pos.y;

			if (u::window_pos.x >= 0 &&
				u::window_pos.x <= u::size.width &&
				u::window_pos.y >= 0 && u::window_pos.y <= 19) {

				SetWindowPos(
					u::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0,
					0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
			}
		}

		return 0L;
	}

	return DefWindowProc(
		window,
		message,
		w_param,
		l_param
	);
}
