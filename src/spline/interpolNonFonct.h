#ifndef INTERPOLNONFONCT_H
#define INTERPOLNONFONCT_H


#include "interpolSpline.h"

#include <vector>
#include <iostream>

class CInterpolNonFonct
{
public:
	CInterpolNonFonct(const std::vector<glm::vec2>& controlpoints);
	virtual ~CInterpolNonFonct();

	CInterpolSpline * Splineya;
	CInterpolSpline * Splineyb;

	glm::vec2 getpoint(float t);
};

#endif
