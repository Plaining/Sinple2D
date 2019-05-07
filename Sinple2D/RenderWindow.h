#pragma once
#include "stdafx.h"
#include "common.h"
#include <Windows.h>
#include <string>

namespace Simple2D
{
	class DLL_export RenderWindow
	{
	public:
		RenderWindow(int width, int height, const char* title = "Simple2D");
		~RenderWindow();

		void createWindow(int width, int height, const char* title);
		void setClientSize(int width, int height);

		static LRESULT WINAPI static_window_proc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
		HWND getHwnd() { return hwnd; }
		SIZE getClientSize() { return clientSize; }
	private:
		HWND hwnd;
		SIZE clientSize;
	};
}