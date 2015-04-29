#include "interpolNonFonct.h"

CInterpolNonFonct::CInterpolNonFonct(const std::vector<glm::vec2>& controlpoints)
{
	std::vector<glm::vec2> ctrlptsa(controlpoints.size());
	std::vector<glm::vec2> ctrlptsb(controlpoints.size());


	for(int i=0; i<(int)controlpoints.size(); i++)
	{
		ctrlptsa[i] = glm::vec2((float)i, controlpoints[i].x);
		ctrlptsb[i] = glm::vec2((float)i, controlpoints[i].y);
	}

	Splineya = new CInterpolSpline(ctrlptsa);
	Splineyb = new CInterpolSpline(ctrlptsb);

	ctrlptsa.clear();
	ctrlptsb.clear();

}

CInterpolNonFonct::~CInterpolNonFonct()
{
	delete Splineya;
	delete Splineyb;

}

glm::vec2 CInterpolNonFonct::getpoint(float t)
{
	glm::vec2 pta = Splineya->getpoint(t);
	glm::vec2 ptb = Splineyb->getpoint(t);
	return glm::vec2(pta.y, ptb.y);
}
