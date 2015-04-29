#include "sceneBase.h"
#include "../viewCamera.h"
#include "../light.h"

SceneBase::SceneBase()
{
	m_pCamEyeTraj = new SplineGL();
	m_pCamLookAtTraj = new SplineGL();
}

void SceneBase::InterpolCameraTraj(float fElapsedTime)
{
	if(m_pCamEyeTraj->isValid() && m_pCamLookAtTraj->isValid())
	{
		ViewCamera& cam = ViewCamera::GetInstance();
		m_pCamEyeTraj->Idle(fElapsedTime);
		m_pCamLookAtTraj->Idle(fElapsedTime);
		cam.setEye(m_pCamEyeTraj->getPoint());
		cam.setView(m_pCamLookAtTraj->getPoint());

		LightResource::GetInstance().Update();
		//cout << glm::to_string(m_pCamEyeTraj->getPoint()) << endl;
	}
}

bool SceneBase::LoadCameraTrajFromFile(const std::string& name)
{
	m_pCamEyeTraj->Clear();
	m_pCamLookAtTraj->Clear();

	std::string filename = "./data/splines/" + name;
	std::ifstream fp(filename.c_str());
	if(!fp) return false;

	float speed = 0.01f;
	std::string str;
	std::string strBuffer;
	std::getline(fp, strBuffer);
	if(strBuffer.size() > 3)
	{
		std::stringstream strStream(strBuffer);
		float f;
		strStream >> str >> f;
		if(str == "SPEED")
			speed = f;
	}

	while(std::getline(fp, strBuffer))
	{
		if(strBuffer.size() > 3)
		{
			std::stringstream strStream(strBuffer);

			glm::vec3 point;
			strStream >> str >> point.x >> point.y >> point.z;

			if(str == "EYE")			m_pCamEyeTraj->AddPoint(point);
			else if(str == "LOOKAT")	m_pCamLookAtTraj->AddPoint(point);
		}
	}

	m_pCamEyeTraj->BuildSplines(false, speed);
	m_pCamLookAtTraj->BuildSplines(false, speed);

	fp.close();

	return true;
}

void SceneBase::OnKeyboard(unsigned int key)
{
	switch(key)
	{
		case E_KEY:
			m_pCamEyeTraj->AddPoint( ViewCamera::GetInstance().GetEyePosition() );
			m_pCamEyeTraj->BuildSplines(false);
			//SaveCameraTrajInFile("out.txt");
			m_pCamLookAtTraj->AddPoint(  ViewCamera::GetInstance().GetViewPosition() );
			m_pCamLookAtTraj->BuildSplines(false);
			SaveCameraTrajInFile("out.txt");
			return;

		case R_KEY:
			m_pCamEyeTraj->DeleteLastPoint();
			m_pCamEyeTraj->BuildSplines(false);
			m_pCamLookAtTraj->DeleteLastPoint();
			m_pCamLookAtTraj->BuildSplines(false);
			SaveCameraTrajInFile("out.txt");
			return;

//		case K_KEY:
//			m_pCamLookAtTraj->AddPoint(  ViewCamera::GetInstance().GetViewPosition() );
//			m_pCamLookAtTraj->BuildSplines(false);
//			SaveCameraTrajInFile("out.txt");
//			return;
//
//		case L_KEY:
//			m_pCamLookAtTraj->DeleteLastPoint();
//			m_pCamLookAtTraj->BuildSplines(false);
//			SaveCameraTrajInFile("out.txt");
//			return;
		}
}

bool SceneBase::SaveCameraTrajInFile(const std::string& name)
{
	std::string filename = "./data/splines/" + name;
	std::ofstream fp(filename.c_str());
	if(!fp) return false;

	fp << "SPEED " << m_pCamEyeTraj->getSpeed() << std::endl;

	for(int i=0; i<(int)m_pCamEyeTraj->getControlPoints().size(); i++) {
		fp << "EYE"	<< " " << m_pCamEyeTraj->getControlPoints()[i].x
					<< " " << m_pCamEyeTraj->getControlPoints()[i].y
					<< " " << m_pCamEyeTraj->getControlPoints()[i].z << std::endl;

		if(i<(int)m_pCamLookAtTraj->getControlPoints().size()) {
			fp << "LOOKAT"	<< " " << m_pCamLookAtTraj->getControlPoints()[i].x
							<< " " << m_pCamLookAtTraj->getControlPoints()[i].y
							<< " " << m_pCamLookAtTraj->getControlPoints()[i].z << std::endl;
		}

		fp << std::endl;
	}

	fp.close();

	return true;
}

void SceneBase::Release()
{
	if(m_pCamEyeTraj) delete m_pCamEyeTraj;
	m_pCamEyeTraj = NULL;
	if(m_pCamLookAtTraj) delete m_pCamLookAtTraj;
	m_pCamLookAtTraj = NULL;
}
