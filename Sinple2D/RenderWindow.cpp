// Sinple2D.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "RenderWindow.h"
#include "Resource.h"
#include <Exception>

#define CLASS_NAME "Simple2DApplication"


namespace Simple2D {
	RenderWindow::RenderWindow(int width, int height, const char * title)
	{
		clientSize.cx = width;
		clientSize.cy = height;
		this->createWindow(width, height, title);
	}

	RenderWindow::~RenderWindow()
	{
	}

	void RenderWindow::createWindow(int width, int height, const char * title)
	{
		WNDCLASS wndclass;
		memset(&wndclass, 0, sizeof(wndclass));
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wndclass.lpfnWndProc = (WNDPROC)RenderWindow::static_window_proc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = (HINSTANCE)GetModuleHandle(0);
		wndclass.hIcon = 0;
		wndclass.hCursor = 0;
		wndclass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		wndclass.lpszMenuName = 0;
		wndclass.lpszClassName = TEXT(CLASS_NAME);
		RegisterClass(&wndclass);

		hwnd = CreateWindowA(CLASS_NAME, title, WS_OVERLAPPEDWINDOW, 100, 100, width, height, NULL, NULL, (HINSTANCE)GetModuleHandle(0), this);

		if (hwnd == NULL) {
			throw std::exception("Unable to create window");
		}
		ShowWindow(hwnd, SW_SHOWNA);
		setClientSize(width, height);
	}

	void RenderWindow::setClientSize(int width, int height)
	{
		RECT rectProgram, rectClient;
		GetWindowRect(hwnd, &rectProgram);
		GetClientRect(hwnd, &rectClient);
		int borderw = rectProgram.right - rectProgram.left - (rectClient.right - rectClient.left);
		int borderh = rectProgram.bottom - rectProgram.top - (rectClient.bottom - rectClient.top);
		borderw += width;
		borderh += height;
		int showToScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - borderw / 2;
		int showToScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - borderh / 2;
		MoveWindow(hwnd, showToScreenX, showToScreenY, borderw, borderh, false);
	}
	LRESULT WINAPI RenderWindow::static_window_proc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		RenderWindow* self = nullptr;
		if (msg == WM_CREATE) {
			LPCREATESTRUCT create_struct = (LPCREATESTRUCT)lParam;
			self = (RenderWindow*)create_struct->lpCreateParams;
			SetWindowLongPtr(wnd, GWLP_USERDATA, (LONG_PTR)self);
			self->hwnd = wnd;
		}
		else {
			self = (RenderWindow*)GetWindowLongPtr(wnd, GWLP_USERDATA);
		}
		switch (msg) {
			case WM_CLOSE: {
				PostQuitMessage(0);
				break;
			}
			case WM_DESTROY: {
				PostQuitMessage(0);
				break;
			}
			case WM_PAINT:
			{
				RECT rect;
				if (GetUpdateRect(wnd, &rect, FALSE)) {
					ValidateRect(wnd, &rect);
				}
				break;
			}
			
		}
		return DefWindowProc(wnd,msg,wParam,lParam);
	}

}