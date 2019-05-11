#pragma once
//用来渲染顶点数据

#include "common.h"
#include "Math.h"
#include <vector>
#include <map>

namespace Simple2D {
	class Texture;
	/*渲染类型*/
	enum RenderType {
		RENDER_TYPE_LINES,
		RENDER_TYPE_TRIANGLES,
		RENDER_TYPE_TEXTURE
	};
	/*渲染单元的结构*/
	struct RenderUnit {
		Vec3* pPositions;
		Vec2* pTexcoords;
		int nPositionCount;

		Color color;

		GLuint* pIndices;
		int nIndexCount;

		Texture* texture;

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
			/*输入数据*/
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
		/*渲染vertexData buffer*/
		Renderer();
		~Renderer();
		void render();
		void renderVertexData(VertexData* vertexData, GLuint texture=-1);
		/*调用pushData和pushIndex将RenderUnit顶点数据push到render中，转化为VertexData，
		再调用renderVertexData函数渲染，那应该是递归调用？*/
		void pushRenderUnit(const RenderUnit& unit);
	private:
		void initBuffers();
		/*顶点位置向量需要进行矩阵变换*/
		Vec3 tranformPosition(Vec3& pos);
		void createShaderProgram(const char* vertexName, const char* fragmentName);

	private:
		VertexData triangleData;
		VertexData lineData;

		std::map<Texture*, VertexData*> textureDatas;
		GLuint shaderProgram;
		GLuint vertexShader, fragmentShader;
		
		GLuint positionBuffer;
		GLuint texcoordBuffer;
		GLuint colorBuffer;
		GLuint indexBuffer;
		GLuint VAO;

		Matrix4 mTransformMatrix;
	};
}