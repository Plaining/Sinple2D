#pragma once

#include "stdafx.h"
#include "GraphicsContext.h"
#include "RenderWindow.h"
#include "Renderer.h"
#include "Math.h"

const GLchar *shader_vs = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec4 color;\n"
"uniform mat4 projection;\n"
"out vec4 Vcolor;\n"
"void main(){\n"
"gl_Position = vec4(position, 1.0f);\n"
"Vcolor = color;\n"
"}";

const GLchar *shader_frag = "#version 330 core\n"
"out vec4 color;\n"
"in vec4 Vcolor;\n"
"void main(){\n"
"color = Vcolor;\n"
"}";

namespace Simple2D {
	GraphicsContext::GraphicsContext(RenderWindow * renderWindow)
		:pRenderWindow(renderWindow)
		,pRenderer(nullptr)
		,openglRenderContext(0)
		,deviceContext(0)
	{
		this->createOpenGLContext();
		this->createShaderProgram();
		pRenderer = new Renderer();
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
	void GraphicsContext::createShaderProgram()
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &shader_vs, NULL);
		glCompileShader(vertexShader);
		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			int tmp = 0;
		}
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &shader_frag, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			int tmp = 0;
		}
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			int tmp = 0;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glUseProgram(shaderProgram);
	}
	void GraphicsContext::flip()
	{
		/* ʹ��һ���Զ������ɫ�����Ļ������ʹ�ú�ɫ */
		glClearColor(1.0f, 0, 0, 1.0f);

		/* ����glClear�����������Ļ����ɫ���� */
		glClear(GL_COLOR_BUFFER_BIT);

		// ������ǰ�������ͺ�̨������
		pRenderer->render();
		SwapBuffers(deviceContext);
	}
}