#pragma once
#include "stdafx.h"
#include "Canvas2D.h"
#include "Renderer.h"
#include "TextureManager.h"

namespace Simple2D {
	Canvas2D::Canvas2D(Renderer * renderer)
		:pRenderer(renderer)
	{
	}
	Canvas2D::~Canvas2D()
	{
	}
	void Canvas2D::drawLine(int x1, int y1, int z1, int x2, int y2, int z2, Color& color)
	{
		this->resizeVector(2, -1);
		vPositions[0].set(x1, y1, z1);
		vPositions[1].set(x2, y2, z2);

		this->drawLines(&vPositions[0], color, 2);
	}
	void Canvas2D::drawLines(Vec3 * positions, Color & colors, int positionCount, bool sameColor)
	{
		int indexCount = (positionCount - 1) * 2;
		this->resizeVector(-1, indexCount);

		for (int i = 0; i < positionCount - 1; i++) {
			vIndices[i * 2 + 0] = i;
			vIndices[i * 2 + 1] = i + 1;
		}

		static RenderUnit unit;
		unit.pPositions = positions;
		unit.nPositionCount = positionCount;
		unit.pIndices = &vIndices[0];
		unit.nIndexCount = indexCount;
		unit.color = colors;
		unit.renderType = RENDER_TYPE_LINES;

		pRenderer->pushRenderUnit(unit);
	}
	void Canvas2D::drawTriangles(Vec3 * positions, Color & colors, int positionCount, GLuint * indices, int indexCount, bool sameColor)
	{
		static RenderUnit unit;
		unit.pPositions = positions;
		unit.nPositionCount = positionCount;
		unit.pIndices = indices;
		unit.nIndexCount = indexCount;
		unit.color = colors;
		unit.renderType = RENDER_TYPE_TRIANGLES;

		pRenderer->pushRenderUnit(unit);
	}
	void Canvas2D::drawRect(float x, float y, float w, float h, Color color)
	{
		this->resizeVector(5, 8);

		vPositions[0].set(x + 0, y + 0, 0);
		vPositions[1].set(x + 0, y + h, 0);
		vPositions[2].set(x + w, y + h, 0);
		vPositions[3].set(x + w, y + 0, 0);
		vPositions[4].set(x + 0, y + 0, 0);

		this->drawLines(&vPositions[0], color, 5);
	}
	void Canvas2D::fillRect(float x, float y, float w, float h, Color color)
	{
		this->resizeVector(4, 6);

		vPositions[0].set(x + 0, y + 0, 0);
		vPositions[1].set(x + 0, y + h, 0);
		vPositions[2].set(x + w, y + h, 0);
		vPositions[3].set(x + w, y + 0, 0);

		vIndices[0] = 0;
		vIndices[1] = 2;
		vIndices[2] = 1;
		vIndices[3] = 0;
		vIndices[4] = 3;
		vIndices[5] = 2;

		this->drawTriangles(&vPositions[0], color, 4, &vIndices[0], 6);
	}
	void Canvas2D::drawCircle(const Vec3 & center, int radius, Color & color, int nSlice)
	{
		int nVertexCount = radius <= 100 ? nSlice : nSlice + 32;
		this->resizeVector(nVertexCount + 1, -1);
		for (int i = 0; i <= nVertexCount; i++) {
			vPositions[i].set(
				center.x + cos(i / GLfloat(nVertexCount)*PI_2)*radius,
				center.y + sin(i / GLfloat(nVertexCount)*PI_2)*radius,
				center.z
			);
		}
		this->drawLines(&vPositions[0], color, nVertexCount + 1);
	}
	void Canvas2D::fillCircle(const Vec3 & center, int radius, int degrees, Color & color)
	{
		this->fillCircle(center, 0, radius, 0, degrees, color);
	}
	void Canvas2D::fillCircle(const Vec3 & center, int in_radius, int out_radius, int beginAngle, int endAngle, Color & color)
	{
		int nSlice = (out_radius <= 50) ? 32 : 64;

		float fBeginRadian = toRadian(beginAngle);
		float fEndRadian = toRadian(endAngle);

		nSlice = (fEndRadian - fBeginRadian) / PI_2 * nSlice;

		if (in_radius == 0) {
			this->resizeVector(nSlice + 2, nSlice * 3);
			vPositions[nPositionCount++] = center;

			for (int i = 0; i <= nSlice; i++) {
				float fTmp = fBeginRadian + (fEndRadian - fBeginRadian) * (i / float(nSlice));

				vPositions[nPositionCount++].set(
					center.x + sin(fTmp) * out_radius,
					center.y + cos(fTmp) * out_radius,
					center.z);

				if (i >= 1) {
					vIndices[nIndexCount++] = 0;
					vIndices[nIndexCount++] = i + 1;
					vIndices[nIndexCount++] = i;
				}
			}
		}
		else {
			this->resizeVector((nSlice + 1) * 2, nSlice * 6);
			for (int i = 0; i <= nSlice; i++) {
				float fTmp = fBeginRadian + (fEndRadian - fBeginRadian) * (i / float(nSlice));

				vPositions[nPositionCount++].set(
					center.x + sin(fTmp) * out_radius,
					center.y + cos(fTmp) * out_radius,
					center.z);

				vPositions[nPositionCount++].set(
					center.x + sin(fTmp) * in_radius,
					center.y + cos(fTmp) * in_radius,
					center.z);

				if (i >= 1) {
					vIndices[nIndexCount++] = i * 2 - 1;
					vIndices[nIndexCount++] = i * 2 - 0;
					vIndices[nIndexCount++] = i * 2 - 2;
					vIndices[nIndexCount++] = i * 2 + 1;
					vIndices[nIndexCount++] = i * 2 - 0;
					vIndices[nIndexCount++] = i * 2 - 1;
				}
			}
		}

		this->drawTriangles(&vPositions[0], color, nPositionCount, &vIndices[0], nIndexCount);
	}
	void Canvas2D::drawTexture(int x, int y, Texture * texture, Color & color)
	{
		int w = texture->size.w;
		int h = texture->size.h;
		this->resizeVector(4, 6);
		vPositions[0].set(x + 0, y + 0, 0);
		vPositions[1].set(x + 0, y + h, 0);
		vPositions[2].set(x + w, y + h, 0);
		vPositions[3].set(x + w, y + 0, 0);
		vIndices[0] = 0;
		vIndices[1] = 2;
		vIndices[2] = 1;
		vIndices[3] = 0;
		vIndices[4] = 3;
		vIndices[5] = 2;
		static RenderUnit unit;
		unit.pPositions = &vPositions[0];
		unit.nPositionCount = 4;
		unit.pTexcoords = texture->texcoords;
		unit.pIndices = &vIndices[0];
		unit.nIndexCount = 6;
		unit.color = color;
		unit.texture = texture;
		unit.renderType = RENDER_TYPE_TEXTURE;
		pRenderer->pushRenderUnit(unit);
	}
	void Canvas2D::resizeVector(int positionCount, int indexCount)
	{
		if (positionCount != -1 && vPositions.size() < positionCount) {
			vPositions.resize(positionCount);
		}
		if (indexCount != -1 && vIndices.size() < indexCount) {
			vIndices.resize(indexCount);
		}
		nPositionCount = 0;
		nIndexCount = 0;
	}
}