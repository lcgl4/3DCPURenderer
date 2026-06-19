#include "Window.h"

extern Window* g_WindowInstance;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		if (g_WindowInstance) g_WindowInstance->setKeyState(wParam, true);
		break;
	case WM_KEYUP:
		if (g_WindowInstance) g_WindowInstance->setKeyState(wParam, false);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


Window::Window(int w, int h) : mHinstance(GetModuleHandle(nullptr))
{
	g_WindowInstance = this;

	width = w;
	height = h;
	keys = new bool[256] {false};

	buffer = new uint32_t[width * height];

	ZeroMemory(&bmi, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;



	const wchar_t* CLASS_NAME = L"Window";

	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = mHinstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;


	RECT rect;
	rect.left = 200;
	rect.top = 200;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, style, false);

	mHwnd = CreateWindowEx(0, CLASS_NAME, L"Renderer", style, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, mHinstance, NULL);

	ShowWindow(mHwnd, SW_SHOW);
}

Window::~Window()
{
	const wchar_t* CLASS_NAME = L"Window";

	UnregisterClass(CLASS_NAME, mHinstance);

	delete[] buffer;
	delete[] keys;
}

bool Window::processMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

uint32_t* Window::getBuffer()
{
	return buffer;
}

void Window::clearScreen() {


	for (int i = 0; i < width * height; i++)
		buffer[i] = BLACK;
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}

bool Window::isKeyPressed(int virtualKeyCode)
{
	return keys[virtualKeyCode];
}

void Window::setKeyState(int virtualKeyCode, bool isDown)
{
	keys[virtualKeyCode] = isDown;
}

void Window::render()
{

	HDC hdc = GetDC(mHwnd);

	StretchDIBits(
		hdc,
		0, 0, width, height,
		0, 0, width, height,
		buffer,
		&bmi,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	ReleaseDC(mHwnd, hdc);
}
