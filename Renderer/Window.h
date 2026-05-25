#pragma once
#include <Windows.h>
#include <cstdint>

#define BLACK 0x000000
#define GREEN 0x00ff26
#define BLUE 0x00fbff
#define DBLUE 0x0055ff
#define PURPLE 0xc300ff
#define PINK 0xff00d9
#define RED 0xff0011
#define YELLOW 0xff0011
#define ORANGE 0xff6f00


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
	Window(int w, int h);
	~Window();

	bool processMessages();
	uint32_t* getBuffer();
	void render();
	void clearScreen();
	int getWidth();
	int getHeight();

private:
	HINSTANCE mHinstance;
	HWND mHwnd;

	int width;
	int height;

	uint32_t* buffer;
	BITMAPINFO bmi;
};

