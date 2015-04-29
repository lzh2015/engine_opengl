#include "sceneDemo.h"
#include "../sky.h"
#include <iostream>
using namespace std;
#include <glm/glm.hpp>
#include "../timer.h"

bool SceneDemo::Initialize()
{
	if(!m_house.Load("house.obj"))
	{
		return false;
	}

	//load sky resource
	if(!m_skybox.Load("left.bmp right.bmp top.bmp bottom.bmp front.bmp back.bmp"))
	//if(!m_skybox.Load("sand_up.jpg sand_dn.jpg sand_tp.jpg sand_bm.jpg sand_ft.jpg sand_bk.jpg"))
	{
		return false;
	}

	m_waterHeight = 110.0f;

	//load terrain resource
	BoundingBox bb;
	bb.SetMax(800, 400, 800);
	bb.SetMin(-800, 0, -800);

	if(!m_terrain.Load("terrain.png", bb, 40))
	{
		return false;
	}

	if(!m_terrain.GenerateGrass("mixmap_1.png", 100000))
	{
		return false;
	}

	if(!m_terrain.GenerateVegetation("mixmap_1.png", 500, m_waterHeight))
	{
		return false;
	}

	m_terrain.ComputeBoundingBox();

	demo_terrain_ground.UseProgram();
		demo_terrain_ground.SetUniform("bbox_min", bb.min);
		demo_terrain_ground.SetUniform("bbox_max", bb.max);
	demo_terrain_ground.UnUseProgram();

	//load textures
	std::cout << "Loading Texture Resources..." << std::endl;
	string texture_path;
	m_textures.resize(16);

	texture_path = "terrain_detail_NM.tga";
	if(!m_textures[0].Load(texture_path)) return false;
	texture_path = "mixmap_1.png";
	if(!m_textures[1].Load(texture_path)) return false;
	texture_path = "mixmap_2.png";
	if(!m_textures[2].Load(texture_path)) return false;
	texture_path = "snow.jpg";
	if(!m_textures[3].Load(texture_path)) return false;
	texture_path = "dry_grass.jpg";
	if(!m_textures[4].Load(texture_path)) return false;
	texture_path = "grass.jpg";
	if(!m_textures[5].Load(texture_path)) return false;
	texture_path = "cliff.jpg";
	if(!m_textures[6].Load(texture_path)) return false;
	texture_path = "cliff_steep.jpg";
	if(!m_textures[7].Load(texture_path)) return false;
	texture_path = "sandfloor009a.jpg";
	if(!m_textures[8].Load(texture_path)) return false;
	texture_path = "terrain_water_caustics.jpg";
	if(!m_textures[9].Load(texture_path)) return false;
	texture_path = "grass_billboards.tga";
	if(!m_textures[10].Load(texture_path)) return false;
	texture_path = "terrain_water_NM.jpg";
	if(!m_textures[11].Load(texture_path)) return false;
	texture_path = "tree_1.tga";
	if(!m_textures[12].Load(texture_path)) return false;
	texture_path = "tree_2.tga";
	if(!m_textures[13].Load(texture_path)) return false;
	texture_path = "tree_3.tga";
	if(!m_textures[14].Load(texture_path)) return false;
	texture_path = "tree_4.jpg";
	if(!m_textures[15].Load(texture_path)) return false;

	std::cout << "Loading Texture Resources OK" << std::endl;

	m_underWater.CreateFramebufferWithTexture(WINDOW_X, WINDOW_Y);
	m_underWater.AddDepthBuffer();
	m_underWater.SetFramebufferTextureFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);

	static float quad[] =
	{
	  -1, -1,
	   1, -1,
	   1,  1,
	   1,  1,
	  -1,  1,
	  -1, -1
	};

	static float quadUV[] =
	{
	   0, 0,
	   1, 0,
	   1, 1,
	   1, 1,
	   0, 1,
	   0, 0
	};

	glGenVertexArrays(1, &m_vaoUnderWater);
	glBindVertexArray(m_vaoUnderWater);

	m_vboUnderWater.CreateVBO();
	m_vboUnderWater.BindVBO();
	m_vboUnderWater.AddData(&quad, 12*sizeof(float));
	m_vboUnderWater.UploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	m_vboUnderWaterUV.CreateVBO();
	m_vboUnderWaterUV.BindVBO();
	m_vboUnderWaterUV.AddData(&quadUV, 12*sizeof(float));
	m_vboUnderWaterUV.UploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	m_volumetriclight_2nd_pass.CreateFramebufferWithTexture(WINDOW_X, WINDOW_Y);
	m_volumetriclight_2nd_pass.AddDepthBuffer();
	m_volumetriclight_2nd_pass.SetFramebufferTextureFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);

	glGenVertexArrays(1, &m_vaoVolumetricLight);
	glBindVertexArray(m_vaoVolumetricLight);

	m_vboVolumetricLight.CreateVBO();
	m_vboVolumetricLight.BindVBO();
	m_vboVolumetricLight.AddData(&quad, 12*sizeof(float));
	m_vboVolumetricLight.UploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	m_vboVolumetricLightUV.CreateVBO();
	m_vboVolumetricLightUV.BindVBO();
	m_vboVolumetricLightUV.AddData(&quadUV, 12*sizeof(float));
	m_vboVolumetricLightUV.UploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	m_waterRelfection.CreateFramebufferWithTexture(512, 512);
	m_waterRelfection.AddDepthBuffer();
	m_waterRelfection.SetFramebufferTextureFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);

	float d = 10.0f * SCREEN_DEPTH;

	static float points[] =
	{
	  -d,  m_waterHeight, -d,
	  -d,  m_waterHeight,  d,
	   d,  m_waterHeight,  d,
	   d,  m_waterHeight,  d,
	   d,  m_waterHeight, -d,
	  -d,  m_waterHeight, -d
	};

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

	glBindVertexArray(0);

	m_volumetriclight_1st_pass.CreateFramebufferWithTexture(512, 512);
	m_volumetriclight_1st_pass.AddDepthBuffer();
	m_volumetriclight_1st_pass.SetFramebufferTextureFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);

	return true;
}

void SceneDemo::PreRender()
{
	VOLUMETRIC_LIGHT = false;
	const Light & light = LightResource::GetInstance().GetLight(0);

	for(int i=0; i<CSM_FRUSTUMS; i++)
	{
		light.BindFrameBuffer(i);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RenderTrees(false, i+1);
			RenderBuildings(false, i+1);
		light.UnBindFrameBuffer(i);
	}

	m_waterRelfection.BindFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		bool reflection = true;
		if (ViewCamera::GetInstance().GetEyePosition().y < m_waterHeight) reflection = false;

		Sky::GetInstance().RenderSky(m_skybox, reflection);
		RenderTerrain(reflection);
		RenderTrees(reflection);
		RenderBuildings(reflection);
	m_waterRelfection.UnBindFramebuffer();

	VOLUMETRIC_LIGHT = true;
	m_volumetriclight_1st_pass.BindFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Sky::GetInstance().RenderSky(m_skybox, false);
		RenderTerrain(false);
		RenderTrees(false);
		RenderBuildings(false);
		//RenderGrass();
		//RenderWater();
	m_volumetriclight_1st_pass.UnBindFramebuffer();
}

void SceneDemo::Render()
{
	VOLUMETRIC_LIGHT = false;
	ViewCamera & cam = ViewCamera::GetInstance();
	const Light & light = LightResource::GetInstance().GetLight(0);

	if(cam.GetEyePosition().y < m_waterHeight)
	{
		m_underWater.BindFramebuffer();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Sky::GetInstance().RenderSky(m_skybox, false);
			RenderTerrain(false);
			//RenderGrass();
			RenderTrees(false);
			RenderBuildings(false);
			RenderWater();
		m_underWater.UnBindFramebuffer();

		RenderEffect(UNDERWATER);
	}
	else
	{
		glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0,
							 0.0, 0.5, 0.0, 0.0,
							 0.0, 0.0, 0.5, 0.0,
							 0.5, 0.5, 0.5, 1.0);

		glm::vec4 sun_position = glm::vec4((light.GetPosition() + cam.GetEyePosition()), 1.0);

		glm::mat4 mvp = cam.GetProjectionMatrix(90) * cam.LookAt();
		glm::vec4 screen_light = biasMatrix * mvp * sun_position;
		screen_light /= screen_light.w;

		//only for debug
//		sun_position = glm::vec4(619, 111, -452, 1.0);
//		glm::mat4 mvp = cam.GetProjectionMatrix() * cam.LookAt();
//		glm::vec4 screen_light = biasMatrix * mvp * sun_position;
//		screen_light /= screen_light.w;
//		cout << "45: " << glm::to_string(screen_light) << endl;
//
//		mvp = cam.GetProjectionMatrix(60) * cam.LookAt();
//		screen_light = biasMatrix * mvp * sun_position;
//		screen_light /= screen_light.w;
//		cout << "60: " << glm::to_string(screen_light) << endl;

//		float tan_fov = tan(FOV*0.5/180*PI);
//		float y = tan_fov * SCREEN_NEAR;
//		float x = y * SCREEN_ASPECT_RATIO;
//
//		glm::vec2 uv(1.0, 0.0);
//		float px = 2*x*uv.x - x;
//		float py = 2*y*uv.y - y;
//		sun_position = glm::vec4(px, py, -SCREEN_NEAR, 1.0);
//		mvp = cam.GetProjectionMatrix();
//		screen_light = biasMatrix * mvp * sun_position;
//		screen_light /= screen_light.w;
//		cout << "test 45:" << glm::to_string(screen_light) << endl;
//
//		mvp = cam.GetProjectionMatrix(60);
//		screen_light = biasMatrix * mvp * sun_position;
//		screen_light /= screen_light.w;
//		cout << "test 60" << glm::to_string(screen_light) << endl;

		if((screen_light.x >= 0) && (screen_light.x <= 1) && (screen_light.y >= 0) && (screen_light.y <= 1) && (screen_light.z <= 1))
		{
			//cout << glm::to_string(sun_screen) << endl;
			m_volumetriclight_2nd_pass.BindFramebuffer();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				Sky::GetInstance().RenderSky(m_skybox, false);
				RenderTerrain(false);
				RenderTrees(false);
				RenderGrass();
				RenderBuildings(false);
				RenderWater();
			m_volumetriclight_2nd_pass.UnBindFramebuffer();

			volumetric_light_scattering.UseProgram();
				volumetric_light_scattering.SetUniform("screenLightPos", screen_light);
				volumetric_light_scattering.SetUniform("biasMatrix", biasMatrix);
			volumetric_light_scattering.UnUseProgram();

			RenderEffect(VOLUMETRIC_LIGHT_SCATTERING);
		}
		else
		{
			Sky::GetInstance().RenderSky(m_skybox, false);
			RenderTerrain(false);
			RenderTrees(false);
			RenderGrass();
			RenderBuildings(false);
			RenderWater();
		}
	}

	VOLUMETRIC_LIGHT = true;

	//shadowmap debug
	//LightResource::GetInstance().Render();
}

void SceneDemo::RenderTerrain(bool bInverse)
{
	demo_terrain_ground.UseProgram();

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

			if(VOLUMETRIC_LIGHT) mvp = cam.GetProjectionMatrix(90) * cam.LookAt() * transform;
			else mvp = cam.GetProjectionMatrix() * cam.LookAt() * transform;
		}

		glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0,
							 0.0, 0.5, 0.0, 0.0,
							 0.0, 0.0, 0.5, 0.0,
							 0.5, 0.5, 0.5, 1.0);

		glm::mat4 depthBiasMVP_0 = biasMatrix * LightResource::GetInstance().GetLight(0).GetDepthMVP(0);
		glm::mat4 depthBiasMVP_1 = biasMatrix * LightResource::GetInstance().GetLight(0).GetDepthMVP(1);

		demo_terrain_ground.SetUniform("mvp", mvp);
		demo_terrain_ground.SetUniform("depthBiasMVP_0", depthBiasMVP_0);
		demo_terrain_ground.SetUniform("depthBiasMVP_1", depthBiasMVP_1);
		demo_terrain_ground.SetUniform("camView", cam.LookAt());
		demo_terrain_ground.SetUniform("transform", transform);
		demo_terrain_ground.SetUniform("lightDir", lightDir);
		demo_terrain_ground.SetUniform("cameraDir", cam.GetDirection());
		demo_terrain_ground.SetUniform("bEnableBumpMap", 1);
		demo_terrain_ground.SetUniform("time", Timer::GetInstance().GetTime());
		demo_terrain_ground.SetUniform("bWaterReflection", bInverse);
		demo_terrain_ground.SetUniform("water_height", m_waterHeight);
		demo_terrain_ground.SetUniform("sunLight.vColor", LightResource::GetInstance().GetLight(0).GetDiffuseColor());
		demo_terrain_ground.SetUniform("sunLight.vDirection", LightResource::GetInstance().GetLight(0).GetDirection());
		demo_terrain_ground.SetUniform("volumetric_light", VOLUMETRIC_LIGHT);
		demo_terrain_ground.SetUniform("csm_frumstum_0", CSM_FRUSTUM_0);
		demo_terrain_ground.SetUniform("csm_frumstum_1", CSM_FRUSTUM_1);

		for(int i=0; i < 10; i++)
		{
			m_textures[i].BindTexture(i);
		}

		LightResource::GetInstance().GetLight(0).BindFramebufferTexture(10,0);
		LightResource::GetInstance().GetLight(0).BindFramebufferTexture(11,1);

		demo_terrain_ground.SetUniform("normalMap", 0);
		demo_terrain_ground.SetUniform("mixmap_1", 1);
		demo_terrain_ground.SetUniform("mixmap_2", 2);
		demo_terrain_ground.SetUniform("snow", 3);
		demo_terrain_ground.SetUniform("dry_grass", 4);
		demo_terrain_ground.SetUniform("grass", 5);
		demo_terrain_ground.SetUniform("cliff", 6);
		demo_terrain_ground.SetUniform("cliff_steep", 7);
		demo_terrain_ground.SetUniform("sand", 8);
		demo_terrain_ground.SetUniform("texWaterCaustics", 9);
		demo_terrain_ground.SetUniform("gDepthMap_0", 10);
		demo_terrain_ground.SetUniform("gDepthMap_1", 11);


		m_terrain.DrawGround(bInverse);

	demo_terrain_ground.UnUseProgram();
}

void SceneDemo::RenderGrass()
{
    grass.UseProgram();
        ViewCamera & cam = ViewCamera::GetInstance();
        glm::vec3 lightDir = LightResource::GetInstance().GetLight(0).GetDirection();
        glm::mat4 transform = glm::scale(glm::mat4(1.0f),glm::vec3(1.0, 1.0, 1.0));

        glm::mat4 mvp;
        if(VOLUMETRIC_LIGHT) grass.SetUniform("mvp", cam.GetProjectionMatrix(90) * cam.LookAt() * transform);
        else grass.SetUniform("mvp", cam.GetProjectionMatrix() * cam.LookAt() * transform);

        m_textures[10].BindTexture(0);
        LightResource::GetInstance().GetLight(0).BindFramebufferTexture(1,0);
        LightResource::GetInstance().GetLight(0).BindFramebufferTexture(2,1);

        glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0,
                             0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.0,
                             0.5, 0.5, 0.5, 1.0);

        glm::mat4 depthBiasMVP_0 = biasMatrix * LightResource::GetInstance().GetLight(0).GetDepthMVP(0);
        glm::mat4 depthBiasMVP_1 = biasMatrix * LightResource::GetInstance().GetLight(0).GetDepthMVP(1);

        grass.SetUniform("depthBiasMVP_0", depthBiasMVP_0);
        grass.SetUniform("depthBiasMVP_1", depthBiasMVP_1);
        grass.SetUniform("camView", cam.LookAt());

        grass.SetUniform("gTexture", 0);
        grass.SetUniform("gDepthMap_0", 1);
        grass.SetUniform("gDepthMap_1", 2);
        grass.SetUniform("lightDir", lightDir);
        grass.SetUniform("time", Timer::GetInstance().GetTime());
        grass.SetUniform("lod_metric", TERRAIN_GRASS_MAX_DISTANCE);
        grass.SetUniform("volumetric_light", VOLUMETRIC_LIGHT);
        grass.SetUniform("csm_frumstum_0", CSM_FRUSTUM_0);
        grass.SetUniform("csm_frumstum_1", CSM_FRUSTUM_1);


        m_terrain.DrawGrass(false);

    grass.UnUseProgram();
}

void SceneDemo::RenderTrees(bool bInverse, int depthMapIndex)
{
	glDisable(GL_CULL_FACE);

	tree.UseProgram();
		ViewCamera & cam = ViewCamera::GetInstance();
		glm::mat4 inverse, depthMVP;

		if(bInverse)
		{
			tree.SetUniform("bInverse", 1);
			tree.SetUniform("vp", cam.GetProjectionMatrix() * cam.LookAt());
			inverse =  glm::translate(glm::vec3(0, m_waterHeight, 0)) * glm::scale(glm::mat4(1.0f),glm::vec3(1.0, -1.0, 1.0)) * glm::translate(glm::vec3(0, -m_waterHeight, 0));
			tree.SetUniform("inverse", inverse);
		}
		else if(depthMapIndex > 0)
		{
			tree.SetUniform("bInverse", 0);
			depthMVP = LightResource::GetInstance().GetLight(0).GetDepthMVP(depthMapIndex-1);
			tree.SetUniform("vp", depthMVP);
		}
		else
		{
			tree.SetUniform("bInverse", 0);
			if(VOLUMETRIC_LIGHT) tree.SetUniform("vp", cam.GetProjectionMatrix(90) * cam.LookAt());
			else tree.SetUniform("vp", cam.GetProjectionMatrix() * cam.LookAt());
		}


		tree.SetUniform("time", Timer::GetInstance().GetTime());
		tree.SetUniform("bWaterReflection", bInverse);
		tree.SetUniform("water_height", m_waterHeight);
		tree.SetUniform("lightDir", LightResource::GetInstance().GetLight(0).GetDirection());
		tree.SetUniform("lightColor", LightResource::GetInstance().GetLight(0).GetDiffuseColor());
		tree.SetUniform("volumetric_light", VOLUMETRIC_LIGHT);

		m_textures[12].BindTexture(0);
		m_textures[13].BindTexture(1);
		m_textures[14].BindTexture(2);
		m_textures[15].BindTexture(3);

		tree.SetUniform("gTexture_1", 0);
		tree.SetUniform("gTexture_2", 1);
		tree.SetUniform("gTexture_3", 2);
		tree.SetUniform("gTexture_4", 3);

		m_terrain.DrawObjects(false);

	tree.UnUseProgram();
}

void SceneDemo::RenderWater()
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	water.UseProgram();
		glBindVertexArray(m_vaoWaterArea);
		glm::vec3 eye = ViewCamera::GetInstance().GetEyePosition();
		//glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(eye.x, 0, eye.z));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::mat4 mvp = ViewCamera::GetInstance().GetProjectionMatrix()*ViewCamera::GetInstance().LookAt() * translate;

		water.SetUniform("mvp", mvp);
		water.SetUniform("translate", translate);

		water.SetUniform("win_x", WINDOW_X);
		water.SetUniform("win_y", WINDOW_Y);

		water.SetUniform("sunLight.vColor", LightResource::GetInstance().GetLight(0).GetDiffuseColor());
		water.SetUniform("sunLight.vDirection", LightResource::GetInstance().GetLight(0).GetDirection());

		water.SetUniform("sunLight.vPosition", LightResource::GetInstance().GetLight(0).GetPosition());

		water.SetUniform("eyePosition", eye);
		water.SetUniform("volumetric_light", VOLUMETRIC_LIGHT);

		water.SetUniform("time", Timer::GetInstance().GetTime());

		m_waterRelfection.BindFramebufferTexture();
		m_textures[11].BindTexture(1);

		water.SetUniform("gSampler", 0);
		water.SetUniform("texWaterNoiseNM", 1);
		glDrawArrays (GL_TRIANGLES, 0, 6);
	water.UnUseProgram();

	glDisable(GL_BLEND);
}

void SceneDemo::RenderEffect(int name)
{
	switch(name)
	{
		case UNDERWATER:
		{
			underwater.UseProgram();
				glBindVertexArray(m_vaoUnderWater);

				m_underWater.BindFramebufferTexture();
				m_textures[11].BindTexture(1);
				underwater.SetUniform("texScreen", 0);
				underwater.SetUniform("texWaterNoiseNM", 1);
				underwater.SetUniform("time", Timer::GetInstance().GetTime());

				glDrawArrays (GL_TRIANGLES, 0, 6);
			underwater.UnUseProgram();
			return;
		}

		case VOLUMETRIC_LIGHT_SCATTERING:
		{
			ViewCamera & cam = ViewCamera::GetInstance();

			volumetric_light_scattering.UseProgram();
				glBindVertexArray(m_vaoVolumetricLight);
				m_volumetriclight_1st_pass.BindFramebufferTexture(0);
				m_volumetriclight_2nd_pass.BindFramebufferTexture(1);

				float tan_fov = tan(FOV*0.5/180*PI);
				float y = tan_fov * SCREEN_NEAR;
				float x = y * SCREEN_ASPECT_RATIO;

				volumetric_light_scattering.SetUniform("x", x);
				volumetric_light_scattering.SetUniform("y", y);
				volumetric_light_scattering.SetUniform("camProjectMat", cam.GetProjectionMatrix(90));

				volumetric_light_scattering.SetUniform("firstPass", 0);
				volumetric_light_scattering.SetUniform("secondPass", 1);
				volumetric_light_scattering.SetUniform("density", 0.84f);
				volumetric_light_scattering.SetUniform("weight", 5.65f);
				volumetric_light_scattering.SetUniform("decay", 1.0f);
				volumetric_light_scattering.SetUniform("exposure", 0.0021f);

				glDrawArrays (GL_TRIANGLES, 0, 6);
			volumetric_light_scattering.UnUseProgram();
			return;
		}
	}

}

void SceneDemo::RenderBuildings(bool bInverse, int depthMapIndex)
{
	building.UseProgram();
		ViewCamera & cam = ViewCamera::GetInstance();
		glEnable(GL_CULL_FACE);

		if(bInverse)
		{
			building.SetUniform("bInverse", 1);
			building.SetUniform("vp", cam.GetProjectionMatrix() * cam.LookAt());
			glm::mat4 inverse =  glm::translate(glm::vec3(0, m_waterHeight, 0)) * glm::scale(glm::mat4(1.0f),glm::vec3(1.0, -1.0, 1.0)) * glm::translate(glm::vec3(0, -m_waterHeight, 0));
			building.SetUniform("inverse", inverse);
		}
		else if(depthMapIndex > 0)
		{
			building.SetUniform("bInverse", 0);
			glm::mat4 depthMVP = LightResource::GetInstance().GetLight(0).GetDepthMVP(depthMapIndex-1);
			building.SetUniform("vp", depthMVP);
		}
		else
		{
			building.SetUniform("bInverse", 0);
			if(VOLUMETRIC_LIGHT) building.SetUniform("vp", cam.GetProjectionMatrix(90) * cam.LookAt());
			else building.SetUniform("vp", cam.GetProjectionMatrix() * cam.LookAt());
		}

		glm::mat4 biasMatrix(0.5, 0.0, 0.0, 0.0,
							 0.0, 0.5, 0.0, 0.0,
							 0.0, 0.0, 0.5, 0.0,
							 0.5, 0.5, 0.5, 1.0);

		glm::mat4 depthBiasMVP_0 = biasMatrix * LightResource::GetInstance().GetLight(0).GetDepthMVP(0);
		glm::mat4 depthBiasMVP_1 = biasMatrix * LightResource::GetInstance().GetLight(0).GetDepthMVP(1);

		building.SetUniform("depthBiasMVP_0", depthBiasMVP_0);
		building.SetUniform("depthBiasMVP_1", depthBiasMVP_1);
		building.SetUniform("camView", cam.LookAt());

		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), 9.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		building.SetUniform("rotate", rotate);
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(619, 111, -452));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f),glm::vec3(5.0, 5.0, 5.0));
		glm::mat4 transform = translate * scale * rotate;

		building.SetUniform("transform", transform);
		building.SetUniform("lightDir", LightResource::GetInstance().GetLight(0).GetDirection());
		building.SetUniform("lightColor", LightResource::GetInstance().GetLight(0).GetDiffuseColor());
		building.SetUniform("volumetric_light", VOLUMETRIC_LIGHT);

		LightResource::GetInstance().GetLight(0).BindFramebufferTexture(0,0);
		LightResource::GetInstance().GetLight(0).BindFramebufferTexture(1,1);

		building.SetUniform("gDepthMap_0", 0);
		building.SetUniform("gDepthMap_1", 1);

		building.SetUniform("csm_frumstum_0", CSM_FRUSTUM_0);
		building.SetUniform("csm_frumstum_1", CSM_FRUSTUM_1);


		m_house.Draw();
		glDisable(GL_CULL_FACE);
	building.UnUseProgram();
}

void SceneDemo::Idle(float fElapsedTime)
{

}

void SceneDemo::Release()
{
	cout << "Releasing sceneDemo" << endl;

	m_skybox.DeleteTexture();
	m_house.ReleaseObj();
	m_terrain.Delete();

	int length = m_textures.size();
	for(int i=0; i<length; i++)
	{
		m_textures[i].DeleteTexture();
	}

	m_waterRelfection.DeleteFramebuffer();
	m_underWater.DeleteFramebuffer();
	m_volumetriclight_1st_pass.DeleteFramebuffer();
	m_volumetriclight_2nd_pass.DeleteFramebuffer();

	m_vboUnderWater.ReleaseVBO();
	m_vboUnderWaterUV.ReleaseVBO();
	m_vboWaterArea.ReleaseVBO();
	m_vboWaterUV.ReleaseVBO();
	m_vboVolumetricLight.ReleaseVBO();
	m_vboVolumetricLightUV.ReleaseVBO();
}

