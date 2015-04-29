#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#pragma once
#include "global.h"

class BoundingBox {
public:
	BoundingBox(){}
	BoundingBox(const glm::vec3 & _min, const glm::vec3 & _max) {min=_min; max=_max;}
	void SetMin(float x, float y, float z);
	void SetMax(float x, float y, float z);
	const float & GetMinX() const;
	const float & GetMinY() const;
	const float & GetMinZ() const;
	const float & GetMaxX() const;
	const float & GetMaxY() const;
	const float & GetMaxZ() const;
	void Display() const;
	glm::vec3	GetCenter()	const;
	bool ContainsPoint(const glm::vec3 & point) const;

	glm::vec3 min;
	glm::vec3 max;
};


//#################################################################################

inline void BoundingBox::SetMin(float x, float y, float z)
{
	min = glm::vec3(x, y, z);
}

inline void BoundingBox::SetMax(float x, float y, float z)
{
	max = glm::vec3(x, y, z);
}

inline const float & BoundingBox::GetMinX() const
{
	return min.x;
}

inline const float & BoundingBox::GetMinY() const
{
	return min.y;
}

inline const float & BoundingBox::GetMinZ() const
{
	return min.z;
}

inline const float & BoundingBox::GetMaxX() const
{
	return max.x;
}

inline const float & BoundingBox::GetMaxY() const
{
	return max.y;
}

inline const float & BoundingBox::GetMaxZ() const
{
	return max.z;
}

inline void BoundingBox::Display() const
{
	cout << glm::to_string(min) << endl;
	cout << glm::to_string(max) << endl;
}

inline glm::vec3 BoundingBox::GetCenter() const
{
	glm::vec3 center = (max + min)/2.0f;
//	cout << "get center" << endl;
//	cout << glm::to_string(max) << " " << glm::to_string(min) << endl;
//	cout << glm::to_string(center) << endl;
//	cout << "###########" << endl;
	return center;
}

inline bool BoundingBox::ContainsPoint(const glm::vec3 & point) const
{
	return (point.x>=min.x && point.y>=min.y && point.z>=min.z && point.x<=max.x && point.y<=max.y && point.z<=max.z);
}


#endif /* BOUNDINGBOX_H_ */
