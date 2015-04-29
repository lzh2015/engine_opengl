#include "terrainTrunk.h"

#define COORD(x,y)	((y)*(nHMTotalWidth)+(x))

void TerrainChunk::Build(unsigned int depth, glm::ivec2 pos, glm::ivec2 HMsize)
{
	for(unsigned int lod=0; lod<TERRAIN_CHUNKS_LOD; lod++)
		ComputeIndicesArray(lod, depth, pos, HMsize);
}

void TerrainChunk::ComputeIndicesArray(unsigned int lod, unsigned int depth, glm::ivec2 pos, glm::ivec2 HMsize)
{
	glm::ivec2 vHeightmapDataPos = pos;
	unsigned int offset = (unsigned int)pow(2.0f, (float)(lod));
	unsigned int div = (unsigned int)pow(2.0f, (float)(depth+lod));
	glm::ivec2 vHeightmapDataSize = glm::ivec2(HMsize.x/div, HMsize.y/div);
	vHeightmapDataSize += glm::ivec2(1,1);

	int nHMWidth = (int)vHeightmapDataSize.x;
	int nHMHeight = (int)vHeightmapDataSize.y;
	int nHMOffsetX = (int)vHeightmapDataPos.x;
	int nHMOffsetY = (int)vHeightmapDataPos.y;

	int nHMTotalWidth = (int)HMsize.x;
	//int nHMTotalHeight = (int)HMsize.y;

	m_tIndOffsetW[lod] = nHMWidth*2;
	m_tIndOffsetH[lod] = nHMWidth-1;
	int nIndice = (nHMWidth)*(nHMHeight-1)*2;
	m_tIndice[lod].reserve( nIndice );

	for(int j=0; j<(int)nHMHeight-1; j++)
	{
		for(int i=0; i<(int)nHMWidth; i++)
		{
			int id0 = COORD(i*(offset) + nHMOffsetX+0, j*(offset) + nHMOffsetY+0);
			int id1 = COORD(i*(offset) + nHMOffsetX+0, j*(offset) + nHMOffsetY+(offset));
			m_tIndice[lod].push_back( id0 );
			m_tIndice[lod].push_back( id1 );

		}
	}
}

#include "../shader.h"

int TerrainChunk::DrawGround(int lod)
{
	//if(lod>0) lod--;
	if(lod == 0)
	{
		demo_terrain_ground.SetUniform("color", glm::vec4(0,0,1,1));
	}
	else if (lod == 1)
	{
		demo_terrain_ground.SetUniform("color", glm::vec4(1,0,1,1));
	}
	else if (lod == 2)
	{
		demo_terrain_ground.SetUniform("color", glm::vec4(1,1,0,1));
	}

	for(GLuint j=0; j<(GLuint)m_tIndOffsetH[lod]; j++)
	{
		//glPolygonMode (GL_FRONT_AND_BACK, GL_LINE ) ;
		glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)m_tIndOffsetW[lod], GL_UNSIGNED_INT, &(m_tIndice[lod][j*m_tIndOffsetW[lod]]) );
	}
	return 1;
}

int  TerrainChunk::DrawGrass(int lod, float d)
{
	//if(lod != 0) return 0;
	if(d > TERRAIN_GRASS_MAX_DISTANCE) return 0;

	if(m_tGrassIndice.size() > 0)
	{
		float ratio = 1.0f - (d / TERRAIN_GRASS_MAX_DISTANCE);
		ratio *= 2.0f;
		if(ratio > 1.0f) ratio = 1.0f;

		GLsizei indices_count = (GLsizei)( ratio * m_tGrassIndice.size() );

		indices_count -= indices_count%6;

		if(indices_count > 0)
		{
			glDisable(GL_CULL_FACE);
			glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, &(m_tGrassIndice[0]) );
		}
	}

	return 1;
}

int TerrainChunk::DrawObjects(GLuint lod)
{
	for(int i=0; i<(int)m_tObject.size(); i++)
		m_tObject[i]->Draw(lod);

	return (int)m_tObject.size();
}


void TerrainChunk::AddObject(TerrainObject* obj)
{
	m_tObject.push_back(obj);
}

void TerrainChunk::Delete()
{
	for(int i=0; i<TERRAIN_CHUNKS_LOD; i++)
	{
		m_tIndice[i].clear();
	}

	m_tGrassIndice.clear();

	for(int i=0; i<(int)m_tObject.size(); i++)
	{
		delete m_tObject[i];
	}

}
