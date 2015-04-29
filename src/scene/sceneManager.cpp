/*
 * sceneManager.cpp
 *
 *  Created on: 2015Äê2ÔÂ14ÈÕ
 *      Author: miumiu
 */

#include "sceneManager.h"
#include "script.h"

SceneManager& SceneManager::GetInstance() {
	static SceneManager instance; // Guaranteed to be destroyed.
									 // Instantiated on first use.
	return instance;
}

SceneManager::SceneManager()
{
	//m_SceneDB["test"] = new SceneTest();
	m_SceneDB["demo"] = new SceneDemo();
	setCurrent("demo");
}

SceneManager::~SceneManager()
{

}


bool SceneManager::Initialize()
{
	for(std::map<std::string, SceneBase*>::iterator it = m_SceneDB.begin(); it != m_SceneDB.end(); it++)
	{
		if(!(*it).second->Initialize())
		{
			return false;
		}
	}

	return true;
}

void SceneManager::PreRender()
{
	m_pCurrentScene->PreRender();
}

void SceneManager::Render()
{
	m_pCurrentScene->Render();
}

void SceneManager::Release()
{
	for(std::map<std::string, SceneBase*>::iterator it = m_SceneDB.begin(); it != m_SceneDB.end(); it++)
	{
		(*it).second->Release();
		delete (*it).second;
		(*it).second = NULL;
	}
}


bool SceneManager::setCurrent(const std::string& name)
{
	std::map<std::string, SceneBase*>::iterator it = m_SceneDB.find(name);

	if(it == m_SceneDB.end())
		return false;

	if(m_pCurrentScene != it->second)
	{
		m_pCurrentScene = it->second;
		//m_pCurrentScene->Reset();
	}
	return true;
}

void SceneManager::Idle(float fElapsedTime)
{
	if(ViewCamera::GetInstance().GetType() == ViewCamera::DRIVEN)
	{
		ScriptIdle(fElapsedTime);
		m_pCurrentScene->InterpolCameraTraj(fElapsedTime);
	}

	m_pCurrentScene->Idle(fElapsedTime);
}

void SceneManager::OnKeyboard(unsigned int key)
{
	m_pCurrentScene->OnKeyboard(key);
}
