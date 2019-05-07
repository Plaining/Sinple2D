#pragma once
//#define GLAD_STATIC

#include "stdafx.h"
#include "common.h"
#include <Windows.h>
#include "GL\glew.h"

namespace Simple2D {
	class RenderWindow;
	class DLL_export GraphicsContext {
	public:
		GraphicsContext(RenderWindow* renderWindow);
		~GraphicsContext();

		void createOpenGLContext();
		void flip();
	private:
		RenderWindow* pRenderWindow;
		HGLRC openglRenderContext;
		HDC deviceContext;
	};
}