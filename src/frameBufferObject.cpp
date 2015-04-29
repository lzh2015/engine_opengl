#include "frameBufferObject.h"

FrameBufferObject::FrameBufferObject():
	m_iWidth(0),
	m_iHeight(0),
	m_uiFramebuffer(0),
	m_uiDepthRenderbuffer(0)
{
}

bool FrameBufferObject::CreateFrameBufferForDepthShadow()
{
	if(m_uiFramebuffer != 0)return false;

	glGenFramebuffers(1, &m_uiFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);

//	m_framebufferTex.CreateEmptyTexture(1024, 1024, 0);
//	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);

	m_framebufferTex.CreateEmptyTexture(2048, 2048, 0);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);

	m_framebufferTex.SetFiltering(TEXTURE_FILTER_MAG_NEAREST, TEXTURE_FILTER_MIN_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_framebufferTex.GetTextureID(), 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.
	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	return true;
}


bool FrameBufferObject::CreateFramebufferWithTexture(int a_m_iWidth, int a_m_iHeight)
{
	if(m_uiFramebuffer != 0)return false;

	glGenFramebuffers(1, &m_uiFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);

	m_framebufferTex.CreateEmptyTexture(a_m_iWidth, a_m_iHeight, GL_RGB);
	//glTexImage2D(GL_TEXTURE_2D, 0,GL_RG32F, a_m_iWidth, a_m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, a_m_iWidth, a_m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebufferTex.GetTextureID(), 0);

	m_iWidth = a_m_iWidth;
	m_iHeight = a_m_iHeight;

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}


}

bool FrameBufferObject::AddDepthBuffer()
{
	if(m_uiFramebuffer == 0)return false;

	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);

	glGenRenderbuffers(1, &m_uiDepthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_iWidth, m_iHeight);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiDepthRenderbuffer);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}
}

void FrameBufferObject::BindFramebuffer(bool bSetFullViewport)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
	if(bSetFullViewport)glViewport(0, 0, m_iWidth, m_iHeight);
}

void FrameBufferObject::BindFramebufferShadowMap() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFramebuffer);
//	glViewport(0, 0, 1024, 1024);
	glViewport(0, 0, 2048, 2048);
}

void FrameBufferObject::UnBindFramebuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WINDOW_X, WINDOW_Y);
}

void FrameBufferObject::BindFramebufferTexture(int iTextureUnit, bool bRegenMipMaps) const
{
	m_framebufferTex.BindTexture(iTextureUnit);
	if(bRegenMipMaps)glGenerateMipmap(GL_TEXTURE_2D);
}

void FrameBufferObject::SetFramebufferTextureFiltering(int a_tfMagnification, int a_tfMinification)
{
	m_framebufferTex.SetFiltering(a_tfMagnification, a_tfMinification);
	m_framebufferTex.SetWrap(GL_CLAMP_TO_EDGE);
}

void FrameBufferObject::DeleteFramebuffer()
{
	if(m_uiFramebuffer)
	{
		glDeleteFramebuffers(1, &m_uiFramebuffer);
		m_uiFramebuffer = 0;
	}
	if(m_uiDepthRenderbuffer)
	{
		glDeleteRenderbuffers(1, &m_uiDepthRenderbuffer);
		m_uiDepthRenderbuffer = 0;
	}
	m_framebufferTex.DeleteTexture();
}

glm::mat4 FrameBufferObject::CalculateProjectionMatrix(float fFOV, float fNear, float fFar)
{
	return glm::perspective(fFOV, float(m_iWidth)/float(m_iHeight), fNear, fFar);
}

glm::mat4 FrameBufferObject::CalculateOrthoMatrix()
{
	return glm::ortho(0.0f, float(m_iWidth), 0.0f, float(m_iHeight));
}

int FrameBufferObject::GetWidth()
{
	return m_iWidth;
}

int FrameBufferObject::GetHeight()
{
	return m_iHeight;
}
