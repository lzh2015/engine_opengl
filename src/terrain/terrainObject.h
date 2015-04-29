
#ifndef TERRAINOBJECT_H_
#define TERRAINOBJECT_H_

#include "../global.h"
#include "../objModel.h"

class TerrainObject
{
public:
	void Draw(unsigned int id = 0);

	TerrainObject(ObjModel* mesh, glm::vec4 tr);
	~TerrainObject() {Delete();}
private:
	void Delete();
	glm::vec3					m_vPosition;
	float						m_fAngle;
	ObjModel*					m_tMesh;
};

#endif /* TERRAINOBJECT_H_ */
