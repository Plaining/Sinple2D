#pragma once
#include "common.h"
#include "Math.h"
#include "PathHelper.h"
#include "GL\glew.h"

#include <map>

namespace Simple2D {
	struct DLL_export Texture {
		Rect size;
		Vec2 texcoords[4];
		GLuint texture;
	};
	class DLL_export TextureManager {
		TextureManager();
		~TextureManager();
	public:
		static TextureManager* instance();
		Texture* getTexture(const char* filename);
	private:
		Texture* createTexture(const char* filename);
		std::map<const char*, Texture*> mTextureMap;
	};
}