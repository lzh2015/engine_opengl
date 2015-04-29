#include "splineGL.h"
#include "interpol3D.h"


#ifdef WIN32
#include <windows.h>
#endif

SplineGL::SplineGL() {
	m_pSpline = new Interpol3D();
	m_t = 0.0f;
	m_fSpeed = 0.01f;
	m_bFinished = false;
}

bool SplineGL::isValid() const
{
	return m_pSpline->isValid();
}

void SplineGL::BuildSplines(bool closed, float speed)
{
	if(closed)
		m_pSpline->Close();
	m_pSpline->BuildSplines();
	m_t = 0.0f;
	m_bFinished = false;
	m_fSpeed = speed;
}


glm::vec3 SplineGL::getPoint()
{
	return m_pSpline->getPoint(m_t);
}

const std::vector<glm::vec3>&	SplineGL::getControlPoints()	const
{
	return m_pSpline->getControlPoints();
}

void SplineGL::Destroy()
{
	if(m_pSpline)
		delete m_pSpline;
	m_pSpline = NULL;
}

bool SplineGL::AddPoint(const glm::vec3& pt)
{
	return m_pSpline->AddPoint(pt);
}

bool SplineGL::DeleteLastPoint()
{
	return m_pSpline->DeleteLastPoint();
}

bool SplineGL::Clear()
{
	return m_pSpline->Clear();
}


void SplineGL::Idle(float fElapsedTime)
{
	m_bFinished = false;
	m_t += m_fSpeed*fElapsedTime;

	if(m_t>1.0f) {
		m_t = 0.0f;
		m_bFinished = true;
	}
}

void SplineGL::DrawGL()
{
	//
}


