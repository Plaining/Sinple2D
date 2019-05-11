#pragma once

#include "stdafx.h"
#include "GraphicsContext.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "Math.h"

namespace Simple2D {
	GraphicsContext::GraphicsContext(RenderWindow * renderWindow)
		:pRenderWindow(renderWindow)
		,pRenderer(nullptr)
		,openglRenderContext(0)
		,deviceContext(0)
	{
		this->createOpenGLContext();
		pRenderer = new Renderer();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	GraphicsContext::~GraphicsContext()
	{
		wglDeleteContext(openglRenderContext);
		ReleaseDC(pRenderWindow->getHwnd(), deviceContext);
		delete pRenderer;
	}
	void GraphicsContext::createOpenGLContext()
	{
		if (openglRenderContext == 0) {
			deviceContext = GetDC(pRenderWindow->getHwnd());
			PIXELFORMATDESCRIPTOR pfd = { 0 };
			int color_deep = GetDeviceCaps(deviceContext, BITSPIXEL);
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = color_deep;
			pfd.cDepthBits = 0;
			pfd.cStencilBits = 0;
			pfd.iLayerType = PFD_MAIN_PLANE;
			int pixle_format = ChoosePixelFormat(deviceContext, &pfd);
			SetPixelFormat(deviceContext, pixle_format, &pfd);

			/*创建opengl渲染上下文*/
			openglRenderContext = wglCreateContext(deviceContext);
			if (openglRenderContext == 0) exit(0);

			/* 选择 openglRenderContext 作为当前线程的 openglRenderContext */
			if (wglMakeCurrent(deviceContext, openglRenderContext) == 0) exit(1);

			/* GLEW 是用来管理 OpenGL 的函数指针的，所以在调用任何 OpenGL 的函数之前我们需要初始化GLEW */
			if (glewInit() != GLEW_OK) exit(1);

			/* 设置视口，大小为客户区大小 */
			SIZE size = pRenderWindow->getClientSize();
			glViewport(0, 0, size.cx, size.cy);
		}
	}
	
	void GraphicsContext::flip()
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		/* 调用glClear函数来清空屏幕的颜色缓冲 */
		glClear(GL_COLOR_BUFFER_BIT);

		// 交换当前缓冲区和后台缓冲区
		pRenderer->render();
		SwapBuffers(deviceContext);
	}
}