#include "objModel.h"

ObjModel::ObjModel():
	m_mtlList(0),
	m_textures(0),
	m_count(0),
	m_idVAO(0),
	m_bLoaded(false),
	m_name("")
{

}

ObjModel::ObjModel(const string & name):
	m_mtlList(0),
	m_textures(0),
	m_count(0),
	m_idVAO(0),
	m_bLoaded(false),
	m_name(name)
{

}

ObjModel::~ObjModel()
{

}


bool ObjModel::Load(const string & objFile)
{
	std::cout << "Loading Obj Resources:  " << objFile <<  std::endl;

	string fileName = "./data/mesh/" + objFile;
	ifstream filestream;
	filestream.open(fileName.c_str());

	string mtlFile;
	string line_stream;
	string type_str;
	stringstream str_stream;
	float u, v;
	float x, y, z;

	vector<glm::vec3> vertices;
	vector<glm::vec2> texCoords;
	vector<glm::vec3> normals;

	int mtlIndex = 0;
	int num_vertices = 0;

	if (!filestream.is_open())
	{
		cout << "can not open file: " << objFile << endl;
		return false;
	}
	else
	{
		while (!filestream.eof())
		{
			getline(filestream, line_stream);
			str_stream.clear();
			str_stream.str(line_stream);
			str_stream >> type_str;

			if (type_str == "mtllib")
			{
				mtlFile = objFile;
				str_stream >> type_str;
				int position = mtlFile.find_last_of("\\");
				int length = mtlFile.length();
				mtlFile.replace(position + 1, length, type_str);

				if (!LoadMtl(mtlFile))
				{
					return false;
				}
			}
			else if (type_str == "o")
			{

			}
			//vertex
			else if (type_str == "v")
			{
				str_stream >> x;
				str_stream >> y;
				str_stream >> z;

				glm::vec3 newVertex(x, y, z);

				vertices.reserve(1);
				vertices.push_back(newVertex);
				//cout << glm::to_string(newVertex) << endl;
			} else if (type_str == "vt")
			{
				str_stream >> u;
				str_stream >> v;

				glm::vec2 newUV(u, v);

				texCoords.reserve(1);
				texCoords.push_back(newUV);
				//cout << glm::to_string(newUV) << endl;
			}
			else if (type_str == "vn")
			{
				str_stream >> x;
				str_stream >> y;
				str_stream >> z;

				glm::vec3 newNormal(x, y, z);

				normals.reserve(1);
				normals.push_back(newNormal);
				//cout << glm::to_string(newNormal) << endl;
			}
			else if (type_str == "usemtl")
			{
				if(num_vertices != 0)
				{
					m_mtlList[mtlIndex]->SetVecticesNum(num_vertices);
					num_vertices = 0;
					++mtlIndex;
				}
			}
			else if (type_str == "f")
			{
				vector<string> result;

				for (int i=0; i<3; ++i)
				{
					++num_vertices;
					++m_count;

					str_stream >> type_str;
					result = split(type_str, "/");
					for (int j = 0; j < (int)result.size(); j++)
					{
						if (j == 0)
						{
							float index = atof(result[j].c_str()) - 1;
							m_vboVertexData.AddData(&vertices[index], sizeof(glm::vec3));
						}
						else if (j == 1)
						{
							if (!result[j].empty())
							{
								float index = atof(result[j].c_str()) - 1;
								m_vboTexCoordData.AddData(&texCoords[index], sizeof(glm::vec2));
							}
						}
						else if (j == 2)
						{
							float index = atof(result[j].c_str()) - 1;
							m_vboVertexNormalData.AddData(&normals[index], sizeof(glm::vec3));
						}
					}
				}
			}
		}

		if((num_vertices != 0) && (int)m_mtlList.size() > 0)
		{
			m_mtlList[mtlIndex]->SetVecticesNum(num_vertices);
		}

		filestream.close();
	}

	m_bLoaded = true;

	Initial();

	std::cout << "Loading Obj Resources OK" <<  std::endl;

	return true;
}

bool ObjModel::LoadMtl(string & mtlFile)
{
	string fileName = "./data/mesh/" + mtlFile;

	ifstream filestream;
	filestream.open(fileName.c_str());

	string line_stream;
	string type_str;
	stringstream str_stream;

	unique_ptr<Material> newMtl(new Material());

	std::vector<string> texList;

	bool readyToAddMtl = false;

	if (!filestream.is_open())
	{
		cout << "can not open file: " << mtlFile << endl;
		return false;
	}

	while (!filestream.eof())
	{
		getline(filestream, line_stream);
		str_stream.clear();
		str_stream.str(line_stream);
		str_stream >> type_str;

		if (type_str == "newmtl")
		{
			if(readyToAddMtl)
			{
				m_mtlList.reserve(1);
				m_mtlList.push_back(std::move(newMtl));
				newMtl.reset();
				newMtl = unique_ptr<Material> (new Material());
			}

			readyToAddMtl = true;
		}
		else if (type_str == "Ns")
		{
			float ns;
			str_stream >> ns;
			newMtl->SetNs(ns);
		}
		else if (type_str == "Kd")
		{
			float r,g,b;
			str_stream >> r;
			str_stream >> g;
			str_stream >> b;
			if((r != 1.0f) && (g != 1.0f) && (b != 1.0f))
			{
				newMtl->SetColor(r, g, b, 1.0f);
			}
		}
		else if (type_str == "Ks")
		{
			float ks;
			str_stream >> ks;
			newMtl->SetKs(ks);
		}
		else if (type_str == "map_Kd")
		{
			str_stream >> type_str;

			texList.reserve(1);
			texList.push_back(type_str);
		}
	}

	m_mtlList.reserve(1);
	m_mtlList.push_back(std::move(newMtl));

	int length = (int)texList.size();
	m_textures.resize(length);

	for (int i=0; i<length; i++)
	{
		m_textures[i].Load(texList[i]);
	}

	return true;
}

void ObjModel::Initial()
{
	// create one VAO
	glGenVertexArrays(1, &m_idVAO);
	glBindVertexArray(m_idVAO);

	// setup vertex VBO
	m_vboVertexData.CreateVBO();
	m_vboVertexData.BindVBO();
	m_vboVertexData.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// setup texCoord VBO
	m_vboTexCoordData.CreateVBO();
	m_vboTexCoordData.BindVBO();
	m_vboTexCoordData.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// setup vertex normal VBO
	m_vboVertexNormalData.CreateVBO();
	m_vboVertexNormalData.BindVBO();
	m_vboVertexNormalData.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void ObjModel::Render()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(m_idVAO);

	//glDrawArrays(GL_LINE_LOOP, 0, count);
	glDrawArrays(GL_TRIANGLES, 0, m_count);

}

void ObjModel::Draw()
{
	glBindVertexArray(m_idVAO);

	int length = m_mtlList.size();
	int count = 0;
	int first = 0;

	for(int i=0; i<length; i++)
	{
		count = m_mtlList[i]->GetVecticesNum();
		m_textures[i].BindTexture(2);
		//mtlList[i]->GetTexture(0).BindTexture();
		building.SetUniform("gTexture", 2);
		glDrawArrays(GL_TRIANGLES, first, count);
		first += count;
	}
}

std::vector<std::string> ObjModel::split(std::string str, std::string pattern) {
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;
	int size = str.size();

	for (int i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if ((int)pos < size) {
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

const string & ObjModel::GetInstantName() const
{
	return m_name;
}

void ObjModel::ReleaseObj()
{
	if(!m_bLoaded)return;

//	int texLength = textureList.size();
//	for(int i=0; i<texLength; ++i)
//	{
//		textureList[i].releaseTexture();
//	}

	glDeleteVertexArrays(1, &m_idVAO);

	m_vboVertexData.ReleaseVBO();
	m_vboTexCoordData.ReleaseVBO();
	m_vboVertexNormalData.ReleaseVBO();

	int length = m_mtlList.size();
	for(int i=0; i<length; i++)
	{
		m_mtlList[i].reset();
	}
	m_mtlList.clear();

	length = m_textures.size();
	for(int i=0; i<length; i++)
	{
		m_textures[i].DeleteTexture();
	}
	m_textures.clear();

	m_bLoaded = false;
}

