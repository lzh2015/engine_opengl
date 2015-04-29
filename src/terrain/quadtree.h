
#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <iostream>
#include <string>
#include "quadtreeNode.h"
#include "../boundingBox.h"

class Quadtree {
public:
	Quadtree()	{m_pRoot = NULL;}
	//~Quadtree()	{}

	void Build(BoundingBox* pBBox, glm::ivec2 HMSize, unsigned int minHMSize);
	void ComputeBoundingBox(const std::vector<glm::vec3> & vertices);

	int  DrawGround(bool bReflection);
	int  DrawGrass(bool bReflection);
	int  DrawObjects(bool bReflection);

	QuadtreeNode*	FindLeaf(glm::vec2& pos);

	void Delete();
private:
	QuadtreeNode*	m_pRoot;	// root node of the quadtree
};

#endif /* QUADTREE_H_ */
