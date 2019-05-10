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
		/* 使用一个自定义的颜色清空屏幕，这里使用红色 */
		glClearColor(1.0f, 0, 0, 1.0f);

		/* 调用glClear函数来清空屏幕的颜色缓冲 */
		glClear(GL_COLOR_BUFFER_BIT);

		// 交换当前缓冲区和后台缓冲区
		pRenderer->render();
		SwapBuffers(deviceContext);
	}
}