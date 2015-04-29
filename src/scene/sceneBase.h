#ifndef SCENES_SCENEBASE_H_
#define SCENES_SCENEBASE_H_

#include "../spline/splineGL.h"

class SceneBase
{
public:
	virtual bool Initialize() = 0;
	virtual void Release();
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void Idle(float fElapsedTime) = 0;

	void InterpolCameraTraj(float fElapsedTime);
	bool LoadCameraTrajFromFile(const std::string& name);
	void OnKeyboard(unsigned int key);

	SceneBase();
	virtual ~SceneBase()  {}

	inline SplineGL*	getCamEyeSpline()	const	{return m_pCamEyeTraj;}
protected:
	SplineGL*		m_pCamEyeTraj;
	SplineGL*		m_pCamLookAtTraj;

private:
	//SceneBase(const SceneBase& sc) {}
	SceneBase &operator=(const SceneBase& sc) {return (*this);}
	bool SaveCameraTrajInFile(const std::string& name);
};

#endif /* SCENES_SCENEBASE_H_ */
