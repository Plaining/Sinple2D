#pragma once
//#define GLAD_STATIC

#include "stdafx.h"
#include "common.h"
#include <Windows.h>
#include "GL\glew.h"

namespace Simple2D {
	class RenderWindow;
	class TextRender;
	class Renderer;

	class DLL_export GraphicsContext {
	public:
		GraphicsContext(RenderWindow* renderWindow);
		~GraphicsContext();

		void flip();
		Renderer* getRenderer() { return pRenderer; }
	private:
		void createOpenGLContext();

		RenderWindow* pRenderWindow;
		Renderer* pRenderer;
		HGLRC openglRenderContext;
		HDC deviceContext;
		TextRender* pTextRender;
	};
}