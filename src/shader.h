#ifndef SHADER_H_
#define SHADER_H_

#pragma once

#include "global.h"

class Shader
{
public:
	bool LoadShader(string sFile, int a_iType);
	void DeleteShader();

	bool GetLinesFromFile(string sFile, bool bIncludePart, vector<string>* vResult);

	bool IsLoaded();
	UINT GetShaderID();

	Shader();

private:
	UINT m_uiShader; // ID of shader
	int m_iType; // GL_VERTEX_SHADER, GL_FRAGMENT_SHADER...
	bool m_bLoaded; // Whether shader was loaded and compiled
};

class ShaderProgram
{
public:
	void CreateProgram();
	void DeleteProgram();

	bool AddShaderToProgram(Shader* shShader);
	bool LinkProgram();

	void UseProgram();
	void UnUseProgram();

	UINT GetProgramID();

	// Setting vectors
	void SetUniform(string sName, glm::vec2* vVectors, int iCount = 1);
	void SetUniform(string sName, const glm::vec2 vVector);
	void SetUniform(string sName, glm::vec3* vVectors, int iCount = 1);
	void SetUniform(string sName, const glm::vec3 vVector);
	void SetUniform(string sName, glm::vec4* vVectors, int iCount = 1);
	void SetUniform(string sName, const glm::vec4 vVector);

	// Setting floats
	void SetUniform(string sName, float* fValues, int iCount = 1);
	void SetUniform(string sName, const float fValue);

	// Setting 3x3 matrices
	void SetUniform(string sName, glm::mat3* mMatrices, int iCount = 1);
	void SetUniform(string sName, const glm::mat3 mMatrix);

	// Setting 4x4 matrices
	void SetUniform(string sName, glm::mat4* mMatrices, int iCount = 1);
	void SetUniform(string sName, const glm::mat4 mMatrix);

	// Setting integers
	void SetUniform(string sName, int* iValues, int iCount = 1);
	void SetUniform(string sName, const int iValue);

	ShaderProgram();

private:
	UINT m_uiProgram; // ID of program
	bool m_bLinked; // Whether program was linked and is ready to use
};

class ShaderResource
{
public:
	static ShaderResource& GetInstance();
	bool Initialize();
	void Delete();
private:
	ShaderResource();
	~ShaderResource();
	ShaderResource(ShaderResource const&);  // Don't Implement
	void operator=(ShaderResource const&); // Don't implement
	vector<Shader> m_shaderList;
};

extern ShaderProgram basic;
extern ShaderProgram texture;
extern ShaderProgram phong;
extern ShaderProgram shadowmap;
extern ShaderProgram displayShadowMap;
extern ShaderProgram sky;
extern ShaderProgram terrain_ground;
extern ShaderProgram building;
extern ShaderProgram tree;
extern ShaderProgram water;
extern ShaderProgram demo_terrain_ground;
extern ShaderProgram grass;
extern ShaderProgram underwater;
extern ShaderProgram volumetric_light_scattering;

#endif
