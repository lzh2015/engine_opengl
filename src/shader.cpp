#include "shader.h"

ShaderProgram basic;
ShaderProgram texture;
ShaderProgram phong;
ShaderProgram shadowmap;
ShaderProgram displayShadowMap;
ShaderProgram sky;
ShaderProgram terrain_ground;
ShaderProgram building;
ShaderProgram tree;
ShaderProgram water;
ShaderProgram demo_terrain_ground;
ShaderProgram grass;
ShaderProgram underwater;
ShaderProgram volumetric_light_scattering;

Shader::Shader():
	m_uiShader(0),
	m_iType(0),
	m_bLoaded(false)
{
}

bool Shader::LoadShader(string sFile, int a_iType)
{
	vector<string> sLines;

	if(!GetLinesFromFile(sFile, false, &sLines))return false;

	const char** sProgram = new const char*[ESZ(sLines)];
	FOR(i, ESZ(sLines))sProgram[i] = sLines[i].c_str();
	
	m_uiShader = glCreateShader(a_iType);

	glShaderSource(m_uiShader, ESZ(sLines), sProgram, NULL);
	glCompileShader(m_uiShader);

	delete[] sProgram;

	int iCompilationStatus;
	glGetShaderiv(m_uiShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if(iCompilationStatus == GL_FALSE)
	{
		char sInfoLog[1024];
		char sFinalMessage[1536];
		int iLogLength;
		glGetShaderInfoLog(m_uiShader, 1024, &iLogLength, sInfoLog);
		sprintf(sFinalMessage, "Error! Shader file %s wasn't compiled! The compiler returned:\n\n%s", sFile.c_str(), sInfoLog);
		MessageBox(NULL, sFinalMessage, "Error", MB_ICONERROR);
		return false;
	}
	m_iType = a_iType;
	m_bLoaded = true;

	return true;
}

bool Shader::GetLinesFromFile(string sFile, bool bIncludePart, vector<string>* vResult)
{
	FILE* fp = fopen(sFile.c_str(), "rt");

	if(!fp)return false;

	string sDirectory;
	int slashIndex = -1;
	RFOR(i, ESZ(sFile)-1)
	{
		if(sFile[i] == '\\' || sFile[i] == '/')
		{
			slashIndex = i;
			break;
		}
	}

	sDirectory = sFile.substr(0, slashIndex+1);

	// Get all lines from a file

	char sLine[255];

	bool bInIncludePart = false;

	while(fgets(sLine, 255, fp))
	{
		stringstream ss(sLine);
		string sFirst;
		ss >> sFirst;
		if(sFirst == "#include")
		{
			string sFileName;
			ss >> sFileName;
			if(ESZ(sFileName) > 0 && sFileName[0] == '\"' && sFileName[ESZ(sFileName)-1] == '\"')
			{
				sFileName = sFileName.substr(1, ESZ(sFileName)-2);
				GetLinesFromFile(sDirectory+sFileName, true, vResult);
			}
		}
		else if(sFirst == "#include_part")
			bInIncludePart = true;
		else if(sFirst == "#definition_part")
			bInIncludePart = false;
		else if(!bIncludePart || (bIncludePart && bInIncludePart))
			vResult->push_back(sLine);
	}
	fclose(fp);

	return true;
}

bool Shader::IsLoaded()
{
	return m_bLoaded;
}

UINT Shader::GetShaderID()
{
	return m_uiShader;
}

void Shader::DeleteShader()
{
	if(!IsLoaded())return;
	m_bLoaded = false;
	glDeleteShader(m_uiShader);
}

ShaderProgram::ShaderProgram():
	m_uiProgram(0),
	m_bLinked(false)
{

}

void ShaderProgram::CreateProgram()
{
	m_uiProgram = glCreateProgram();
}

bool ShaderProgram::AddShaderToProgram(Shader* shShader)
{
	if(!shShader->IsLoaded())return false;

	glAttachShader(m_uiProgram, shShader->GetShaderID());

	return true;
}

bool ShaderProgram::LinkProgram()
{
	glLinkProgram(m_uiProgram);
	int iLinkStatus;
	glGetProgramiv(m_uiProgram, GL_LINK_STATUS, &iLinkStatus);
	m_bLinked = iLinkStatus == GL_TRUE;
	return m_bLinked;
}

void ShaderProgram::DeleteProgram()
{
	if(!m_bLinked)return;
	m_bLinked = false;
	glDeleteProgram(m_uiProgram);
}

void ShaderProgram::UseProgram()
{
	if(m_bLinked)glUseProgram(m_uiProgram);
}

void ShaderProgram::UnUseProgram()
{
	if(m_bLinked)glUseProgram(0);
}

UINT ShaderProgram::GetProgramID()
{
	return m_uiProgram;
}

// Setting floats

void ShaderProgram::SetUniform(string sName, float* fValues, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniform1fv(iLoc, iCount, fValues);
}

void ShaderProgram::SetUniform(string sName, const float fValue)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniform1fv(iLoc, 1, &fValue);
}

// Setting vectors

void ShaderProgram::SetUniform(string sName, glm::vec2* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniform2fv(iLoc, iCount, (GLfloat*)vVectors);
}

void ShaderProgram::SetUniform(string sName, const glm::vec2 vVector)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniform2fv(iLoc, 1, (GLfloat*)&vVector);
}

void ShaderProgram::SetUniform(string sName, glm::vec3* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniform3fv(iLoc, iCount, (GLfloat*)vVectors);
}

void ShaderProgram::SetUniform(string sName, const glm::vec3 vVector)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniform3fv(iLoc, 1, (GLfloat*)&vVector);
}

void ShaderProgram::SetUniform(string sName, glm::vec4* vVectors, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniform4fv(iLoc, iCount, (GLfloat*)vVectors);
}

void ShaderProgram::SetUniform(string sName, const glm::vec4 vVector)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniform4fv(iLoc, 1, (GLfloat*)&vVector);
}

// Setting 3x3 matrices

void ShaderProgram::SetUniform(string sName, glm::mat3* mMatrices, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, iCount, FALSE, (GLfloat*)mMatrices);
}

void ShaderProgram::SetUniform(string sName, const glm::mat3 mMatrix)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniformMatrix3fv(iLoc, 1, FALSE, (GLfloat*)&mMatrix);
}

// Setting 4x4 matrices

void ShaderProgram::SetUniform(string sName, glm::mat4* mMatrices, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, iCount, FALSE, (GLfloat*)mMatrices);
}

void ShaderProgram::SetUniform(string sName, const glm::mat4 mMatrix)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniformMatrix4fv(iLoc, 1, FALSE, (GLfloat*)&mMatrix);
}

// Setting integers

void ShaderProgram::SetUniform(string sName, int* iValues, int iCount)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniform1iv(iLoc, iCount, iValues);
}

void ShaderProgram::SetUniform(string sName, const int iValue)
{
	int iLoc = glGetUniformLocation(m_uiProgram, sName.c_str());
	glUniform1i(iLoc, iValue);
}

//#################################################################################################################


ShaderResource & ShaderResource::GetInstance()
{
	static ShaderResource instance; // Guaranteed to be destroyed.
									 // Instantiated on first use.
	return instance;
}

ShaderResource::ShaderResource():m_shaderList(0)
{

}

ShaderResource::~ShaderResource()
{

}

bool ShaderResource::Initialize()
{
	Shader newShader;

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\basic_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\basic_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	basic.CreateProgram();
	basic.AddShaderToProgram(&m_shaderList[0]);
	basic.AddShaderToProgram(&m_shaderList[1]);
	basic.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\texture_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\texture_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	texture.CreateProgram();
	texture.AddShaderToProgram(&m_shaderList[2]);
	texture.AddShaderToProgram(&m_shaderList[3]);
	texture.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\phong_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\phong_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	phong.CreateProgram();
	phong.AddShaderToProgram(&m_shaderList[4]);
	phong.AddShaderToProgram(&m_shaderList[5]);
	phong.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\shadowmap_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\shadowmap_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	shadowmap.CreateProgram();
	shadowmap.AddShaderToProgram(&m_shaderList[6]);
	shadowmap.AddShaderToProgram(&m_shaderList[7]);
	shadowmap.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\shadowdebug_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\shadowdebug_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	displayShadowMap.CreateProgram();
	displayShadowMap.AddShaderToProgram(&m_shaderList[8]);
	displayShadowMap.AddShaderToProgram(&m_shaderList[9]);
	displayShadowMap.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\sky_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\sky_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	sky.CreateProgram();
	sky.AddShaderToProgram(&m_shaderList[10]);
	sky.AddShaderToProgram(&m_shaderList[11]);
	sky.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\terrain_ground_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

//	if(!newShader.LoadShader("data\\shaders\\terrain_ground_geom.glsl", GL_GEOMETRY_SHADER)) return false;
//	m_shaderList.reserve(1);
//	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\terrain_ground_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	terrain_ground.CreateProgram();
	terrain_ground.AddShaderToProgram(&m_shaderList[12]);
	terrain_ground.AddShaderToProgram(&m_shaderList[13]);
//	terrain_ground.AddShaderToProgram(&m_shaderList[14]);
	terrain_ground.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\building_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\building_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	building.CreateProgram();
	building.AddShaderToProgram(&m_shaderList[14]);
	building.AddShaderToProgram(&m_shaderList[15]);
	building.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\tree_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\tree_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	tree.CreateProgram();
	tree.AddShaderToProgram(&m_shaderList[16]);
	tree.AddShaderToProgram(&m_shaderList[17]);
	tree.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\water_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\water_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	water.CreateProgram();
	water.AddShaderToProgram(&m_shaderList[18]);
	water.AddShaderToProgram(&m_shaderList[19]);
	water.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\demo_terrain_ground_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\demo_terrain_ground_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	demo_terrain_ground.CreateProgram();
	demo_terrain_ground.AddShaderToProgram(&m_shaderList[20]);
	demo_terrain_ground.AddShaderToProgram(&m_shaderList[21]);
	demo_terrain_ground.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\grass_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\grass_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	grass.CreateProgram();
	grass.AddShaderToProgram(&m_shaderList[22]);
	grass.AddShaderToProgram(&m_shaderList[23]);
	grass.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\underwater_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\underwater_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	underwater.CreateProgram();
	underwater.AddShaderToProgram(&m_shaderList[24]);
	underwater.AddShaderToProgram(&m_shaderList[25]);
	underwater.LinkProgram();

	//Shader newShader;
	if(!newShader.LoadShader("data\\shaders\\volumetric_light_scattering_vert.glsl", GL_VERTEX_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	if(!newShader.LoadShader("data\\shaders\\volumetric_light_scattering_frag.glsl", GL_FRAGMENT_SHADER)) return false;
	m_shaderList.reserve(1);
	m_shaderList.push_back(newShader);

	volumetric_light_scattering.CreateProgram();
	volumetric_light_scattering.AddShaderToProgram(&m_shaderList[26]);
	volumetric_light_scattering.AddShaderToProgram(&m_shaderList[27]);
	volumetric_light_scattering.LinkProgram();
	return true;
}

void ShaderResource::Delete()
{
	basic.DeleteProgram();
	texture.DeleteProgram();
	phong.DeleteProgram();
	shadowmap.DeleteProgram();
	displayShadowMap.DeleteProgram();
	sky.DeleteProgram();
	terrain_ground.DeleteProgram();
	building.DeleteProgram();
	tree.DeleteProgram();
	water.DeleteProgram();
	demo_terrain_ground.DeleteProgram();
	grass.DeleteProgram();
	underwater.DeleteProgram();
	volumetric_light_scattering.DeleteProgram();

	int length = m_shaderList.size();

	for (int i=0; i<length; ++i)
	{
		m_shaderList[i].DeleteShader();
	}
}
