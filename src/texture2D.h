#ifndef TEXTURE2D_H_
#define TEXTURE2D_H_

#include "texture.h"

class Texture2D : public Texture
{
public:
	Texture2D():Texture() {};

	GLenum GetTextureType() const {return GL_TEXTURE_2D;}
	bool Load(string name);
};



#endif /* TEXTURE2D_H_ */
