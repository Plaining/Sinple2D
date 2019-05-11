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

			/*����opengl��Ⱦ������*/
			openglRenderContext = wglCreateContext(deviceContext);
			if (openglRenderContext == 0) exit(0);

			/* ѡ�� openglRenderContext ��Ϊ��ǰ�̵߳� openglRenderContext */
			if (wglMakeCurrent(deviceContext, openglRenderContext) == 0) exit(1);

			/* GLEW ���������� OpenGL �ĺ���ָ��ģ������ڵ����κ� OpenGL �ĺ���֮ǰ������Ҫ��ʼ��GLEW */
			if (glewInit() != GLEW_OK) exit(1);

			/* �����ӿڣ���СΪ�ͻ�����С */
			SIZE size = pRenderWindow->getClientSize();
			glViewport(0, 0, size.cx, size.cy);
		}
	}
	
	void GraphicsContext::flip()
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		/* ����glClear�����������Ļ����ɫ���� */
		glClear(GL_COLOR_BUFFER_BIT);

		// ������ǰ�������ͺ�̨������
		pRenderer->render();
		SwapBuffers(deviceContext);
	}
}