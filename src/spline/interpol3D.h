#ifndef INTERPOL3D_H
#define INTERPOL3D_H

#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include "../global.h"
#include <vector>
#include <iostream>

class CInterpolNonFonct;

class Interpol3D
{
public:
	const std::vector<glm::vec3>&	getControlPoints()	const	{return m_tCtrlPts;}
	bool						isClosed()			const	{return m_bClosed;}
	bool						isValid()			const	{return m_bBuilt && m_tCtrlPts.size() > 1;}

	Interpol3D();
	~Interpol3D();

	glm::vec3 getPoint(float t);

	bool AddPoint(const glm::vec3& pt);
	bool DeleteLastPoint();
	bool Clear();

	void BuildSplines();
	bool Close();

private:
	std::vector<glm::vec3>	m_tCtrlPts;
	bool				m_bClosed;
	bool				m_bBuilt;

	CInterpolNonFonct *		m_pSpline1;
	CInterpolNonFonct *		m_pSpline2;
};    

#endif

