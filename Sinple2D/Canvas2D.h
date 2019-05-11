#pragma once
#include "common.h"
#include "Math.h"
#include <vector>

namespace Simple2D {
	class Renderer;
	class Texture;
	class DLL_export Canvas2D {
	public:
		Canvas2D(Renderer* renderer);
		~Canvas2D();
		void drawLine(int x1, int y1, int z1, int x2, int y2, int z2, Color& color);
		void drawLines(Vec3* positions, Color& colors, int positionCount, bool sameColor = true);
		void drawTriangles(Vec3* positions, Color& colors, int positionCount, GLuint* indices, int indexCount, bool sameColor = true);
		void drawRect(float x, float y, float w, float h, Color color);
		void fillRect(float x, float y, float w, float h, Color color);
		void drawCircle(const Vec3& center, int radius, Color& color, int nSlice = 36);
		void fillCircle(const Vec3& center, int radius, int degrees, Color& color);
		void fillCircle(const Vec3& center, int in_radius, int out_radius, int beginAngle, int endAngle, Color& color);
		void drawTexture(int x, int y, Texture* texture, Color& color);
	private:
		/*定义vPositions和vIndices vector的大小*/
		void resizeVector(int positionCount, int indexCount);
		Renderer* pRenderer;
		std::vector<Vec3> vPositions;
		int nPositionCount;
		std::vector<GLuint> vIndices;
		int nIndexCount;
	};
}