#pragma once
//用来渲染顶点数据

#include "common.h"
#include "Math.h"
#include <vector>

namespace Simple2D {
	/*渲染类型*/
	enum RenderType {
		RENDER_TYPE_LINES,
		RENDER_TYPE_TRIANGLES,
		RENDER_TYPE_TEXTURE
	};
	/*渲染单元的结构*/
	struct RenderUnit {
		Vec3* pPositions;
		int nPositionCount;

		Color* pColors;
		bool bSameColor;
		GLuint* pIndices;
		int nIndexCount;

		RenderType renderType;
	};
	class DLL_export Renderer {
		class VertexData {
		public:
			std::vector<Vec3> positions;
			std::vector<Vec2> texcoords;
			std::vector<Color> colors;
			std::vector<GLuint> indices;

			int nPositionCount;
			int nIndexCount;
			bool bHasTexcoord;

			RenderType renderType;

			void clear() {
				nPositionCount = 0;
				nIndexCount = 0;
			}
			void resize(int positionCount, int indexCount) {
				if (positions.size() - nPositionCount < positionCount) {
					positions.resize(positions.size() + positionCount);
					colors.resize(colors.size() + positionCount);
					if (bHasTexcoord)
						texcoords.resize(texcoords.size() + positionCount);
				}
				if (indices.size() - nIndexCount < indexCount) {
					indices.resize(indices.size() + indexCount);
				}
			}
			void pushData(const Vec3& pos, const Color& color) {
				positions[nPositionCount] = pos;
				colors[nPositionCount++] = color;
			}
			void pushData(const Vec3& pos, const Vec2& texcoord, const Color& color) {
				positions[nPositionCount] = pos;
				texcoords[nPositionCount] = texcoord;
				colors[nPositionCount++] = color;
			}
			void pushIndex(GLuint index) {
				indices[nIndexCount++] = index;
			}
		};
	public:
		Renderer();
		~Renderer();
		void render();
		void renderVertexData(VertexData& vertexData);

		void pushRenderUnit(const RenderUnit& unit);
	private:
		void initBuffers();
		Vec3 tranformPosition(Vec3& pos);
	private:
		VertexData triangleData;
		VertexData lineData;
		GLuint positionBuffer;
		GLuint colorBuffer;
		GLuint indexBuffer;
		GLuint VAO;
		Matrix4 mTransformMatrix;
	};
}