#include "Interpol3D.h"
#include "InterpolNonFonct.h"

#ifdef WIN32
#include <windows.h>
#endif

Interpol3D::Interpol3D() {
	m_bClosed = false;
	m_bBuilt = false;
	m_pSpline1 = NULL;
	m_pSpline2 = NULL;
}


void Interpol3D::BuildSplines() {

	m_bBuilt = false;

	if(m_pSpline1) delete m_pSpline1;
	if(m_pSpline2) delete m_pSpline2;
	m_pSpline1 = NULL;
	m_pSpline2 = NULL;

	if(m_tCtrlPts.size() > 1)
	{
		std::vector<glm::vec2> tab1(m_tCtrlPts.size());
		std::vector<glm::vec2> tab2(m_tCtrlPts.size());

		for(unsigned int i=0;i<m_tCtrlPts.size();i++) {
			tab1[i].x = m_tCtrlPts[i].x;
			tab1[i].y = m_tCtrlPts[i].y;
	          
			tab2[i].x = m_tCtrlPts[i].x;
			tab2[i].y = m_tCtrlPts[i].z;      
		}

		m_pSpline1 = new CInterpolNonFonct(tab1);
		m_pSpline2 = new CInterpolNonFonct(tab2);


		tab1.clear();
		tab2.clear();

		m_bBuilt = true;
	}
	
}


glm::vec3 Interpol3D::getPoint(float t)
{
	if(m_bBuilt) {
		glm::vec2 pt1 = m_pSpline1->getpoint(t);
		glm::vec2 pt2 = m_pSpline2->getpoint(t);
		return glm::vec3(pt1.x, pt1.y, pt2.y);
	}
	else {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
}


Interpol3D::~Interpol3D() {
	if(m_pSpline1)
		delete m_pSpline1;
	if(m_pSpline2)
		delete m_pSpline2;
	m_bBuilt = false;
}

bool Interpol3D::AddPoint(const glm::vec3& pt)
{
	m_bBuilt = false;
	m_tCtrlPts.push_back(pt);
	return true;
}

bool Interpol3D::DeleteLastPoint()
{
	m_bBuilt = false;
	if(m_tCtrlPts.size() > 0)
		m_tCtrlPts.erase(m_tCtrlPts.end()-1);
	return true;
}

bool Interpol3D::Clear()
{
	m_bBuilt = false;
	m_tCtrlPts.clear();
	return true;
}

bool Interpol3D::Close()
{
	m_bBuilt = false;
	m_bClosed = true;

	if(m_tCtrlPts.size() > 1) {
		AddPoint( m_tCtrlPts[0] );
		return true;
	}

	m_bClosed = false;
	return false;
}
