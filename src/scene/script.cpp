#include "sceneManager.h"

static int current = -1;

void ScriptIdle(float fElapsedTime)
{
	SceneManager& manag = SceneManager::GetInstance();
	//SceneBase* pScene = manag.getCurrentScenePointer();

	ViewCamera & cam = ViewCamera::GetInstance();

	static std::string tSceneName[3] = {"demo"};

	switch(current)
	{
		case -1:
		{
			manag.setCurrent(tSceneName[0]);
			SceneDemo* pDemo = (SceneDemo*)manag.getCurrentScenePointer();
			pDemo->LoadCameraTrajFromFile("terrain.txt");
			//pDemo->setSunAngle(vec2(0.0f, RADIANS(45.0f)));
			current = 0;
			return;
		}
		case 0:
		{
			manag.setCurrent(tSceneName[0]);
			glm::vec3 eye = cam.GetEyePosition();
			if(((int)eye.x == 625) && ((int)eye.y == 115) && ((int)eye.z == -452))
			{
				LightResource::GetInstance().GetLight(0).SetDirection(0.541676f, 0.642787f, -0.541675f);
			}

			if(((int)eye.x == 616) && ((int)eye.y == 118) && ((int)eye.z == -449))
			{
				LightResource::GetInstance().GetLight(0).SetDirection(0.705385f, 0.069756f, -0.705384f);
			}

//			if(pScene->getCamEyeSpline()->isFinished())
//			{
//				manag.setCurrent(tSceneName[1]);
//				SceneDemo* pDemo = (SceneDemo*)manag.getCurrentScenePointer();
//				pDemo->LoadCameraTrajFromFile("terrain_2.txt");
//				LightResource::GetInstance().GetLight(0).SetDirection(0.541676f, 0.642787f, -0.541675f);
//				current = 1;
//			}
			return;
		}
//		case 1:
//		{
//			manag.setCurrent(tSceneName[1]);
//			if(pScene->getCamEyeSpline()->isFinished())
//			{
//				manag.setCurrent(tSceneName[2]);
//				SceneDemo* pDemo = (SceneDemo*)manag.getCurrentScenePointer();
//				pDemo->LoadCameraTrajFromFile("terrain_3.txt");
//				LightResource::GetInstance().GetLight(0).SetDirection(0.705385f, 0.069756f, -0.705384f);
//				current = 2;
//			}
//			return;
//		}
//
//	case 2:
//	{
//		return;
//	}
//
//	case 3: {
//		manag.setCurrent(tSceneName[3]);
//		if(pScene->getCamEyeSpline()->isFinished()) {
//			manag.setCurrent(tSceneName[4]);
//			SceneTerrain* pTerrain = (SceneTerrain*)manag.getCurrentScenePointer();
//			pTerrain->LoadCameraTrajFromFile("terrain_3.txt");
//			pTerrain->setSunAngle(vec2(RADIANS(-5.0f), RADIANS(71.0f)));
//			current = 4;
//		}
//		break;}
//
	}
}

