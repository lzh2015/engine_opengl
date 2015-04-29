#ifndef TERRAIN_H_
#define TERRAIN_H_

#pragma once

#include "../objModel.h"
#include "../vertexBufferObject.h"
#include "../boundingBox.h"
#include "quadtree.h"
#include "../texture2D.h"
#include "../shader.h"


class Terrain
{
public:
	Terrain();
	bool Load(const std::string& heightmap, const BoundingBox& bbox, int chunkSize);
	bool GenerateGrass(const std::string& map, int density);
	bool GenerateVegetation(const std::string& map, int density, float waterHeight);

	void ComputeBoundingBox();

	int  DrawGround(bool bReflection);
	int  DrawGrass(bool bReflection);
	int  DrawObjects(bool bReflection);

	glm::vec3  getPosition(float x_clampf, float z_clampf) const;
	glm::vec3  getNormal(float x_clampf, float z_clampf) const;
	glm::vec3  getTangent(float x_clampf, float z_clampf) const;

	void Delete();
private:
	int m_width;
	int m_height;
	int m_bpp;
	UINT m_idGroundVAO;
	UINT m_idGrassVAO;
	BoundingBox m_bbox;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normal;
	std::vector<glm::vec3> m_tangent;
	VertexBufferObject m_groundPosition;
	VertexBufferObject m_groundTexture;
	VertexBufferObject m_groundNormal;
	VertexBufferObject m_groundTangent;
	VertexBufferObject m_grassPosition;
	VertexBufferObject m_grassTexture;
	VertexBufferObject m_grassNormal;
	Quadtree* m_pQuadtree;
	ObjModel	   m_tree_1;
	ObjModel	   m_tree_2;
	ObjModel	   m_tree_3;
	ObjModel	   m_tree_4;
};

#endif /* TERRAIN_H_ */
