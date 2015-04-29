#ifndef TERRAINTRUNK_H_
#define TERRAINTRUNK_H_

#define TERRAIN_CHUNKS_LOD 3

#define TERRAIN_CHUNK_LOD0	150.0f
#define TERRAIN_CHUNK_LOD1	400.0f

#define TERRAIN_GRASS_MAX_DISTANCE	400.0f

#include "terrainObject.h"

class TerrainChunk
{
public:
	TerrainChunk() {}
	~TerrainChunk() {Delete();}

	int  DrawGround(int lod);
	int  DrawGrass(int lod, float d);
	int  DrawObjects(GLuint lod);

	void Build(unsigned int depth, glm::ivec2 pos, glm::ivec2 HMsize);
	inline std::vector<int> & GetIndiceArray(unsigned int lod)	{return m_tIndice[lod];}
	inline std::vector<int> & GetGrassIndiceArray() {return m_tGrassIndice;}

	void AddObject(TerrainObject* obj);

	void Delete();
private:
	void ComputeIndicesArray(unsigned int lod, unsigned int depth, glm::ivec2 pos, glm::ivec2 HMsize);
	std::vector<int>				m_tIndice[TERRAIN_CHUNKS_LOD];
	int								m_tIndOffsetW[TERRAIN_CHUNKS_LOD];
	int								m_tIndOffsetH[TERRAIN_CHUNKS_LOD];

	std::vector<int>				m_tGrassIndice;
	std::vector<TerrainObject*>		m_tObject;
};


#endif /* TERRAINTRUNK_H_ */
