#ifndef INTERPOL_H
#define INTERPOL_H

#ifdef WIN32
#include <windows.h>
#endif

#include "../global.h"
#include <vector>
#include <iostream>

class CInterpol 
{
protected:
	std::vector<glm::vec2> ctrlpoints;

public:
	CInterpol();
	CInterpol(const std::vector<glm::vec2>& controlpoints);
	virtual ~CInterpol();

	virtual glm::vec2 getpoint(float t)=0;

};
#endif 
