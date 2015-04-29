
#ifndef TEXTURECUBEMAP_H_
#define TEXTURECUBEMAP_H_

#include "texture.h"

class TextureCubemap : public Texture
{
public:
	TextureCubemap():Texture() {};
	~TextureCubemap();

	GLenum GetTextureType() const {return GL_TEXTURE_CUBE_MAP;}
	bool Load(string name);
};

#endif /* TEXTURECUBEMAP_H_ */
