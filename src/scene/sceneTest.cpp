#include "sceneTest.h"
#include "../sky.h"
#include <iostream>
using namespace std;
#include <glm/glm.hpp>
#include "../timer.h"


bool SceneTest::Initialize()
{
	//load sky resource
	if(!m_skybox.Load("left.bmp right.bmp top.bmp bottom.bmp front.bmp back.bmp"))
	//if(!m_skybox.Load("sand_up.jpg sand_dn.jpg sand_tp.jpg sand_bm.jpg sand_ft.jpg sand_bk.jpg"))
	{
		return false;
	}

	//load obj
	if(!m_house.Load("house.obj"))
	{
		return false;
	}

	if(!m_tree.Load("tree.obj"))
	{
		return false;
	}

	//load terrain resource
	BoundingBox bb;
	bb.SetMax(300, 40, 300);
	bb.SetMin(-300, 0, -300);

	if(!m_terrain.Load("test_heightmap512_2.jpg", bb, 16))
	{
		return false;
	}

	if(!m_terrain.GenerateGrass("test_heightmap512_2_diffusemap.jpg", 100000))
	{
		return false;
	}

	m_terrain.ComputeBoundingBox();

	//load textures
	std::cout << "Loading Texture Resources..." << std::endl;
	string texture_path;
	m_textures.resize(9);

	texture_path = "terrain_detail_NM.tga";
	if(!m_textures[0].Load(texture_path)) return false;
	texture_path = "sandfloor009a.jpg";
	if(!m_textures[1].Load(texture_path)) return false;
	texture_path = "terrain_rocky_map_1024.png";
	if(!m_textures[2].Load(texture_path)) return false;
	texture_path = "terrain_grass_map_1024.png";
	if(!m_textures[3].Load(texture_path)) return false;
	texture_path = "test_heightmap512_2_diffusemap.jpg";
	if(!m_textures[4].Load(texture_path)) return false;
	texture_path = "images.jpg";
	if(!m_textures[5].Load(texture_path)) return false;
	texture_path = "palm_texture.tga";
	//	texture_path = "tree.jpg";
	if(!m_textures[6].Load(texture_path)) return false;
	texture_path = "terrain_water_NM.jpg";
	if(!m_textures[7].Load(texture_path)) return false;
	texture_path = "grass_billboards.tga";
	if(!m_textures[8].Load(texture_path)) return false;

	std::cout << "Loading Texture Resources OK" << std::endl;

	m_waterRelfection.CreateFramebufferWithTexture(512, 512);
	m_waterRelfection.AddDepthBuffer();
	m_waterRelfection.SetFramebufferTextureFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);

	m_waterHeight = 4.22f;

	float d = 2.0 * SCREEN_DEPTH;
	//float d = 100.0f;

	static float points[] =
	{
	  -d,  m_waterHeight, -d,
	  -d,  m_waterHeight,  d,
	   d,  m_waterHeight,  d,
	   d,  m_waterHeight,  d,
	   d,  m_waterHeight, -d,
	  -d,  m_waterHeight, -d
	};

//	static float textures[] =
//	{
//	  0, 0,
//	  0, 1,
//	  1, 1,
//	  1, 1,
//	  0, 1,
//	  0, 0
//	};

	glGenVertexArrays(1, &m_vaoWaterArea);
	glBindVertexArray(m_vaoWaterArea);

	m_vboWaterArea.CreateVBO();
	m_vboWaterArea.BindVBO();
	m_vboWaterArea.AddData(&points, 18*sizeof(float));
	m_vboWaterArea.UploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	water.UseProgram();
		water.SetUniform("bbox_min", bb.min);
		water.SetUniform("bbox_max", bb.max);
	water.UnUseProgram();

//	m_vboWaterUV.CreateVBO();
//	m_vboWaterUV.BindVBO();
//	m_vboWaterUV.AddData(&textures, 12*sizeof(float));
//	m_vboWaterUV.UploadDataToGPU(GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	return true;
}

void SceneTest::PreRender()
{
	const Light & light = LightResource::GetInstance().GetLight(0);

	for(int i=0; i<CSM_FRUSTUMS; i++)
	{
		light.BindFrameBuffer(i);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RenderTrees(false, i+1);
		light.UnBindFrameBuffer(i);
	}

	m_waterRelfection.BindFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Sky::GetInstance().RenderSky(m_skybox, true);
		RenderTerrain(true);
		RenderTrees(true);
	m_waterRelfection.UnBindFramebuffer();

}

void SceneTest::Render()
{
	Sky::GetInstance().RenderSky(m_skybox, false);
	//RenderBuildings();
	RenderTerrain(false);
	//RenderGrass();
	RenderTrees(false);
	RenderWater();

	//LightResource::GetInstance().Render();
}

void SceneTest::RenderBuildings()
{
	building.UseProgram();
		building.SetUniform("mvp", ViewCamera::GetInstance().GetProjectionMatrix()*ViewCamera::GetInstance().LookAt());
		m_textures[5].BindTexture();

		m_house.Render();

	building.UnUseProgram();
}

void SceneTest::RenderTerrain(bool bInverse)
{
	terrain_ground.UseProgram();
		ViewCamera & cam = ViewCamera::GetInstance();
		glm::mat4 transform, mvp;

		glm::vec3 lightDir = LightResource::GetInstance().GetLight(0).GetDirection();

		if(bInverse)
		{
			glDisable(GL_CULL_FACE);
			transform =  glm::translate(glm::vec3(0, m_waterHeight, 0)) * glm::scale(glm::mat4(1.0f),glm::vec3(1.0, -1.0, 1.0)) * glm::translate(glm::vec3(0, -m_waterHeight, 0));
			//transform =  glm::scale(glm::mat4(1.0f),glm::vec3(-1.0, 1.0, 1.0));
			//mvp = cam.GetProjectionMatrix() * cam.LookAt(true, m_waterHeight) * transform;
			mvp = cam.GetProjectionMatrix() * cam.LookAt() * transform;
		}
		else
		{
			//glDisable(GL_CULL_FACE);
			glEnable(GL_CULL_FACE);
			transform = glm::scale(glm::mat4(1.0f),glm::vec3(1.0, 1.0, 1.0));
			mvp = cam.GetProjectionMatrix() * cam.LookAt() * transform;
		}

		glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0,
							 0.0, 0.5, 0.0, 0.0,
							 0.0, 0.0, 0.5, 0.0,
							 0.5, 0.5, 0.5, 1.0);

		glm::mat4 depthBiasMVP_0 = biasMatrix * LightResource::GetInstance().GetLight(0).GetDepthMVP(0);
		glm::mat4 depthBiasMVP_1 = biasMatrix * LightResource::GetInstance().GetLight(0).GetDepthMVP(1);

		terrain_ground.SetUniform("mvp", mvp);
		terrain_ground.SetUniform("depthBiasMVP_0", depthBiasMVP_0);
		terrain_ground.SetUniform("depthBiasMVP_1", depthBiasMVP_1);
		terrain_ground.SetUniform("camView", cam.LookAt());
		terrain_ground.SetUniform("lightDir", lightDir);
		terrain_ground.SetUniform("sunLight.vColor", LightResource::GetInstance().GetLight(0).GetDiffuseColor());
		terrain_ground.SetUniform("sunLight.vDirection", LightResource::GetInstance().GetLight(0).GetDirection());
		terrain_ground.SetUniform("cameraDirection", ViewCamera::GetInstance().GetDirection());
		terrain_ground.SetUniform("lightPosition", LightResource::GetInstance().GetLight(0).GetPosition());
		terrain_ground.SetUniform("cameraPosition", ViewCamera::GetInstance().GetEyePosition());
		terrain_ground.SetUniform("bEnableBumpMap", 1);
		terrain_ground.SetUniform("water_height", m_waterHeight);
		terrain_ground.SetUniform("bWaterReflection", bInverse);
		terrain_ground.SetUniform("transform", transform);

		for(int i=0; i <(int)m_textures.size(); i++)
		{
			m_textures[i].BindTexture(i);
		}

		LightResource::GetInstance().GetLight(0).BindFramebufferTexture(5,0);
		LightResource::GetInstance().GetLight(0).BindFramebufferTexture(6,1);

		terrain_ground.SetUniform("gNormalMap", 0);
		terrain_ground.SetUniform("gDiffuse0", 1);
		terrain_ground.SetUniform("gDiffuse1", 2);
		terrain_ground.SetUniform("gDiffuse2", 3);
		terrain_ground.SetUniform("gDiffuseMap", 4);
		terrain_ground.SetUniform("gDepthMap_0", 5);
		terrain_ground.SetUniform("gDepthMap_1", 6);

		//m_terrain.DrawGround(false);
		m_terrain.DrawGround(bInverse);

	terrain_ground.UnUseProgram();
}

void SceneTest::RenderGrass()
{
	grass.UseProgram();
		ViewCamera & cam = ViewCamera::GetInstance();
		glm::vec3 lightDir = LightResource::GetInstance().GetLight(0).GetDirection();
		glm::mat4 transform = glm::scale(glm::mat4(1.0f),glm::vec3(1.0, 1.0, 1.0));
		glm::mat4 mvp = cam.GetProjectionMatrix() * cam.LookAt() * transform;

		m_textures[8].BindTexture();

		grass.SetUniform("mvp", mvp);
		grass.SetUniform("gTexture", 0);
		grass.SetUniform("lightDir", lightDir);
		grass.SetUniform("time", Timer::GetInstance().GetTime());
		grass.SetUniform("lod_metric", TERRAIN_GRASS_MAX_DISTANCE);

		m_terrain.DrawGrass(false);

	grass.UnUseProgram();
}

void SceneTest::RenderTrees(bool bInverse, int depthMapIndex)
{
	glDisable(GL_CULL_FACE);

	tree.UseProgram();
		ViewCamera & cam = ViewCamera::GetInstance();
		glm::mat4 transform, vp, inverse;

		glm::vec3 worldposition(200, 30, 0);
		float degree = 30.0f;
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), degree, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), worldposition);

		if(bInverse)
		{
			transform = translate * rotation;
			inverse =  glm::translate(glm::vec3(0, m_waterHeight, 0)) * glm::scale(glm::mat4(1.0f),glm::vec3(1.0, -1.0, 1.0)) * glm::translate(glm::vec3(0, -m_waterHeight, 0));
			vp = cam.GetProjectionMatrix() * cam.LookAt();

			tree.SetUniform("vp", vp);
			tree.SetUniform("bInverse", 1);
			tree.SetUniform("transform", transform);
			tree.SetUniform("inverse", inverse);
		}
		else if(depthMapIndex > 0)
		{
			transform = translate * rotation;
			vp = LightResource::GetInstance().GetLight(0).GetDepthMVP(depthMapIndex-1);
			tree.SetUniform("vp", vp);
			tree.SetUniform("bInverse", 0);
			tree.SetUniform("transform", transform);
		}
		else
		{
			transform = translate * rotation;
			vp = cam.GetProjectionMatrix() * cam.LookAt();
			tree.SetUniform("vp", vp);
			tree.SetUniform("bInverse", 0);
			tree.SetUniform("transform", transform);
		}

		glm::vec4 vertexM = translate * rotation * glm::vec4(0.0, 0.0, 0.0, 1.0);
		float move_speed = (float(int(vertexM.y*vertexM.z) % 50)/50.0 + 0.5)*2;
		float move_offset = vertexM.x;


		tree.SetUniform("rotate", rotation);
		tree.SetUniform("move_speed", move_speed);
		tree.SetUniform("move_offset", move_offset);
		tree.SetUniform("time", Timer::GetInstance().GetTime());
		tree.SetUniform("lightDir", LightResource::GetInstance().GetLight(0).GetDirection());
		tree.SetUniform("lightColor", LightResource::GetInstance().GetLight(0).GetDiffuseColor());

		m_textures[6].BindTexture();

		m_tree.Render();

	tree.UnUseProgram();
}

void SceneTest::RenderWater()
{
	water.UseProgram();
		//glm::vec3 eye =  ViewCamera::GetInstance().GetEyePosition();
		glBindVertexArray(m_vaoWaterArea);
		//glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(eye.x, 0, eye.z));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::mat4 mvp = ViewCamera::GetInstance().GetProjectionMatrix()*ViewCamera::GetInstance().LookAt() * translate;

		water.SetUniform("mvp", mvp);
		water.SetUniform("translate", translate);

		water.SetUniform("win_x", WINDOW_X);
		water.SetUniform("win_y", WINDOW_Y);

		water.SetUniform("sunLight.vColor", LightResource::GetInstance().GetLight(0).GetDiffuseColor());
		water.SetUniform("sunLight.vDirection", LightResource::GetInstance().GetLight(0).GetDirection());
		water.SetUniform("eyePosition", ViewCamera::GetInstance().GetEyePosition());

		water.SetUniform("time", Timer::GetInstance().GetTime());

		m_waterRelfection.BindFramebufferTexture();
		m_textures[7].BindTexture(1);

		water.SetUniform("gSampler", 0);
		water.SetUniform("texWaterNoiseNM", 1);

		glDrawArrays (GL_TRIANGLES, 0, 6);
	water.UnUseProgram();
}


void SceneTest::Release()
{
	cout << "Releasing sceneTest" << endl;


	m_skybox.DeleteTexture();
	m_house.ReleaseObj();
	m_tree.ReleaseObj();
	m_terrain.Delete();

	int length = m_textures.size();
	for(int i=0; i<length; i++)
	{
		m_textures[i].DeleteTexture();
	}

	m_waterRelfection.DeleteFramebuffer();
	m_vboWaterArea.ReleaseVBO();
	m_vboWaterUV.ReleaseVBO();
}
