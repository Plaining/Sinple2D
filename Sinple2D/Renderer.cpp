#include "stdafx.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "PathHelper.h"

#include<fstream>
#include<sstream>

namespace Simple2D {
	Renderer::Renderer()
	{
		this->initBuffers();
		this->createShaderProgram("Shader/shader.vs", "Shader/shader.frag");

		Matrix4 ortho = Matrix4::ortho(0, 800, 600, 0, -1, 1);
		Matrix4 tranform = Matrix4::makeTransform(Vec3(0, 600, 0), Vec3(1, -1, 1));
		mTransformMatrix = ortho*tranform;
		this->initBuffers();

		triangleData.renderType = RENDER_TYPE_TRIANGLES;
		lineData.renderType = RENDER_TYPE_LINES;
	}
	Renderer::~Renderer()
	{
		glDeleteBuffers(1, &positionBuffer);
		glDeleteBuffers(1, &texcoordBuffer);
		glDeleteBuffers(1, &colorBuffer);
		glDeleteBuffers(1, &indexBuffer);
		glDeleteVertexArrays(1, &VAO);
	}
	void Renderer::render()
	{
		glUniform1i(glGetUniformLocation(shaderProgram, "bRenderTexture"), 1);
		for (auto tex : textureDatas) {
			this->renderVertexData(tex.second, tex.first->texture);
		}
		for (auto tex : textureDatas) {
			delete tex.second;
		}
		textureDatas.clear();
		if (triangleData.nPositionCount > 0) {
			glUniform1i(glGetUniformLocation(shaderProgram, "bRenderTexture"), 0);
			this->renderVertexData(&triangleData);
		}
		if (lineData.nPositionCount > 0) {

			glUniform1i(glGetUniformLocation(shaderProgram, "bRenderTexture"), 0);
			this->renderVertexData(&lineData);
		}
	}
	void Renderer::renderVertexData(VertexData * vertexData, GLuint texture)
	{
		/* 填充顶点数据  */
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * vertexData->nPositionCount, &vertexData->positions[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * vertexData->nPositionCount, &vertexData->colors[0], GL_STATIC_DRAW);
		
		/*设置纹理*/
		if (vertexData->bHasTexcoord) {
			glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * vertexData->nPositionCount, &vertexData->texcoords[0], GL_DYNAMIC_DRAW); 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(glGetUniformLocation(shaderProgram, "defaultTexture"), 0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * vertexData->nIndexCount, &vertexData->indices[0], GL_STATIC_DRAW);
		glBindVertexArray(VAO);
		switch (vertexData->renderType) {
		case RENDER_TYPE_TRIANGLES:
			glDrawElements(GL_TRIANGLES, vertexData->nIndexCount, GL_UNSIGNED_INT, 0);
			break;
		case RENDER_TYPE_TEXTURE:
			glDrawElements(GL_TRIANGLES, vertexData->nIndexCount, GL_UNSIGNED_INT, 0);
			break;
		case RENDER_TYPE_LINES:
			glDrawElements(GL_LINES, vertexData->nIndexCount, GL_UNSIGNED_INT, 0);
			break;
		}
		glBindVertexArray(0);

		vertexData->clear();
	}
	void Renderer::pushRenderUnit(const RenderUnit & unit)
	{
		VertexData* vertexData = nullptr;
		if (unit.renderType == RENDER_TYPE_TRIANGLES) {
			vertexData = &triangleData;
		}
		else if (unit.renderType == RENDER_TYPE_LINES) {
			vertexData = &lineData;
		}
		else if (unit.renderType == RENDER_TYPE_TEXTURE) {
			auto it = textureDatas.find(unit.texture);
			if (it == textureDatas.end()) {
				vertexData = new VertexData();
				vertexData->bHasTexcoord = true;
				vertexData->renderType = RENDER_TYPE_TEXTURE;
				textureDatas.insert(std::make_pair(unit.texture, vertexData));
			}
			else {
				vertexData = it->second;
			}
		}
		/*填充数据*/
		vertexData->resize(unit.nPositionCount, unit.nIndexCount);
		int baseIndex = vertexData->nPositionCount;
		for (int i = 0; i < unit.nPositionCount; i++) {
			if (unit.renderType == RENDER_TYPE_TEXTURE) {
				vertexData->pushData(tranformPosition(unit.pPositions[i]), unit.pTexcoords[i], unit.color);
			}
			else {
				vertexData->pushData(tranformPosition(unit.pPositions[i]), unit.color);
			}
		}
		for (int i = 0; i < unit.nIndexCount; i++) {
			vertexData->pushIndex(baseIndex + unit.pIndices[i]);

		}
	}
	void Renderer::initBuffers()
	{
		glGenBuffers(1, &positionBuffer);
		glGenBuffers(1, &texcoordBuffer);
		glGenBuffers(1, &colorBuffer);
		glGenBuffers(1, &indexBuffer);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		/* 设置顶点属性指针 */
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, texcoordBuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (GLvoid*)0);
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Color), (GLvoid*)0);
		glEnableVertexAttribArray(2);

		/* 解绑缓冲区 */
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	Vec3 Renderer::tranformPosition(Vec3 & pos)
	{
		return mTransformMatrix*pos;
	}
	void Renderer::createShaderProgram(const char * vertexName, const char * fragmentName)
	{
		std::string vShaderSource, fShaderSource;
		std::ifstream vShaderFile, fShaderFile;
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);

		try {
			vShaderFile.open(PathHelper::fullPath(vertexName), std::ios::in);
			fShaderFile.open(PathHelper::fullPath(fragmentName), std::ios::in);

			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderSource = vShaderStream.str();
			fShaderSource = fShaderStream.str();

			vShaderFile.close();
			fShaderFile.close();
		}
		catch (std::ifstream::failure e) {
			throw std::exception("Error shader: file not succesfully read");
		}
		const GLchar* vShaderCode = vShaderSource.c_str();
		const GLchar* fShaderCode = fShaderSource.c_str();

		/* 创建顶点作色器  */
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			throw std::exception("");
		}

		/* 创建片段着色器 */
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			throw std::exception("");
		}

		/* 创建着色程序 */
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			throw std::exception("");
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		/* 使用着色程序 */
		glUseProgram(shaderProgram);
	}
}
