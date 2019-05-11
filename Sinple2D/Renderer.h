#pragma once
//������Ⱦ��������

#include "common.h"
#include "Math.h"
#include <vector>
#include <map>

namespace Simple2D {
	class Texture;
	/*��Ⱦ����*/
	enum RenderType {
		RENDER_TYPE_LINES,
		RENDER_TYPE_TRIANGLES,
		RENDER_TYPE_TEXTURE
	};
	/*��Ⱦ��Ԫ�Ľṹ*/
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
			/*��������*/
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
		/*��ȾvertexData buffer*/
		Renderer();
		~Renderer();
		void render();
		void renderVertexData(VertexData* vertexData, GLuint texture=-1);
		/*����pushData��pushIndex��RenderUnit��������push��render�У�ת��ΪVertexData��
		�ٵ���renderVertexData������Ⱦ����Ӧ���ǵݹ���ã�*/
		void pushRenderUnit(const RenderUnit& unit);
	private:
		void initBuffers();
		/*����λ��������Ҫ���о���任*/
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