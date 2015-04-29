#include "terrain.h"
#include "../image.h"
#include "../light.h"
#include <glm/glm.hpp>

#define EPSILON				0.000001f
#define M_PI				3.141592653589793238462643383279f		// PI
#define M_PIDIV2			1.570796326794896619231321691639f		// PI / 2
#define M_2PI				6.283185307179586476925286766559f		// 2 * PI
#define M_PI2				9.869604401089358618834490999876f		// PI au carre
#define M_PIDIV180			0.01745329251994329576923690768488f		// PI / 180
#define M_180DIVPI			57.295779513082320876798154814105f		// 180 / PI

#define DegToRad(a)	(a)*=M_PIDIV180
#define RadToDeg(a)	(a)*=M_180DIVPI
#define RADIANS(a)	((a)*M_PIDIV180)
#define DEGRES(a)	((a)*M_180DIVPI)

#define COORD(x,y,w)	((y)*(w)+(x))

Terrain::Terrain():
	m_width(0),
	m_height(0),
	m_bpp(0),
	m_idGroundVAO(0),
	m_idGrassVAO(0),
	m_vertices(0),
	m_normal(0),
	m_tangent(0),
	m_pQuadtree(NULL),
	m_tree_1("m_tree_1"),
	m_tree_2("m_tree_2"),
	m_tree_3("m_tree_3")
{

}

bool Terrain::Load(const string& heightmap, const BoundingBox& bbox, int chunkSize)
{
	std::cout << "Loading Terrain..." << std::endl;

	//load heightmap file
	string a_sPath = "./data/textures/" + heightmap;
	BYTE * data = ImageTool::GetInstance().LoadImage(a_sPath, m_width, m_height, m_bpp);

	if(data == NULL)
	{
		return false;
	}

	m_bpp = (m_bpp == 24)?3:(m_bpp == 16)?2:1;

	m_bbox = bbox;

	int width = m_width;
	int height = m_height;

	// The heightmap should be in odd size
	// So it can be cut in the quadtree
	if(m_width%2==0)		m_width++;
	if(m_height%2==0)		m_height++;

	//add vertex
	m_vertices.resize(m_height*m_width);
	m_normal.resize(m_height*m_width);
	m_tangent.resize(m_height*m_width);
	std::vector<glm::vec2> uv(m_height*m_width);

	//support height level 8bit or 16bit
	float height_level =  (m_bpp == 3)?256:65536;

	float x_step = (bbox.GetMaxX() - bbox.GetMinX())/(m_width - 1);
	float y_step = (bbox.GetMaxY() - bbox.GetMinY())/height_level;
	float z_step = (bbox.GetMaxZ() - bbox.GetMinZ())/(m_height - 1);

	glm::vec3 uv_length = bbox.max - bbox.min;

	float h;

	//calculate vertex and vertex texture
	for(int i=0; i<m_height; i++)
	{
		for(int j=0; j<m_width; j++)
		{
			int index = i * m_width + j;

			m_vertices[index].x = bbox.GetMinX() + ((float)j) * x_step;
			m_vertices[index].z = bbox.GetMinZ() + ((float)i) * z_step;

			int height_index = ((i == height)?i-1:i) * height * m_bpp + ((j ==  width)?j-1:j)* m_bpp;

			//float h = (float)(data[height_index + 0] + data[height_index + 1] + data[height_index + 2])/3;
			if(m_bpp == 3)
			{
				//h = (float)(data[height_index + 0]);
				h = (float)(data[height_index + 0] + data[height_index + 1] + data[height_index + 2])/3;
			}
			else if(m_bpp == 2)
			{
				h = (data[height_index + 1] << 8) | data[height_index + 0];
				//cout << h << endl;
			}

			m_vertices[index].y = bbox.min.y + h * y_step;

			//cout << m_vertices[index].y << endl;

			glm::vec3 vertex_normalized = (m_vertices[index] - bbox.min) / uv_length;
			uv[index] = glm::vec2(vertex_normalized.x, vertex_normalized.z);
		}
	}

	delete[] data;

	// Normals are here - the heightmap contains ( (m_height-1)*(m_width-1) quads, each one containing 2 triangles, therefore array of we have 3D array)
	vector< vector<glm::vec3> > vNormals[2];
	vNormals[0] = vector< vector<glm::vec3> >(m_height-1, vector<glm::vec3>(m_width-1));
	vNormals[1] = vector< vector<glm::vec3> >(m_height-1, vector<glm::vec3>(m_width-1));

	vector< vector<glm::vec3> > vTangents[2];
	vTangents[0] = vector< vector<glm::vec3> >(m_height-1, vector<glm::vec3>(m_width-1));
	vTangents[1] = vector< vector<glm::vec3> >(m_height-1, vector<glm::vec3>(m_width-1));

	vector< vector<glm::vec3> > vBtangents[2];
	vBtangents[0] = vector< vector<glm::vec3> >(m_height-1, vector<glm::vec3>(m_width-1));
	vBtangents[1] = vector< vector<glm::vec3> >(m_height-1, vector<glm::vec3>(m_width-1));

	for(int i=0; i<(m_height-1); i++)
	{
		for(int j=0; j<(m_width-1); j++)
		{
			glm::vec3 vTriangle0[] =
			{
				m_vertices[(i * m_width + j)],
				m_vertices[((i+1) * m_width + j)],
				m_vertices[((i+1) * m_width + (j+1))]
			};

			//only for debug
			glm::vec3 deltaPos1 = m_vertices[((i+1) * m_width + j)] - m_vertices[(i * m_width + j)];
			glm::vec3 deltaPos2 = m_vertices[((i+1) * m_width + (j+1))] - m_vertices[(i * m_width + j)];

			glm::vec2 deltaUV1 = uv[((i+1) * m_width + j)]-uv[(i * m_width + j)];
			glm::vec2 deltaUV2 = uv[((i+1) * m_width + (j+1))]-uv[(i * m_width + j)];

			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

			glm::vec3 tangent, bitangent;

			tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)* r;
			tangent = glm::normalize(tangent);

			bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)* r;
			bitangent = glm::normalize(bitangent);

			vTangents[0][i][j] = glm::normalize(tangent);
			vBtangents[0][i][j] = glm::normalize(bitangent);
			//if((i * m_width + j) < 100)cout << glm::to_string(tangent) << endl;
			//debug finish

			glm::vec3 vTriangle1[] =
			{
				m_vertices[((i+1) * m_width + (j+1))],
				m_vertices[(i * m_width + (j+1))],
				m_vertices[(i * m_width + j)]
			};

			//only for debug
			deltaPos1 = m_vertices[(i * m_width + (j+1))] - m_vertices[((i+1) * m_width + (j+1))];
			deltaPos2 = m_vertices[(i * m_width + j)] - m_vertices[((i+1) * m_width + (j+1))];

			deltaUV1 = uv[(i * m_width + (j+1))]-uv[((i+1) * m_width + (j+1))];
			deltaUV2 = uv[(i * m_width + j)]-uv[((i+1) * m_width + (j+1))];

			r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

			tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)* r;
			tangent = glm::normalize(tangent);

			bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)* r;
			bitangent = glm::normalize(bitangent);

			vTangents[1][i][j] = glm::normalize(tangent);
			vBtangents[1][i][j] = glm::normalize(bitangent);
			//debug finish

			glm::vec3 vTriangleNorm0 = glm::cross(vTriangle0[0]-vTriangle0[1], vTriangle0[1]-vTriangle0[2]);
			glm::vec3 vTriangleNorm1 = glm::cross(vTriangle1[0]-vTriangle1[1], vTriangle1[1]-vTriangle1[2]);

			vNormals[0][i][j] = glm::normalize(vTriangleNorm0);
			vNormals[1][i][j] = glm::normalize(vTriangleNorm1);
		}
	}

	//vector< vector<glm::vec3> > vFinalNormals = vector< vector<glm::vec3> >(m_height, vector<glm::vec3>(m_width));
	std::vector<glm::vec3> vFinalNormals(m_height*m_width);

	std::vector<glm::vec3> vFinalTangents(m_height*m_width);
	std::vector<glm::vec3> vFinalBtangents(m_height*m_width);

	for(int i=0; i<m_height; i++)
	{
		for(int j=0; j<m_width; j++)
		{
			// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
			// of all adjacent triangles' normals

			glm::vec3 vFinalNormal = glm::vec3(0.0f, 0.0f, 0.0f);

			glm::vec3 vFinalTangent = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 vFinalBtangent = glm::vec3(0.0f, 0.0f, 0.0f);

			// Look for upper-left triangles
			if(j != 0 && i != 0)
			{
				for(int k=0; k < 2; k++)
				{
					vFinalNormal += vNormals[k][i-1][j-1];

					vFinalTangent += vTangents[k][i-1][j-1];
					vFinalBtangent += vBtangents[k][i-1][j-1];
				}
			}

			// Look for upper-right triangles
			if(i != 0 && j != m_width-1)
			{
				vFinalNormal += vNormals[0][i-1][j];

				vFinalTangent += vTangents[0][i-1][j];
				vFinalBtangent += vBtangents[0][i-1][j];
			}

			// Look for bottom-right triangles
			if(i != m_height-1 && j != m_width-1)
			{
				for(int k=0; k < 2; k++)
				{
					vFinalNormal += vNormals[k][i][j];

					vFinalTangent += vTangents[k][i][j];
					vFinalBtangent += vBtangents[k][i][j];
				}
			}

			// Look for bottom-left triangles
			if(i != m_height-1 && j != 0)
			{
				vFinalNormal += vNormals[1][i][j-1];

				vFinalTangent += vTangents[1][i][j-1];
				vFinalBtangent += vBtangents[1][i][j-1];
			}

			vFinalNormal = glm::normalize(vFinalNormal);

			vFinalTangent = glm::normalize(vFinalTangent);
			vFinalBtangent = glm::normalize(vFinalBtangent);

			int index = i * m_width + j;

			vFinalNormals[index] = vFinalNormal; // Store final normal of j-th vertex in i-th row
			m_normal[index] = vFinalNormal;

			vFinalTangents[index] = vFinalTangent;
			m_tangent[index] = vFinalTangent;

			vFinalBtangents[index] = vFinalBtangent;
		}
	}


//	//calculate tangent & bitangent
//	int offset = 2;
//
//	vector<glm::vec3> normal(m_height*m_width);
//	vector<glm::vec3> tangent(m_height*m_width);
//
//	for(int j=offset; j<m_height-offset; j++)
//	{
//		for(int i=offset; i<m_width-offset; i++)
//		{
//			int index = j * m_height + i;
//
//			glm::vec3 vU = m_vertices[(i+offset + j * m_width)] - m_vertices[(i-offset + j * m_width)];
//			glm::vec3 vV = m_vertices[(i + (j+offset) * m_width)] - m_vertices[(i + (j-offset) * m_width)];
//
//			normal[index] = glm::normalize(glm::cross(vV, vU));
//			tangent[index] = glm::normalize(-vU);
//
//			//cout << glm::to_string(tangent[index]) << endl;
//		}
//	}
//
//	for(int j=0; j<offset; j++)
//	{
//		for(int i=0; i<m_width; i++)
//		{
//			int index0 = i + j * m_width;
//			int index1 = i + offset * m_width;
//
//			normal[index0] = normal[index1];
//			tangent[index0] = tangent[index1];
//
//			index0 = i + (m_height-1-j) * m_width;
//			index1 = i + (m_height-1-offset ) *	m_width;
//
//			normal[index0] = normal[index1];
//			tangent[index0] = tangent[index1];
//		}
//	}
//
//	for(int i=0; i<offset; i++)
//	{
//		for(int j=0; j<m_height; j++)
//		{
//			int index0 = i + j * m_width;
//			int index1 = offset + j * m_width;
//
//			normal[index0] = normal[index1];
//			tangent[index0] = tangent[index1];
//
//			index0 = m_width-1-i + j * m_width;
//			index1 = m_width-1-offset +	j *	m_width;
//
//			normal[index0] = normal[index1];
//			tangent[index0] = tangent[index1];
//		}
//	}


//	for(int i=0; i<m_height; i++)
//	{
//		for(int j=0; j<m_width; j++)
//		{
//			int index = i * m_width + j;
//
//			if(index > 20000)
//			{
//				cout << "##########" << endl;
//				cout << glm::to_string(vFinalNormals[index]) << endl;
//				cout << glm::to_string(normal[index]) << endl;
//				cout << "##########" << endl;
//
//			}
//
//		}
//	}

	// create one VAO
	glGenVertexArrays(1, &m_idGroundVAO);
	glBindVertexArray(m_idGroundVAO);

	m_groundPosition.AddData(&m_vertices[0], sizeof(glm::vec3) * m_height*m_width);
	m_groundPosition.CreateVBO();
	m_groundPosition.BindVBO();
	m_groundPosition.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	m_groundTexture.AddData(&uv[0], sizeof(glm::vec2) * m_height*m_width);
	m_groundTexture.CreateVBO();
	m_groundTexture.BindVBO();
	m_groundTexture.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	m_groundNormal.AddData(&vFinalNormals[0], sizeof(glm::vec3) * m_height*m_width);
	m_groundNormal.CreateVBO();
	m_groundNormal.BindVBO();
	m_groundNormal.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	m_groundTangent.AddData(&vFinalTangents[0], sizeof(glm::vec3) * m_height*m_width);
	m_groundTangent.CreateVBO();
	m_groundTangent.BindVBO();
	m_groundTangent.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

//	m_groundBtangent.AddData(&vFinalBtangents[0], sizeof(glm::vec3) * m_height*m_width);
//	m_groundBtangent.CreateVBO();
//	m_groundBtangent.BindVBO();
//	m_groundBtangent.UploadDataToGPU(GL_STATIC_DRAW);
//	glEnableVertexAttribArray(4);
//	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	std::cout << "Loading Terrain OK" << std::endl;


	std::cout << "Loading Quadtree..." << std::endl;
	//cout << glm::to_string(glm::ivec2(m_width, m_height)) << endl;
	m_pQuadtree = new Quadtree();
	m_pQuadtree->Build(&m_bbox, glm::ivec2(m_width, m_height), chunkSize);


	std::cout << "Loading Quadtree OK" << std::endl;

	return true;
}

bool Terrain::GenerateGrass(const std::string& map, int density)
{
	std::cout << "Loading Grass..." << std::endl;

	//load heightmap file
	string a_sPath = "./data/textures/" + map;
	int width, height, bpp;

	BYTE * data = ImageTool::GetInstance().LoadImage(a_sPath, width, height, bpp);
	//cout << bpp << endl;
	bpp = (bpp == 32)? 4:(bpp==24)? 3:(bpp==16)? 2:1;

	if(data == NULL)
	{
		return false;
	}

	glm::vec2 pos0(cos(0*PI/180), sin(0*PI/180));
	glm::vec2 pos120(cos(120*PI/180), sin(120*PI/180));
	glm::vec2 pos240(cos(240*PI/180), sin(240*PI/180));

	glm::vec3 tVertices[] =
	{
		glm::vec3(-pos0.x, -pos0.y, 0.0f), 		glm::vec3(-pos0.x, -pos0.y, 1.0f), 		glm::vec3(pos0.x, pos0.y, 1.0f), 		glm::vec3(pos0.x, pos0.y, 0.0f),
		glm::vec3(-pos120.x, -pos120.y, 0.0f),	glm::vec3(-pos120.x, -pos120.y, 1.0f),	glm::vec3(pos120.x, pos120.y, 1.0f),	glm::vec3(pos120.x, pos120.y, 0.0f),
		glm::vec3(-pos240.x, -pos240.y, 0.0f),	glm::vec3(-pos240.x, -pos240.y, 1.0f),	glm::vec3(pos240.x, pos240.y, 1.0f),	glm::vec3(pos240.x, pos240.y, 0.0f)
	};

	glm::vec2 tTexcoords[] =
	{
		glm::vec2(0.0f, 0.49f), glm::vec2(0.0f, 0.01f), glm::vec2(1.0f, 0.01f), glm::vec2(1.0f, 0.49f),
		glm::vec2(0.0f, 0.49f), glm::vec2(0.0f, 0.01f), glm::vec2(1.0f, 0.01f), glm::vec2(1.0f, 0.49f),
		glm::vec2(0.0f, 0.49f), glm::vec2(0.0f, 0.01f), glm::vec2(1.0f, 0.01f), glm::vec2(1.0f, 0.49f)
	};

	std::vector<glm::vec3> vPosition;
	std::vector<glm::vec3> vNormal;
	std::vector<glm::vec2> vTexcoord;

	vPosition.reserve( density*3*4 );
	vNormal.reserve( density*3*4 );
	vTexcoord.reserve( density*3*4 );

	for(int i=0; i<(int)density; i++)
	{
		float x = random(1.0f);
		float y = random(1.0f);
		int map_x = (int)(x * width);
		int map_y = (int)(y * height);
		glm::vec2 uv_offset = glm::vec2(0.0f, random(3)==0 ? 0.0f : 0.5f);
		float size = random(0.5f) + 0.5f;

		int index = (map_x + map_y * width) * bpp;
		glm::vec4 map_color = glm::vec4(data[index + 0], data[index + 1], data[index + 2], data[index + 3]);

		if(map_color.y < 200)
		{
			i--;
			continue;
		}

		size = (float)(map_color.y+1) / 256;

		glm::vec3 P = getPosition(x, y);
		glm::vec3 N = getNormal(x, y);
		glm::vec3 T = getTangent(x, y);
		glm::vec3 B = glm::normalize(glm::cross(T, N));

		if(N.y < 0.7f)
		{
			i--;
			continue;
		}
		else
		{
			float degree = random(360.0f);
			glm::mat4 matRot = glm::rotate(glm::mat4(1.0f), degree, glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));

			int idx = (int)vPosition.size();

			glm::vec2 temp(P.x, P.z);
			QuadtreeNode* node = m_pQuadtree->FindLeaf(temp);
			TerrainChunk* chunk = node->getChunk();
			//cout << glm::to_string(temp) << endl;
			for(int j=0; j<3*4; j++)
			{
				glm::vec3 data = glm::vec3(scale*matRot*glm::vec4(tVertices[j]*size, 1.0));
				//cout << glm::to_string(data) << endl;
				glm::vec3 vertex = P;
				vertex.x += glm::dot(data, T);
				vertex.y += glm::dot(data, B);
				vertex.z += glm::dot(data, N);

				//cout << glm::to_string(vertex) << endl;
				vPosition.push_back(vertex);
				vNormal.push_back( tTexcoords[j].t < 0.2f ? -N : N );
				vTexcoord.push_back( uv_offset + tTexcoords[j] );
				//vTexcoord.push_back(tTexcoords[j] );

				if(j == 3 || j == 7 || j == 11)
				{
					chunk->GetGrassIndiceArray().push_back(idx + j - 3);
					chunk->GetGrassIndiceArray().push_back(idx + j - 3 + 1);
					chunk->GetGrassIndiceArray().push_back(idx + j - 3 + 2);

					chunk->GetGrassIndiceArray().push_back(idx + j - 3);
					chunk->GetGrassIndiceArray().push_back(idx + j - 3 + 2);
					chunk->GetGrassIndiceArray().push_back(idx + j - 3 + 3);
					//chunk->GetGrassIndiceArray().push_back(idx+j);
				}
			}
		}
	}

	// create one VAO
	glGenVertexArrays(1, &m_idGrassVAO);
	glBindVertexArray(m_idGrassVAO);

	m_grassPosition.AddData(&vPosition[0], sizeof(glm::vec3) * density * 3 * 4);
	m_grassPosition.CreateVBO();
	m_grassPosition.BindVBO();
	m_grassPosition.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	m_grassTexture.AddData(&vTexcoord[0], sizeof(glm::vec2) * density * 3 * 4);
	m_grassTexture.CreateVBO();
	m_grassTexture.BindVBO();
	m_grassTexture.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	m_grassNormal.AddData(&vNormal[0], sizeof(glm::vec3) * density * 3 * 4);
	m_grassNormal.CreateVBO();
	m_grassNormal.BindVBO();
	m_grassNormal.UploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);

	std::cout << "Loading Grass OK" << std::endl;
	return true;
}

bool Terrain::GenerateVegetation(const std::string& map, int density, float waterHeight)
{
	std::cout << "Loading Vegetation..." << std::endl;

	int palm_tree_count = density * 0.3;
	int water_grass = density * 0.2;

	//load obj
	if(!m_tree_1.Load("tree_1.obj"))
	{
		return false;
	}

	if(!m_tree_2.Load("tree_2.obj"))
	{
		return false;
	}

	if(!m_tree_3.Load("tree_3.obj"))
	{
		return false;
	}

	if(!m_tree_4.Load("tree_4.obj"))
	{
		return false;
	}

	//load heightmap file
	string a_sPath = "./data/textures/" + map;
	int width, height, bpp;

	BYTE * data = ImageTool::GetInstance().LoadImage(a_sPath, width, height, bpp);
	//cout << bpp << endl;
	bpp = (bpp == 32)? 4:(bpp==24)? 3:(bpp==16)? 2:1;

	if(data == NULL)
	{
		return false;
	}

	for(int i=0; i<density; i++)
	{
		float x = random(1.0f);
		float y = random(1.0f);
		int map_x = (int)(x * width);
		int map_y = (int)(y * height);

		int index = (map_x + map_y * width) * bpp;
		glm::vec4 map_color = glm::vec4(data[index + 0], data[index + 1], data[index + 2], data[index + 3]);

		glm::vec3 P = getPosition(x, y);

		if(i <= water_grass)
		{
			if(P.y >= waterHeight)
			{
				i--;
				continue;
			}

			if(map_color.w < 200)
			{
				i--;
				continue;
			}

			glm::vec2 temp(P.x, P.z);
			QuadtreeNode* node = m_pQuadtree->FindLeaf(temp);
			TerrainChunk* chunk = node->getChunk();

			chunk->AddObject( new TerrainObject(&m_tree_4, glm::vec4(P.x, P.y-1.0f, P.z, random(360.0f))));
		}
		else if(i <= (palm_tree_count + water_grass))
		{
			if(P.y < waterHeight)
			{
				i--;
				continue;
			}

			if(map_color.w < 200)
			{
				i--;
				continue;
			}

			glm::vec2 temp(P.x, P.z);
			QuadtreeNode* node = m_pQuadtree->FindLeaf(temp);
			TerrainChunk* chunk = node->getChunk();

			chunk->AddObject( new TerrainObject(&m_tree_1, glm::vec4(P.x, P.y-1.0f, P.z, random(360.0f))));
		}
		else
		{
			if(P.y < waterHeight)
			{
				i--;
				continue;
			}

			if(map_color.y < 200)
			{
				i--;
				continue;
			}

			glm::vec2 temp(P.x, P.z);
			QuadtreeNode* node = m_pQuadtree->FindLeaf(temp);
			TerrainChunk* chunk = node->getChunk();

			if(i%4 == 0) chunk->AddObject( new TerrainObject(&m_tree_3, glm::vec4(P.x, P.y-1.0f, P.z, random(360.0f))));
			else chunk->AddObject( new TerrainObject(&m_tree_2, glm::vec4(P.x, P.y-1.0f, P.z, random(360.0f))));
		}

	}

	std::cout << "Loading Vegetation OK" << std::endl;

	return true;
}

void Terrain::ComputeBoundingBox()
{
	m_pQuadtree->ComputeBoundingBox(m_vertices);
}

int Terrain::DrawGround(bool bReflection)
{
	glBindVertexArray(m_idGroundVAO);

	int ret = m_pQuadtree->DrawGround(bReflection);

	return ret;
}

int Terrain::DrawGrass(bool bReflection)
{
	glBindVertexArray(m_idGrassVAO);

	int ret = m_pQuadtree->DrawGrass(bReflection);

	return ret;
}

int Terrain::DrawObjects(bool bReflection)
{
	int ret = m_pQuadtree->DrawObjects(bReflection);

	return ret;
}

glm::vec3 Terrain::getPosition(float x_clampf, float z_clampf) const
{
	if(x_clampf<.0f || z_clampf<.0f || x_clampf>1.0f || z_clampf>1.0f) return glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec2  posF(	x_clampf * m_width, z_clampf * m_height );
	glm::ivec2 posI(	(int)(posF.x), (int)(posF.y) );
	glm::vec2  posD(	posF.x - posI.x, posF.y - posI.y );

	if(posI.x >= (int)m_width-1)		posI.x = m_width-2;
	if(posI.y >= (int)m_height-1)	posI.y = m_height-2;

	glm::vec3 pos(m_bbox.min.x + x_clampf * (m_bbox.max.x - m_bbox.min.x), 0.0f, m_bbox.min.z + z_clampf * (m_bbox.max.z - m_bbox.min.z));
	pos.y =   (m_vertices[ posI.x +  posI.y * m_width ].y) * (1.0f-posD.x) * (1.0f-posD.y)
				+ (m_vertices[ posI.x + 1 + posI.y * m_width ].y) * posD.x * (1.0f-posD.y)
				+ (m_vertices[ posI.x + (posI.y+1) * m_width ].y) * (1.0f-posD.x) * posD.y
				+ (m_vertices[ posI.x + 1 + (posI.y+1) * m_width ].y) * posD.x * posD.y;

	return pos;
}

glm::vec3 Terrain::getNormal(float x_clampf, float z_clampf) const
{
	if(x_clampf<.0f || z_clampf<.0f || x_clampf>1.0f || z_clampf>1.0f) return glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec2  posF(	x_clampf * m_width, z_clampf * m_height );
	glm::ivec2 posI(	(int)(x_clampf * m_width), (int)(z_clampf * m_height) );
	glm::vec2  posD(	posF.x - posI.x, posF.y - posI.y );

	if(posI.x >= (int)m_width-1)		posI.x = m_width-2;
	if(posI.y >= (int)m_height-1)	posI.y = m_height-2;

	return    (m_normal[ posI.x +  posI.y * m_width ])  * (1.0f-posD.x) * (1.0f-posD.y)
				+ (m_normal[ posI.x + 1 + posI.y * m_width ])  *       posD.x  * (1.0f-posD.y)
				+ (m_normal[ posI.x + (posI.y+1) * m_width ])  * (1.0f-posD.x) *       posD.y
				+ (m_normal[ posI.x + 1 + (posI.y+1) * m_width ])  *       posD.x  *       posD.y;
}

glm::vec3 Terrain::getTangent(float x_clampf, float z_clampf) const
{
	if(x_clampf<.0f || z_clampf<.0f || x_clampf>1.0f || z_clampf>1.0f) return glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec2  posF(	x_clampf * m_width, z_clampf * m_height );
	glm::ivec2 posI(	(int)(x_clampf * m_width), (int)(z_clampf * m_height) );
	glm::vec2  posD(	posF.x - posI.x, posF.y - posI.y );

	if(posI.x >= (int)m_width-1)		posI.x = m_width-2;
	if(posI.y >= (int)m_height-1)	posI.y = m_height-2;

	return    (m_tangent[ posI.x +  posI.y * m_width ])  * (1.0f-posD.x) * (1.0f-posD.y)
			+ (m_tangent[ posI.x + 1 + posI.y * m_width ])  *       posD.x  * (1.0f-posD.y)
			+ (m_tangent[ posI.x + (posI.y+1) * m_width ])  * (1.0f-posD.x) *       posD.y
			+ (m_tangent[posI.x + 1 + (posI.y+1) * m_width ])  *       posD.x  *       posD.y;
}

void Terrain::Delete()
{
	m_width = 0;
	m_height = 0;

	if(m_pQuadtree)
	{
		m_pQuadtree->Delete();
		delete m_pQuadtree;
		m_pQuadtree = NULL;
	}
}
