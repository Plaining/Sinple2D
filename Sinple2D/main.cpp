#pragma once

#include "stdafx.h"
#include "RenderWindow.h"
#include "GraphicsContext.h"
#include "common.h"
#include <Exception>
using namespace Simple2D;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	RenderWindow window(DEFAULT_WIN_W, DEFAULT_WIN_H);
	GraphicsContext graphicsContext(&window);
	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			graphicsContext.flip();
		}
	}
	return 0;
}