#include "terrainObject.h"
#include "../shader.h"

TerrainObject::TerrainObject(ObjModel* mesh, glm::vec4 tr)
{
	m_tMesh = mesh;
	m_vPosition = glm::vec3(tr);
	m_fAngle = tr.a;
}

void TerrainObject::Draw(unsigned int id)
{
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_fAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_vPosition);

	glm::vec4 vertexM = translate * scale * rotation * glm::vec4(0.0, 0.0, 0.0, 1.0);
	float move_speed = (float(int(vertexM.y*vertexM.z) % 50)/50.0 + 0.5)*2;
	float move_offset = vertexM.x;

	tree.SetUniform("transform", translate*scale*rotation);
	tree.SetUniform("rotate", rotation);
	tree.SetUniform("move_speed", move_speed);
	tree.SetUniform("move_offset", move_offset);

	if(m_tMesh->GetInstantName() == "m_tree_1")
	{
		tree.SetUniform("samplerID", 1);
	}
	else if(m_tMesh->GetInstantName() == "m_tree_2")
	{
		tree.SetUniform("samplerID", 2);
	}
	else if(m_tMesh->GetInstantName() == "m_tree_3")
	{
		tree.SetUniform("samplerID", 3);
	}
	else if(m_tMesh->GetInstantName() == "m_tree_4")
	{
		tree.SetUniform("samplerID", 4);
	}

	m_tMesh->Render();
}

void TerrainObject::Delete()
{
	m_tMesh->ReleaseObj();
	delete m_tMesh;
	m_tMesh = NULL;

}
