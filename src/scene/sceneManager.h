
#ifndef SCENES_SCENEMANAGER_H_
#define SCENES_SCENEMANAGER_H_

#include <map>
#include "sceneBase.h"
#include "sceneTest.h"
#include "sceneDemo.h"

class SceneManager
{
public:
	static SceneManager& GetInstance();
	bool Initialize();
	void PreRender();
	void Render();
	void Idle(float fElapsedTime);
	void Release();

	bool setCurrent(const std::string& name);
	SceneBase* getScenePointer(const std::string& name);
	inline SceneBase* getCurrentScenePointer()	const {return m_pCurrentScene;}


	void OnKeyboard(unsigned int key);
private:
	SceneManager();
	~SceneManager();
	SceneManager(SceneManager const&);  // Don't Implement
	void operator=(SceneManager const&); // Don't implement
	std::map<std::string, SceneBase*>	m_SceneDB; // Save all the scenes
	SceneBase* m_pCurrentScene; //Point to the current active scene
};


#endif
