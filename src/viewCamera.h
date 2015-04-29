#pragma once

#include "global.h"
#include "boundingbox.h"

#define FRUSTUM_OUT			0
#define FRUSTUM_IN			1
#define FRUSTUM_INTERSECT	2

static const int CSM_FRUSTUMS = 2;
static const float CSM_FRUSTUM_0 = 30.0;
static const float CSM_FRUSTUM_1 = 200.0;

class Frustum {
public:
	Frustum();
	void AddVertex(const glm::vec3 & p);
	void AddPlaneNormal(const glm::vec3 & p);
	const glm::vec3 & GetVertex(int index) const;
	int GetVertexLength() const;
	const glm::vec3 & GerPlaneNormal(int index) const;
	int GetPlaneLength() const;
	void Display() const;
private:
	vector<glm::vec3> vertices;
	vector<glm::vec3> plane_normal;
	int vertex_length;
	int plane_length;
};

//###############################################################################

class ViewCamera
{
public:
	enum TYPE {FREE, DRIVEN};
	static ViewCamera& GetInstance();
	//ViewCamera(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed);
	glm::mat4 LookAt(bool bInverse = false, float water_height = 0);
	void rotateViewY(float fAngle);
	void RotateView(float fAngleX, float fAngleY);
	void move(float fBy);
	void OnKeyboard(unsigned int input);
	glm::vec3 GetDirection() const;
	const Frustum & GetFrustum(int index) const;
	const glm::vec3 & GetEyePosition() const;
	const glm::vec3 & GetViewPosition() const;
	void SetProjectionMatrix();
	const glm::mat4 & GetProjectionMatrix(float fFOV = 0) const;
	int ContainsSphere(const glm::vec3 & center, float radius) const;
	int ContainsBoundingBox(const BoundingBox & bbox) const;
	inline TYPE  GetType() const {return m_type;}
	inline void SetType(TYPE t)  { m_type = t;}//cout << m_type << endl;}
	void setEye(glm::vec3 vPos)	 { vEye = vPos; }
	void setView(glm::vec3 vPos) { vView = vPos; }
private:
	glm::vec3 vEye, vView, vUp;
	float m_fSpeed;
	vector<Frustum> frustums;
	glm::mat4 m_defaut_projection;
	glm::mat4 m_projection_90;
	TYPE m_type;
	ViewCamera();
	~ViewCamera() {};
	ViewCamera(ViewCamera const&);  // Don't Implement
	void operator=(ViewCamera const&); // Don't implement
	void BuildFrustum();
};
