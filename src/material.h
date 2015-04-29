#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "global.h"
#include "texture.h"

class Material {
public:
	Material();
	~Material();
	void SetColor(float r, float g, float b, float a);
	const glm::vec4 & GetColor() const;
	void AddTexture(const Texture2D & newTex);
	Texture2D & GetTexture(int index);
	bool IsTextureUsed() const;
	void SetVecticesNum(int num);
	int GetVecticesNum() const;
	void SetNs(float nS);
	float GetNs() const;
	void SetKs(float kS);
	float GetKs() const;
private:
	glm::vec4 color;
	bool isTexUsed;
	vector<Texture2D> texList;
	int num_vertices;
	float ns;
	float ks;
};

inline Material::Material():
	isTexUsed(false),
	texList(0),
	num_vertices(0),
	ns(0),
	ks(0)
{
}

inline Material::~Material()
{
}

inline void Material::SetColor(float r, float g, float b, float a)
{
	color = glm::vec4(r, g, b, a);
}

inline const glm::vec4 & Material::GetColor() const
{
	return color;
}

inline bool Material::IsTextureUsed() const
{
	return isTexUsed;
}

inline void Material::AddTexture(const Texture2D & newTex)
{
	texList.reserve(1);
	texList.push_back(newTex);
	isTexUsed = true;
}

inline Texture2D & Material::GetTexture(int index)
{
	return texList[index];
}

inline void Material::SetVecticesNum(int num)
{
	num_vertices = num;
}

inline int Material::GetVecticesNum() const
{
	return num_vertices;
}

inline void Material::SetNs(float nS)
{
	ns = nS;
}
inline float Material::GetNs() const
{
	return ns;
}

inline void Material::SetKs(float kS)
{
	ks = kS;
}
inline float Material::GetKs() const
{
	return ks;
}

#endif /* MATERIAL_H_ */
