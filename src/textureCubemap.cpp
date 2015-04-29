#include "textureCubemap.h"

bool TextureCubemap::Load(string name)
{
	if(!GenerateTexture())
	{
		return false;
	}

	BindTexture();

	string file;
	stringstream str_stream;
	str_stream.str(name);

	if(!name.empty())
	{
		int count=0;
		while(std::getline(str_stream, file, ' '))
		{
			//cout << file << endl;
			if(count >= 6)
			{
				cout << "error:more than 6 pics" << endl;
				return false;
			}

			if(!LoadTexture(file, GL_TEXTURE_CUBE_MAP_POSITIVE_X + count, true))
			{
				cout << "error:loding texture: " << file << endl;
				return false;
			}

			++count;
		}

		if(count != 6)
		{
			cout << "error:less than 6 pics" << endl;
			return false;
		}
	}

	SetFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
	SetWrap();

	UnBindTexture();

	return true;
}

TextureCubemap::~TextureCubemap()
{
	DeleteTexture();
}
