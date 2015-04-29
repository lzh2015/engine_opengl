#include "texture2D.h"

bool Texture2D::Load(string name)
{
	if(!GenerateTexture())
	{
		return false;
	}

	BindTexture();

	if(!LoadTexture(name, GL_TEXTURE_2D, true))
	{
		cout << "error:loding texture: " << name << endl;
		return false;
	}

	//SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_NEAREST_MIPMAP);
	SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_TRILINEAR);
	SetWrap();

	UnBindTexture();

	return true;

}
