#ifndef QUADTREENODE_H_
#define QUADTREENODE_H_

#define CHILD_NW	0
#define CHILD_NE	1
#define CHILD_SW	2
#define CHILD_SE	3

#define CHUNK_BIT_TESTCHILDREN		0x1
#define CHUNK_BIT_WATERREFLECTION	0x2

#include "terrainTrunk.h"
#include "../boundingBox.h"
#include "../viewCamera.h"

class QuadtreeNode {
public:
	QuadtreeNode();

	inline void setBoundingBox(const BoundingBox& bbox)	{m_bbox = bbox;}
	inline const BoundingBox&	getBoundingBox()		{return m_bbox;}
	void Build(unsigned int depth, glm::ivec2 pos, glm::ivec2 HMsize, unsigned int minHMSize);
	void ComputeBoundingBox(const std::vector<glm::vec3> & vertices);

	int  DrawGround(int options);
	int  DrawGrass(bool bReflection);
	int  DrawObjects(bool bReflection);

	inline bool isALeaf() const							{return m_pChildren==0;}
	inline QuadtreeNode*	getChildren()				{return m_pChildren;}
	inline TerrainChunk*	getChunk()					{return m_pTerrainChunk;}

	void Delete();
private:
	int				m_nLOD;
	float			m_fDistance;
	BoundingBox		m_bbox;
	QuadtreeNode*	m_pChildren;
	TerrainChunk*	m_pTerrainChunk;
};

#endif /* QUADTREENODE_H_ */
