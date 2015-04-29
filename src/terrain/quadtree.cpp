#include "quadtree.h"


void Quadtree::Build(BoundingBox* pBBox,
					 glm::ivec2 HMsize,
					 unsigned int minHMSize)
{
	m_pRoot = new QuadtreeNode();
	m_pRoot->setBoundingBox(*pBBox);


	m_pRoot->Build(0, glm::ivec2(0,0), HMsize, minHMSize);

	// fonction récursive calcul de bounding box
//	m_pRoot->ComputeBoundingBox(vertices);

//	*pBBox = m_pRoot->getBoundingBox();
}

void Quadtree::ComputeBoundingBox(const std::vector<glm::vec3> & vertices)
{
	m_pRoot->ComputeBoundingBox(vertices);
}

int Quadtree::DrawGround(bool bReflection)
{
	int options = CHUNK_BIT_TESTCHILDREN;
	if(bReflection)
		options |= CHUNK_BIT_WATERREFLECTION;
	//cout << "start root" << endl;
	return m_pRoot->DrawGround(options);
}

int Quadtree::DrawGrass(bool bReflection)
{
	return m_pRoot->DrawGrass(bReflection);
}

int Quadtree::DrawObjects(bool bReflection)
{
	return m_pRoot->DrawObjects(bReflection);
}

QuadtreeNode* Quadtree::FindLeaf(glm::vec2& pos)
{
	QuadtreeNode* node = m_pRoot;

	while(!node->isALeaf()) {
		int i=0;
		for(i=0; i<4; i++) {
			QuadtreeNode* child = &(node->getChildren()[i]);
			if(child->getBoundingBox().ContainsPoint(glm::vec3(pos.x, child->getBoundingBox().GetCenter().y, pos.y) ))
			{
				node = child;
				break;
			}
		}

		if(i>=4) {
			return NULL;
		}
	}

	return node;
}


void Quadtree::Delete()
{
	if(m_pRoot) {
		m_pRoot->Delete();
		delete m_pRoot;
		m_pRoot = NULL;
	}
}

