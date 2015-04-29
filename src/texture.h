#pragma once

#include "global.h"
#include "image.h"

enum ETextureFiltering
{
	TEXTURE_FILTER_MAG_NEAREST = 0, // Nearest criterion for magnification
	TEXTURE_FILTER_MAG_BILINEAR, // Bilinear criterion for magnification
	TEXTURE_FILTER_MIN_NEAREST, // Nearest criterion for minification
	TEXTURE_FILTER_MIN_BILINEAR, // Bilinear criterion for minification
	TEXTURE_FILTER_MIN_NEAREST_MIPMAP, // Nearest criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_BILINEAR_MIPMAP, // Bilinear criterion for minification, but on closest mipmap
	TEXTURE_FILTER_MIN_TRILINEAR, // Bilinear criterion for minification on two closest mipmaps, then averaged
};

class Texture
{
public:
	Texture();
	virtual ~Texture() = 0;

	virtual GLenum GetTextureType() const = 0;
	virtual bool Load(string name) = 0;
	void CreateEmptyTexture(int a_iWidth, int a_iHeight, GLenum format);
	void DeleteTexture();

	void BindTexture(int iTextureUnit = 0) const;
	void UnBindTexture(int iTextureUnit = 0) const;
	void SetWrap(int type = 0);
	void SetFiltering(int a_tfMagnification, int a_tfMinification);
	void SetSamplerParameter(GLenum parameter, GLenum value);
	int GetMinificationFilter();
	int GetMagnificationFilter();
	int GetWidth();
	int GetHeight();
	int GetBPP();

	UINT GetTextureID();
	void test();
protected:
	void CreateFromData(BYTE* bData, GLenum target, int a_iWidth, int a_iHeight, int a_iBPP, GLenum format, bool bGenerateMipMaps = false);
	bool LoadTexture(string a_sPath, GLenum target, bool bGenerateMipMaps = false);
	bool GenerateTexture();
private:
	int m_iWidth, m_iHeight, m_iBPP; // Texture width, height, and bytes per pixel
	UINT m_textureId; // Texture id
	UINT m_samplerId; // Sampler id
	bool m_bMipMapsGenerated;
	int m_tfMinification, m_tfMagnification;
	string m_sPath;
};

