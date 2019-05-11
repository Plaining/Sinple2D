#include "TextureManager.h"
#include "stb_image.h"

namespace Simple2D{
	TextureManager::TextureManager()
	{
	}
	TextureManager::~TextureManager()
	{
		for (auto ele : mTextureMap) {
			glDeleteTextures(1, &ele.second->texture);
			delete ele.second;
		}
		mTextureMap.clear();
	}
	TextureManager * TextureManager::instance()
	{
		static TextureManager tm;
		return &tm;
	}
	Texture * TextureManager::getTexture(const char * filename)
	{
		auto it = mTextureMap.find(filename);
		if (it == mTextureMap.end()) {
			Texture*  texture = this->createTexture(filename);
			mTextureMap.insert(std::make_pair(filename, texture));
			return texture;
		}
		else {
			return it->second;
		}
	}
	Texture * TextureManager::createTexture(const char * filename)
	{
		GLuint texture = -1;
		std::string fullName = PathHelper::fullPath(filename);
		unsigned char* image_data = nullptr;
		int width, height, nrChannels;
		image_data = stbi_load(fullName.c_str(), &width, &height, &nrChannels, 0);
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (image_data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		free(image_data);
		Texture* tex = new Texture();
		tex->texture = texture;
		tex->size.set(0, 0, width, height);
		tex->texcoords[0].set(0, 0);
		tex->texcoords[1].set(0, 1);
		tex->texcoords[2].set(1, 0);
		tex->texcoords[3].set(1, 0);

		return tex;
	}
}