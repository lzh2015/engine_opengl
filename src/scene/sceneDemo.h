#ifndef SCENEDEMO_H_
#define SCENEDEMO_H_

#include "sceneBase.h"
#include "../textureCubemap.h"
#include "../terrain/terrain.h"
#include "../shader.h"
#include "../light.h"

static const int UNDERWATER = 1;
static const int VOLUMETRIC_LIGHT_SCATTERING = 2;

class SceneDemo : public SceneBase
{
public:
	virtual bool Initialize();
	virtual void Render();
	virtual void PreRender();
	virtual void Release();
	virtual void Idle(float fElapsedTime);
private:
	void RenderTerrain(bool bInverse);
	void RenderBuildings(bool bInverse, int depthMapIndex = 0);
	void RenderTrees(bool bInverse, int depthMapIndex = 0);
	void RenderWater();
	void RenderGrass();
	void RenderEffect(int name);
	TextureCubemap m_skybox;
	Terrain 	   m_terrain;
	ObjModel	   m_house;
	std::vector<Texture2D> m_textures;
	FrameBufferObject m_waterRelfection;
	FrameBufferObject m_underWater;
	FrameBufferObject m_volumetriclight_1st_pass;
	FrameBufferObject m_volumetriclight_2nd_pass;
	VertexBufferObject m_vboUnderWater;
	VertexBufferObject m_vboUnderWaterUV;
	VertexBufferObject m_vboWaterArea;
	VertexBufferObject m_vboWaterUV;
	VertexBufferObject m_vboVolumetricLight;
	VertexBufferObject m_vboVolumetricLightUV;
	float m_waterHeight;
	UINT m_vaoUnderWater;
	UINT m_vaoWaterArea;
	UINT m_vaoVolumetricLight;
};

#endif /* SCENEDEMO_H_ */
