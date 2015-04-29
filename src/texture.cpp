#include "texture.h"

Texture::Texture():
	m_iWidth(0),
	m_iHeight(0),
	m_iBPP(0),
	m_textureId(0),
	m_samplerId(0),
	m_bMipMapsGenerated(false),
	m_tfMinification(0),
	m_tfMagnification(0)
{

}

Texture::~Texture()
{
	DeleteTexture();
}


void Texture::CreateEmptyTexture(int a_iWidth, int a_iHeight, GLenum format)
{
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	if(format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
	// We must handle this because of internal format parameter
	else if(format == GL_RGB || format == GL_BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, NULL);

	glGenSamplers(1, &m_samplerId);
}

bool Texture::LoadTexture(string a_sPath, GLenum target, bool bGenerateMipMaps)
{
	int width, height, bpp;

	//texture path prefix
	a_sPath = "./data/textures/" + a_sPath;
	//cout << a_sPath << endl;

	BYTE * bDataPointer = ImageTool::GetInstance().LoadImage(a_sPath, width, height, bpp);

	if(bDataPointer == NULL) return false;

	GLenum format;
	if(bpp == 32)format = GL_BGRA;
	if(bpp == 24)format = GL_BGR;
	if(bpp == 8)format = GL_LUMINANCE;

	CreateFromData(bDataPointer, target, width, height, bpp, format, bGenerateMipMaps);

	m_sPath = a_sPath;

	delete[] bDataPointer;
	return true; // Success
}

void Texture::CreateFromData(BYTE* bData, GLenum target, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps)
{
	if(format == GL_RGBA || format == GL_BGRA)
		glTexImage2D(target, 0, GL_RGBA, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	// We must handle this because of internal format parameter
	else if(format == GL_RGB || format == GL_BGR)
		glTexImage2D(target, 0, GL_RGB, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);
	else
		glTexImage2D(target, 0, format, a_iWidth, a_iHeight, 0, format, GL_UNSIGNED_BYTE, bData);

	if(bGenerateMipMaps)glGenerateMipmap(GetTextureType());

	m_sPath = "";
	m_bMipMapsGenerated = bGenerateMipMaps;
	m_iWidth = a_iWidth;
	m_iHeight = a_iHeight;
	m_iBPP = a_iBPP;
}

void Texture::SetSamplerParameter(GLenum parameter, GLenum value)
{
	glSamplerParameteri(m_samplerId, parameter, value);
}

void Texture::SetWrap(int type)
{
	if(GetTextureType() == GL_TEXTURE_CUBE_MAP)
	{
		glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	else if((type != 0) &&(GetTextureType() == GL_TEXTURE_2D))
	{
		glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_S, type);
		glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_T, type);
	}
	else if(GetTextureType() == GL_TEXTURE_2D)
	{
		glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glSamplerParameteri(m_samplerId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}

void Texture::SetFiltering(int a_tfMagnification, int a_tfMinification)
{
	// Set magnification filter
	if(a_tfMagnification == TEXTURE_FILTER_MAG_NEAREST)
		glSamplerParameteri(m_samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else if(a_tfMagnification == TEXTURE_FILTER_MAG_BILINEAR)
		glSamplerParameteri(m_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set minification filter
	if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST)
		glSamplerParameteri(m_samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR)
		glSamplerParameteri(m_samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
		glSamplerParameteri(m_samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
		glSamplerParameteri(m_samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	else if(a_tfMinification == TEXTURE_FILTER_MIN_TRILINEAR)
		glSamplerParameteri(m_samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	m_tfMinification = a_tfMinification;
	m_tfMagnification = a_tfMagnification;
}

bool Texture::GenerateTexture()
{
	DeleteTexture();
	glGenTextures(1, &m_textureId);
	glGenSamplers(1, &m_samplerId);

	if((m_textureId == 0) || (m_samplerId == 0))
	{
		cout << "wrong id" << std::endl;
		return false;
	}
	//cout << m_textureId << " " << m_samplerId << endl;
	return true;
}

void Texture::BindTexture(int iTextureUnit) const
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GetTextureType(), m_textureId);
//	glUniform1i(brickSamplerId , 0);   // sets sampler
	glBindSampler(iTextureUnit, m_samplerId);
}

void Texture::UnBindTexture(int iTextureUnit) const
{
	glActiveTexture(GL_TEXTURE0+iTextureUnit);
	glBindTexture(GetTextureType(), 0);
}

void Texture::DeleteTexture()
{
	glDeleteSamplers(1, &m_samplerId);
	glDeleteTextures(1, &m_textureId);
	m_samplerId = 0;
	m_textureId = 0;
}

int Texture::GetMinificationFilter()
{
	return m_tfMinification;
}

int Texture::GetMagnificationFilter()
{
	return m_tfMagnification;
}

int Texture::GetWidth()
{
	return m_iWidth;
}

int Texture::GetHeight()
{
	return m_iHeight;
}

int Texture::GetBPP()
{
	return m_iBPP;
}

UINT Texture::GetTextureID()
{
	return m_textureId;
}

void Texture::test()
{
	cout << m_iWidth << " "  << m_iHeight << " " << m_iBPP << endl; // Texture width, height, and bytes per pixel
	cout << m_textureId << endl;// Texture id
	cout << m_samplerId << endl; // Texture id
	cout << m_sPath << endl; // Texture id
}
