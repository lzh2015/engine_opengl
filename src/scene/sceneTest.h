#ifndef SCENES_SCENETEST_H_
#define SCENES_SCENETEST_H_

#include "sceneBase.h"
#include "../textureCubemap.h"
#include "../terrain/terrain.h"
#include "../shader.h"
#include "../light.h"
#include "../objModel.h"


class SceneTest : public SceneBase
{
public:
	virtual bool Initialize();
	virtual void Render();
	virtual void PreRender();
	virtual void Release();
private:
	void RenderTerrain(bool bInverse);
	void RenderBuildings();
	void RenderTrees(bool bInverse, int depthMapIndex = 0);
	void RenderWater();
	void RenderGrass();
	TextureCubemap m_skybox;
	ObjModel	   m_house;
	ObjModel	   m_tree;
	Terrain 	   m_terrain;
	std::vector<Texture2D> m_textures;
	FrameBufferObject m_waterRelfection;
	VertexBufferObject m_vboWaterArea;
	VertexBufferObject m_vboWaterUV;
	float m_waterHeight;
	UINT m_vaoWaterArea;
};

#endif /* SCENES_SCENETEST_H_ */
