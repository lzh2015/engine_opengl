#include "quadtreeNode.h"

QuadtreeNode::QuadtreeNode():
	m_nLOD(0),
	m_fDistance(0),
	m_pChildren(NULL),
	m_pTerrainChunk(NULL)
{

}

void QuadtreeNode::Build(unsigned int depth,
						 glm::ivec2 pos,			// chunk position in height map
						 glm::ivec2 HMsize,
						 unsigned int minHMSize)
{
	m_nLOD = 0;

	unsigned int div = (unsigned int)pow(2.0f, (float)depth);
	glm::ivec2 nodesize = glm::ivec2(HMsize.x /div, HMsize.y/div);
	if((int)nodesize.x%2==0) nodesize.x++;
	if((int)nodesize.y%2==0) nodesize.y++;
	glm::ivec2 newsize = nodesize/2;

	//if can not be divided any more
	if(((unsigned int)newsize.x < minHMSize) ||
	   ((unsigned int)newsize.y < minHMSize))
	{
		// create a new chunk
		m_pTerrainChunk = new TerrainChunk();
		m_pTerrainChunk->Build(depth, pos, HMsize);

		// we dont need to create more child node
		//m_pChildren = NULL;
		return;
	}

	//create 4 new childs
	m_pChildren = new QuadtreeNode[4];

	// Calculate the bounding box of child node
	glm::vec3 center = m_bbox.GetCenter();
	m_pChildren[CHILD_NW].setBoundingBox( BoundingBox(m_bbox.min, center) );
	m_pChildren[CHILD_NE].setBoundingBox( BoundingBox(glm::vec3(center.x, 0.0f, m_bbox.min.z), glm::vec3(m_bbox.max.x, 0.0f, center.z)) );
	m_pChildren[CHILD_SW].setBoundingBox( BoundingBox(glm::vec3(m_bbox.min.x, 0.0f, center.z), glm::vec3(center.x, 0.0f, m_bbox.max.z)) );
	m_pChildren[CHILD_SE].setBoundingBox( BoundingBox(center, m_bbox.max) );

	// Calculate the position of child node
	glm::ivec2 newHMpos[4];
	newHMpos[CHILD_NW] = pos + glm::ivec2(0, 0);
	newHMpos[CHILD_NE] = pos + glm::ivec2(newsize.x, 0);
	newHMpos[CHILD_SW] = pos + glm::ivec2(0, newsize.y);
	newHMpos[CHILD_SE] = pos + glm::ivec2(newsize.x, newsize.y);


	// build child node in the same way
	for(int i=0; i<4; i++)
	{
		m_pChildren[i].Build(depth+1, newHMpos[i], HMsize, minHMSize);
	}

}

void QuadtreeNode::ComputeBoundingBox(const std::vector<glm::vec3> & vertices)
{
	if(m_pTerrainChunk) {
		m_bbox.min.y = 100000.0f;
		m_bbox.max.y = -100000.0f;
		std::vector<int>& tIndices = m_pTerrainChunk->GetIndiceArray(0);

		for(int i=0; i<(int)tIndices.size(); i++)
		{
			//cout << tIndices[i] << endl;
			glm::vec3 vertex = vertices[ tIndices[i] ];
			//glm::vec3 vertex = vertices[ 0 ];
			if(vertex.y > m_bbox.max.y)	m_bbox.max.y = vertex.y;
			if(vertex.y < m_bbox.min.y)	m_bbox.min.y = vertex.y;
		}
	}

	// we do the same for the child node
	if(m_pChildren) {
		for(int i=0; i<4; i++) {
			m_pChildren[i].ComputeBoundingBox(vertices);

			if(m_bbox.min.y > m_pChildren[i].m_bbox.min.y)	m_bbox.min.y = m_pChildren[i].m_bbox.min.y;
			if(m_bbox.max.y < m_pChildren[i].m_bbox.max.y)	m_bbox.max.y = m_pChildren[i].m_bbox.max.y;
		}
	}
}


int QuadtreeNode::DrawGround(int options)
{
	const ViewCamera & camera = ViewCamera::GetInstance();

	m_nLOD = -1;

	glm::vec3 center = m_bbox.GetCenter();

	float radius = glm::distance(m_bbox.max, center);

	//cout << glm::to_string(center) << " " << glm::to_string(m_bbox.max-center) << " "  << radius << endl;

	if(options & CHUNK_BIT_TESTCHILDREN)
	{
		// if camera not in the node
		if(!m_bbox.ContainsPoint(camera.GetEyePosition()))
		{
			int resSphereInFrustum = camera.ContainsSphere(center, radius);
			switch(resSphereInFrustum)
			{
				case FRUSTUM_OUT: return 0;
				case FRUSTUM_IN:
					options &= ~CHUNK_BIT_TESTCHILDREN;
					break;
				case FRUSTUM_INTERSECT:
				{
					int resBoxInFrustum = camera.ContainsBoundingBox(m_bbox);
					switch(resBoxInFrustum)
					{
						case FRUSTUM_IN: options &= ~CHUNK_BIT_TESTCHILDREN; break;
						case FRUSTUM_OUT: return 0;
					}
				}
			}
		}
	}

	m_nLOD = 0;

	if(!m_pChildren)
	{
		if(options & CHUNK_BIT_WATERREFLECTION)
		{
			return m_pTerrainChunk->DrawGround(TERRAIN_CHUNKS_LOD-1);
		}
		else
		{
			// Calculation of LOD level depending on the distance between the eye and the chunk
			//glm::vec3 vEyeToChunk = this->getBoundingBox().GetCenter() - camera.GetEyePosition();
			m_fDistance = glm::distance(this->getBoundingBox().GetCenter(), camera.GetEyePosition());
			int lod = 0;
			if(m_fDistance > TERRAIN_CHUNK_LOD1)		lod = 2;
			else if(m_fDistance > TERRAIN_CHUNK_LOD0)	lod = 1;
			m_nLOD = lod;
			return m_pTerrainChunk->DrawGround(lod);
		}
	}
	else
	{
		int ret = 0;
		for(int i=0; i<4; i++)
			ret += m_pChildren[i].DrawGround(options);
		return ret;
	}

}

int QuadtreeNode::DrawGrass(bool bReflection)
{
	if(!m_pChildren) {
		if( m_nLOD>=0 )
		{
			return m_pTerrainChunk->DrawGrass(m_nLOD, m_fDistance );
		}
		else
			return 0;
	}
	else {
		int ret = 0;
		if( m_nLOD>=0 )
			for(int i=0; i<4; i++)
				ret += m_pChildren[i].DrawGrass(bReflection);
		return ret;
	}
}

int QuadtreeNode::DrawObjects(bool bReflection)
{
	if(!m_pChildren) {
		if( m_nLOD>=0 )
			return m_pTerrainChunk->DrawObjects( bReflection? TERRAIN_CHUNKS_LOD-1 : (GLuint)m_nLOD);
		else
			return 0;
	}
	else {
		int ret = 0;
		if( m_nLOD>=0 )
			for(int i=0; i<4; i++)
				ret += m_pChildren[i].DrawObjects(bReflection);
		return ret;
	}
}

void QuadtreeNode::Delete()
{
	if(m_pChildren) {
		for(int i=0; i<4; i++)
			m_pChildren[i].Delete();
		delete [] m_pChildren;
		m_pChildren = NULL;
	}
}
