#pragma once

#include "stdafx.h"
#include "RenderWindow.h"
#include "GraphicsContext.h"
#include "common.h"
#include "Canvas2D.h"
#include "Renderer.h"

#include <Exception>
using namespace Simple2D;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	RenderWindow window(DEFAULT_WIN_W, DEFAULT_WIN_H);
	GraphicsContext graphicsContext(&window);

	Canvas2D canvas(graphicsContext.getRenderer());
	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			float n = 29;
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					if (i % 2 == 0 && j % 2 == 0) {
						canvas.fillRect(
							10 + i * 20, 10 + j * 20, 19, 19,
							Color(i / n, i / n, 1, 1));
					}
					else {
						canvas.drawRect(
							10 + i * 20, 10 + j * 20, 19, 19,
							Color(i / n, i / n, 1, 1));
					}
				}
			}
			canvas.drawLine(600, 10, 0, 700, 300, 0, Color(0, 0, 1, 1));
			canvas.drawCircle(Vec3(400, 300, 0), 200, Color(0, 0, 0, 1));

			canvas.fillCircle(Vec3(200, 400, 0), 0, 80, 0, 360, Color(1, 0, 0, 1));
			canvas.fillCircle(Vec3(400, 400, 0), 0, 80, 0, 270, Color(0, 1, 0, 1));
			canvas.fillCircle(Vec3(600, 400, 0), 60, 80, 0, 270, Color(0, 0, 1, 1));

			graphicsContext.flip();
		}
	}
	return 0;
}