#include "stdafx.h"
#include "Renderer.h"

namespace Simple2D {
	Renderer::Renderer()
	{
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
		glDeleteBuffers(1, &colorBuffer);
		glDeleteBuffers(1, &indexBuffer);
		glDeleteVertexArrays(1, &VAO);
	}
	void Renderer::render()
	{
		if (triangleData.nPositionCount > 0) {
			this->renderVertexData(triangleData);
		}
		if (lineData.nPositionCount > 0) {
			this->renderVertexData(lineData);
		}
	}
	void Renderer::renderVertexData(VertexData & vertexData)
	{

		/* 填充顶点数据  */
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * vertexData.nPositionCount, &vertexData.positions[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * vertexData.nPositionCount, &vertexData.colors[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * vertexData.nIndexCount, &vertexData.indices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
		glBindVertexArray(VAO);
		switch (vertexData.renderType) {
		case RENDER_TYPE_TRIANGLES:
			glDrawElements(GL_TRIANGLES, vertexData.nIndexCount, GL_UNSIGNED_INT, 0);
			break;
		case RENDER_TYPE_LINES:
			glDrawElements(GL_LINES, vertexData.nIndexCount, GL_UNSIGNED_INT, 0);
			break;
		}
		glBindVertexArray(0);

		vertexData.clear();
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
		/*填充数据*/
		vertexData->resize(unit.nPositionCount, unit.nIndexCount);
		int baseIndex = vertexData->nPositionCount;
		for (int i = 0; i < unit.nPositionCount; i++) {
			if (unit.bSameColor) {
				vertexData->pushData(tranformPosition(unit.pPositions[i]), unit.pColors[0]);
			}
			else {
				vertexData->pushData(tranformPosition(unit.pPositions[i]), unit.pColors[1]);
			}
		}
		for (int i = 0; i < unit.nIndexCount; i++) {
			vertexData->pushIndex(baseIndex + unit.pIndices[i]);

		}
	}
	void Renderer::initBuffers()
	{
		glGenBuffers(1, &positionBuffer);
		glGenBuffers(1, &colorBuffer);
		glGenBuffers(1, &indexBuffer);
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		/* 设置顶点属性指针 */
		glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Color), (GLvoid*)0);
		glEnableVertexAttribArray(1);

		/* 解绑缓冲区 */
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	Vec3 Renderer::tranformPosition(Vec3 & pos)
	{
		return mTransformMatrix*pos;
	}
}
