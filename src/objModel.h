#ifndef OBJMODEL_H_
#define OBJMODEL_H

#pragma once

#include "global.h"
#include "texture.h"
#include "vertexBufferObject.h"
#include "shader.h"
#include "openglcontrol.h"
#include "viewCamera.h"
#include "light.h"
#include "material.h"

class ObjModel
{
public:
	ObjModel();
	~ObjModel();
	ObjModel(const string & name);
	bool Load(const string & objFile);
	void Render();
	void Draw();
	void ReleaseObj();

	const string & GetInstantName() const;
private:
	VertexBufferObject m_vboVertexData;
	VertexBufferObject m_vboTexCoordData;
	VertexBufferObject m_vboVertexNormalData;
	vector<unique_ptr<Material>> m_mtlList;
	std::vector<Texture2D> m_textures;
	int m_count;
	UINT m_idVAO;
	bool m_bLoaded;
	std::string m_name;
	bool LoadMtl(string & mtlFile);
	void Initial();
	std::vector<std::string> split(std::string str, std::string pattern);
};

#endif /* OBJMODEL_H_ */
