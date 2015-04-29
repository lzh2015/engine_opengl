/*
 * light.h
 *
 *  Created on: Feb 2, 2015
 *      Author: li-zh
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#pragma once

#include "frameBufferObject.h"
#include "vertexBufferObject.h"
#include "shader.h"
#include <glm/glm.hpp>
#include "viewCamera.h"

static const int LIGHT_STATE_ON = 1;
static const int LIGHT_STATE_OFF = 2;

static const int LIGHT_ATTR_DIRECTIONAL = 1;
static const int LIGHT_ATTR_POINT = 2;
static const int LIGHT_ATTR_SPOT = 3;


//################################################################################

class Light {
public:
	Light();
	void SetAttribute(int attribute);
	void SetState(int state);
	int GetState() const;
	void SetDiffuseColor(float r, float g, float b, float a);
	const glm::vec4 & GetDiffuseColor() const;
	void SetDirection(float x, float y, float z);
	const glm::vec3 & GetDirection() const;
	const glm::vec3 & GetPosition() const;
	bool InitialShadowBuffer();
	void BindFrameBuffer(int bufferIndex = 0) const;
	void UnBindFrameBuffer(int bufferIndex = 0) const;
	void BindFramebufferTexture(int iTextureUnit = 0, int bufferIndex = 0) const;
	glm::mat4 GetDepthMVP(int bufferIndex = 0) const;
	void RenderShadowMap();
	void UpdateBoundingBox(int bufferIndex = 0);
	void OnKeyboard(unsigned int input);
	void Delete();
private:
	int m_id;
	int m_state;
	int m_attr;
	glm::vec4 m_diffuse_color;
	glm::vec3 m_dir;
	glm::vec3 m_position;
	FrameBufferObject m_shadowBuffer[CSM_FRUSTUMS];
	UINT m_idVAO[CSM_FRUSTUMS];
	VertexBufferObject m_shadowMapQuad[CSM_FRUSTUMS];
	vector<BoundingBox> m_aabb;
	void BuildBoundingBox(int bufferIndex = 0);
	void Rotate(float degree);
	glm::mat4 LookAt() const;
};


class LightResource {
public:
	static LightResource& GetInstance();
	const Light & GetLight(int index) const;
	Light & GetLight(int index);
	void Render();
	bool Initialize();
	void Update();
	void RotateLight();
	void OnKeyboard(unsigned int input);
	void Delete();
private:
	LightResource();
	~LightResource();
	LightResource(LightResource const&);  // Don't Implement
	void operator=(LightResource const&); // Don't implement
	Light m_lightList[10];
	int m_length;
};



//#################################################################################

inline Light::Light():
	m_id(0),
	m_state(0),
	m_attr(0)
{
}

inline void Light::SetAttribute(int attribute)
{
	m_attr = attribute;
}

inline void Light::SetState(int state)
{
	m_state = state;
}

inline int Light::GetState() const
{
	return m_state;
}

inline void Light::SetDiffuseColor(float r, float g, float b, float a) {
	m_diffuse_color = glm::vec4(r, g, b, a);
}

inline const glm::vec4 & Light::GetDiffuseColor() const
{
	return m_diffuse_color;
}

inline void Light::SetDirection(float x, float y, float z)
{
	if(x == -0.0f) x = 0.0;
	if(y == -0.0f) y = 0.0;
	if(z == -0.0f) z = 0.0;

	m_dir = glm::normalize(glm::vec3(x, y, z));
	m_position = 1000.0f * m_dir;


	float diffuse_color_factor = glm::clamp(m_dir.y * 5.0, 0.0, 1.0);
	m_diffuse_color = glm::vec4(diffuse_color_factor, diffuse_color_factor, diffuse_color_factor, 1.0);

	//cout << glm::to_string(dir) << endl;
}

inline const glm::vec3 & Light::GetDirection() const
{
	return m_dir;
}

inline const glm::vec3 & Light::GetPosition() const
{
	return m_position;
}

inline bool Light::InitialShadowBuffer()
{
	for(int i=0; i<CSM_FRUSTUMS; i++)
	{
		if(m_shadowBuffer[i].CreateFrameBufferForDepthShadow())
		//if(shadowBuffer.CreateFramebufferWithTexture(1024, 1024))
		{
			//shadowBuffer.AddDepthBuffer();
			m_shadowBuffer[i].SetFramebufferTextureFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
			//shadowBuffer.SetFramebufferTextureFiltering(TEXTURE_FILTER_MAG_NEAREST, TEXTURE_FILTER_MIN_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glm::vec3 vShadowMapQuad[] =
			{
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 1.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f)
			};

			glm::vec2 vShadowMapQuadTC[] =
			{
				glm::vec2(0.0f, 1.0f),
				glm::vec2(0.0f, 0.0f),
				glm::vec2(1.0f, 1.0f),
				glm::vec2(1.0f, 0.0f)
			};

			m_shadowMapQuad[i].CreateVBO();
			FOR(j, 4)
			{
				m_shadowMapQuad[i].AddData(&vShadowMapQuad[j], sizeof(glm::vec3));
				m_shadowMapQuad[i].AddData(&vShadowMapQuadTC[j], sizeof(glm::vec2));
			}

			glGenVertexArrays(1, &m_idVAO[i]);
			glBindVertexArray(m_idVAO[i]);

			m_shadowMapQuad[i].BindVBO();
			m_shadowMapQuad[i].UploadDataToGPU(GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), 0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));

			BuildBoundingBox(i);
		}
		else
		{
			return false;
		}
	}

	return true;
}

inline void Light::BindFrameBuffer(int bufferIndex) const
{
	m_shadowBuffer[bufferIndex].BindFramebufferShadowMap();
}

inline void Light::UnBindFrameBuffer(int bufferIndex) const
{
	m_shadowBuffer[bufferIndex].UnBindFramebuffer();
}

inline void Light::BindFramebufferTexture(int iTextureUnit, int bufferIndex) const
{
	m_shadowBuffer[bufferIndex].BindFramebufferTexture(iTextureUnit);
}

inline glm::mat4 Light::GetDepthMVP(int bufferIndex) const
{
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(m_aabb[bufferIndex].GetMinX(), m_aabb[bufferIndex].GetMaxX(), m_aabb[bufferIndex].GetMinY(), m_aabb[bufferIndex].GetMaxY(), -m_aabb[bufferIndex].GetMaxZ(), -m_aabb[bufferIndex].GetMinZ()+50);
	glm::mat4 depthViewMatrix = LookAt();
	glm::mat4 depthModelMatrix = glm::mat4(1.0);

	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	//glm::mat4 depthMVP = crop * depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

//	glm::vec4 temp(-5, -1, -25, 1);
//	glm::vec4 test = crop * depthProjectionMatrix * temp;
//	cout << glm::to_string(depthMVP) << endl;

	return depthMVP;
}

inline void Light::RenderShadowMap()
{
	displayShadowMap.UseProgram();
		glBindVertexArray(m_idVAO[0]);

		m_shadowBuffer[0].BindFramebufferTexture();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	displayShadowMap.UnUseProgram();
}

inline void Light::BuildBoundingBox(int bufferIndex)
{
	ViewCamera & camera = ViewCamera::GetInstance();
	glm::mat4 camera_to_light = LookAt() * glm::inverse(camera.LookAt());

	BoundingBox newAABB;

	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	float x, y, z;

	maxZ = 0;

	int vertex_length = camera.GetFrustum(bufferIndex+1).GetVertexLength();

	for (int i = 0; i < vertex_length; ++i)
	{
		glm::vec4 newP = camera_to_light * glm::vec4(camera.GetFrustum(bufferIndex+1).GetVertex(i), 1);
		x = newP.x;
		y = newP.y;
		z = newP.z;

		if (i == 0) {
			minX = x;
			maxX = x;
			minY = y;
			maxY = y;
			minZ = z;
		}
		else
		{
			if (x > maxX) {
				maxX = x;
			}

			if (x < minX) {
				minX = x;
			}

			if (y > maxY) {
				maxY = y;
			}

			if (y < minY) {
				minY = y;
			}

			if (z < minZ) {
				minZ = z;
			}
		}
	}

	newAABB.SetMax(maxX, maxY, maxZ);
	newAABB.SetMin(minX, minY, minZ);

	m_aabb.reserve(1);
	m_aabb.push_back(newAABB);

	//aabb[bufferIndex].Display();
}

inline void Light::UpdateBoundingBox(int bufferIndex)
{
	ViewCamera & camera = ViewCamera::GetInstance();
	glm::mat4 camera_to_light = LookAt() * glm::inverse(camera.LookAt());

	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	float x, y, z;

	maxZ = 0;

	int vertex_length = camera.GetFrustum(bufferIndex+1).GetVertexLength();

	for (int i = 0; i < vertex_length; ++i)
	{
		glm::vec4 newP = camera_to_light * glm::vec4(camera.GetFrustum(bufferIndex+1).GetVertex(i), 1);
		x = newP.x;
		y = newP.y;
		z = newP.z;

		if (i == 0) {
			minX = x;
			maxX = x;
			minY = y;
			maxY = y;
			minZ = z;
		}
		else
		{
			if (x > maxX) {
				maxX = x;
			}

			if (x < minX) {
				minX = x;
			}

			if (y > maxY) {
				maxY = y;
			}

			if (y < minY) {
				minY = y;
			}

			if (z < minZ) {
				minZ = z;
			}
		}
	}

	m_aabb[bufferIndex].SetMax(maxX, maxY, maxZ);
	m_aabb[bufferIndex].SetMin(minX, minY, minZ);

	//aabb[0].Display();
}

inline glm::mat4 Light::LookAt() const
{
	if(m_dir == glm::vec3(0,1,0))
	{
		return glm::lookAt(m_position, glm::vec3(0,0,0), glm::vec3(0,0,-1));
	}
	else if(m_dir == glm::vec3(0,-1,0))
	{
		return glm::lookAt(m_position, glm::vec3(0,0,0), glm::vec3(0,0,1));
	}
	else {
		return glm::lookAt(m_position, glm::vec3(0,0,0), glm::vec3(0,1,0));
	}

}

inline void Light::OnKeyboard(unsigned int input)
{
	switch (input)
	{
		case F1_KEY:
			Rotate(2.0f);
			break;
		case F2_KEY:
			Rotate(-2.0f);
			break;
	}
}

inline void Light::Rotate(float degree)
{
	degree = (degree / 180)*PI;
	glm::mat4 mRotation = glm::rotate(glm::mat4(1.0f), degree, glm::vec3(-1.0f, 0.0f, -1.0f));
	glm::vec3 newDir = glm::vec3(mRotation*glm::vec4(m_dir, 1.0f));
	SetDirection(newDir.x, newDir.y, newDir.z);
}

inline void Light::Delete()
{
	for(int i= 0; i<CSM_FRUSTUMS; i++)
	{
		m_shadowBuffer[i].DeleteFramebuffer();
	}

	for(int i= 0; i<CSM_FRUSTUMS; i++)
	{
		m_shadowMapQuad[i].ReleaseVBO();
	}

	m_aabb.clear();
}

//####################################################################################################################

inline LightResource::LightResource():
		m_length(0)
{
}

inline LightResource::~LightResource()
{
}

inline const Light & LightResource::GetLight(int index) const
{
	return m_lightList[index];
}

inline Light & LightResource::GetLight(int index)
{
	return m_lightList[index];
}

inline bool LightResource::Initialize()
{
	m_lightList[0].SetAttribute(LIGHT_ATTR_DIRECTIONAL);
	m_lightList[0].SetState(LIGHT_STATE_ON);
	m_lightList[0].SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightList[0].SetDirection(0, 1, 0);
	if(!m_lightList[0].InitialShadowBuffer()) return false;

	++m_length;

	return true;
}

inline void LightResource::Render()
{
	m_lightList[0].RenderShadowMap();
}

inline void LightResource::Update()
{
	for(int i=0; i<CSM_FRUSTUMS; i++)
	{
		m_lightList[0].UpdateBoundingBox(i);
	}
}

inline void LightResource::OnKeyboard(unsigned int input)
{
	m_lightList[0].OnKeyboard(input);
	Update();
}

inline void LightResource::Delete()
{
	for(int i =0; i<m_length; i++)
	{
		m_lightList[i].Delete();
	}
}

#endif /* LIGHT_H_ */
