#ifndef FRAMEBUFFEROBJECT_H_
#define FRAMEBUFFEROBJECT_H_

#include "global.h"
#include "texture2D.h"

class FrameBufferObject
{
public:
	FrameBufferObject();
	bool CreateFrameBufferForDepthShadow();
	bool CreateFramebufferWithTexture(int a_iWidth, int a_m_iHeight);
	bool AddDepthBuffer();
	void BindFramebuffer(bool bSetFullViewport = true);
	void BindFramebufferShadowMap() const;
	void UnBindFramebuffer() const;
	void SetFramebufferTextureFiltering(int a_tfMagnification, int a_tfMinification);
	void BindFramebufferTexture(int iTextureUnit = 0, bool bRegenMipMaps = false) const;

	glm::mat4 CalculateProjectionMatrix(float fFOV, float fNear, float fFar);
	glm::mat4 CalculateOrthoMatrix();

	void DeleteFramebuffer();

	int GetWidth();
	int GetHeight();
private:
	int m_iWidth, m_iHeight;
	UINT m_uiFramebuffer;
	UINT m_uiDepthRenderbuffer;
	Texture2D m_framebufferTex;
};



#endif
