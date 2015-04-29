#include "viewCamera.h"

Frustum::Frustum():
	vertices(0),
	plane_normal(0),
	vertex_length(0),
	plane_length(0)
{

}

void Frustum::AddVertex(const glm::vec3 & p)
{
	vertices.reserve(1);
	vertices.push_back(p);
	++vertex_length;
}

void Frustum::AddPlaneNormal(const glm::vec3 & p)
{
	plane_normal.reserve(1);
	plane_normal.push_back(p);
}

const glm::vec3 & Frustum::GetVertex(int index) const
{
	return vertices[index];
}

int Frustum::GetVertexLength() const
{
	return vertex_length;
}

const glm::vec3 & Frustum::GerPlaneNormal(int index) const
{
	return plane_normal[index];
}

int Frustum::GetPlaneLength() const
{
	return plane_length;
}

void Frustum::Display() const
{
	for(int i=0; i<vertex_length; ++i)
	{
		cout << glm::to_string(vertices[i]) << endl;
	}
}

//#################################################################################

ViewCamera& ViewCamera::GetInstance() {
	static ViewCamera instance; // Guaranteed to be destroyed.
									 // Instantiated on first use.
	return instance;
}

ViewCamera::ViewCamera():
	vEye(620.0f, 110.0f, -441.0f),
	vView(620.0f, 110.0f, -440.0f),
	vUp(0.0f, 1.0f, 0.0f),
	m_fSpeed(15.0f),
	frustums(0),
	m_type(ViewCamera::FREE)
{
	BuildFrustum();
	SetProjectionMatrix();
}

glm::mat4 ViewCamera::LookAt(bool bInverse, float water_height)
{
	if(bInverse)
	{
		glm::vec3 inverseEye = glm::vec3(-vEye.x, 2.0f*water_height-vEye.y,	vEye.z);
		glm::vec3 inverseView = glm::vec3(-vView.x, 2.0f*water_height-vView.y,	vView.z);
		glm::vec3 inverseUp = -vUp;

		return glm::lookAt(inverseEye, inverseView, inverseUp);
	}
	else
	{
		return glm::lookAt(vEye, vView, vUp);
	}

}


void ViewCamera::OnKeyboard(unsigned int input)
{
//	switch (input)
//	{
//		case A_KEY:
//			rotateViewY(0.2f);
//			break;
//		case D_KEY:
//			rotateViewY(-0.2f);
//			break;
//		case S_KEY:
//			move(-m_fSpeed);
//			break;
//		case W_KEY:
//			move(m_fSpeed);
//			break;
//	}
}

void ViewCamera::rotateViewY(float fAngle)
{
	glm::mat4 mRotation = glm::rotate(glm::mat4(1.0f), fAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 vDir = glm::normalize(vView-vEye);
	glm::vec4 vNewDir= mRotation*glm::vec4(vDir, 1.0f);
	vView = glm::vec3(vNewDir) + vEye;
}

void ViewCamera::RotateView(float fAngleX, float fAngleY)
{
	glm::vec3 vDir = glm::normalize(vView-vEye);

	glm::mat4 mRotationX = glm::rotate(glm::mat4(1.0f), fAngleX, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 rotate_vec;

	if(vDir.y >= 0)
	{
		glm::vec3 y = glm::vec3(0.0f, 1.0f, 0.0f);
		rotate_vec = glm::cross(vDir, y);
	}
	else
	{
		glm::vec3 y = glm::vec3(0.0f, -1.0f, 0.0f);
		rotate_vec = glm::cross(y, vDir);
	}

	glm::mat4 mRotationY = glm::rotate(glm::mat4(1.0f), fAngleY, rotate_vec);

	glm::vec4 vNewDir= mRotationX * mRotationY * glm::vec4(vDir, 1.0f);
	vView = glm::vec3(vNewDir) + vEye;
}

void ViewCamera::move(float fBy)
{
	glm::vec3 vDir = glm::normalize(vView-vEye);
	vDir *= fBy*m_fSpeed;
	vEye += vDir;
	vView += vDir;
}

glm::vec3 ViewCamera::GetDirection() const
{
	return glm::normalize(glm::vec3(vView-vEye));
}

const Frustum & ViewCamera::GetFrustum(int index) const
{
	return frustums[index];
}

void ViewCamera::BuildFrustum()
{
	float x,y,z;
	float tan_fov = tan(FOV*0.5/180*PI);

	Frustum newFrustum;
	glm::vec3 newP;
	//cout << SCREEN_ASPECT_RATIO<<endl;
	//calculate the orginal frustum and add it first in frustums

	//improve near plane  by multiply by 5
	//y = tan_fov * 1 * 5;
	y = tan_fov * SCREEN_NEAR;
	x = y * SCREEN_ASPECT_RATIO;
	z = SCREEN_NEAR;

	newP = glm::vec3(-x, -y, -z);
	newFrustum.AddVertex(newP);
	newP = glm::vec3(x, -y, -z);
	newFrustum.AddVertex(newP);
	newP = glm::vec3(-x, y, -z);
	newFrustum.AddVertex(newP);
	newP = glm::vec3(x, y, -z);
	newFrustum.AddVertex(newP);

	z = SCREEN_DEPTH;
	//z = 20;
	y = tan_fov * z;
	x = y * SCREEN_ASPECT_RATIO;

	newP = glm::vec3(-x, -y, -z);
	newFrustum.AddVertex(newP);
	newP = glm::vec3(x, -y, -z);
	newFrustum.AddVertex(newP);
	newP = glm::vec3(-x, y, -z);
	newFrustum.AddVertex(newP);
	newP = glm::vec3(x, y, -z);
	newFrustum.AddVertex(newP);

	//calculate frustum plane normal
	glm::vec3 nw(-x, y, -z);
	glm::vec3 sw(-x, -y, -z);
	glm::vec3 ne(x, y , -z);
	glm::vec3 se(x, -y, -z);

	glm::vec3 normal;
	normal = glm::cross(sw, nw);
	newFrustum.AddPlaneNormal(glm::normalize(normal));

	normal = glm::cross(ne, se);
	newFrustum.AddPlaneNormal(glm::normalize(normal));

	normal = glm::cross(nw, ne);
	newFrustum.AddPlaneNormal(glm::normalize(normal));

	normal = glm::cross(se, sw);
	newFrustum.AddPlaneNormal(glm::normalize(normal));

	normal = glm::vec3(0,0,-1);
	newFrustum.AddPlaneNormal(glm::normalize(normal));

	normal = glm::vec3(0,0,1);
	newFrustum.AddPlaneNormal(glm::normalize(normal));

	frustums.reserve(1);
	frustums.push_back(newFrustum);
	//frustums[0].Display();


	Frustum newFrustum_2;
	y = tan_fov * SCREEN_NEAR;
	x = y * SCREEN_ASPECT_RATIO;
	z = SCREEN_NEAR;
	newP = glm::vec3(-x, -y, -z);
	newFrustum_2.AddVertex(newP);
	newP = glm::vec3(x, -y, -z);
	newFrustum_2.AddVertex(newP);
	newP = glm::vec3(-x, y, -z);
	newFrustum_2.AddVertex(newP);
	newP = glm::vec3(x, y, -z);
	newFrustum_2.AddVertex(newP);

	z = CSM_FRUSTUM_0;
	y = tan_fov * z;
	x = y * SCREEN_ASPECT_RATIO;

	newP = glm::vec3(-x, -y, -z);
	newFrustum_2.AddVertex(newP);
	newP = glm::vec3(x, -y, -z);
	newFrustum_2.AddVertex(newP);
	newP = glm::vec3(-x, y, -z);
	newFrustum_2.AddVertex(newP);
	newP = glm::vec3(x, y, -z);
	newFrustum_2.AddVertex(newP);

	frustums.reserve(1);
	frustums.push_back(newFrustum_2);

	Frustum newFrustum_3;
	y = tan_fov * z;
	x = y * SCREEN_ASPECT_RATIO;
	z = CSM_FRUSTUM_0;
	newP = glm::vec3(-x, -y, -z);
	newFrustum_3.AddVertex(newP);
	newP = glm::vec3(x, -y, -z);
	newFrustum_3.AddVertex(newP);
	newP = glm::vec3(-x, y, -z);
	newFrustum_3.AddVertex(newP);
	newP = glm::vec3(x, y, -z);
	newFrustum_3.AddVertex(newP);

	z = CSM_FRUSTUM_1;
	y = tan_fov * z;
	x = y * SCREEN_ASPECT_RATIO;

	newP = glm::vec3(-x, -y, -z);
	newFrustum_3.AddVertex(newP);
	newP = glm::vec3(x, -y, -z);
	newFrustum_3.AddVertex(newP);
	newP = glm::vec3(-x, y, -z);
	newFrustum_3.AddVertex(newP);
	newP = glm::vec3(x, y, -z);
	newFrustum_3.AddVertex(newP);

	frustums.reserve(1);
	frustums.push_back(newFrustum_3);
}

void ViewCamera::SetProjectionMatrix()
{
	m_defaut_projection = glm::perspective(FOV*PI/180, SCREEN_ASPECT_RATIO, SCREEN_NEAR, SCREEN_DEPTH);
	m_projection_90 = glm::perspective(90.0f*PI/180, SCREEN_ASPECT_RATIO, SCREEN_NEAR, SCREEN_DEPTH);
}

const glm::mat4 & ViewCamera::GetProjectionMatrix(float fFOV) const
{
	if(fFOV == 90)
	{
		return m_projection_90;
	}
	else
	{
		return m_defaut_projection;
	}
}

const glm::vec3 & ViewCamera::GetEyePosition() const
{
	return vEye;
}

const glm::vec3 & ViewCamera::GetViewPosition() const
{
	return vView;
}

int ViewCamera::ContainsSphere(const glm::vec3 & center, float radius) const
{
	//cout << glm::to_string(center) << " radius: "<< radius << endl;
	glm::vec3 point = glm::vec3(glm::lookAt(vEye, vView, vUp) * glm::vec4(center, 1.0f));

	for(int p = 0; p < 6; p++)
	{
		float distance;
		if(p < 4)
		{
			distance = frustums[0].GerPlaneNormal(p).x * point.x +
			           frustums[0].GerPlaneNormal(p).y * point.y +
					   frustums[0].GerPlaneNormal(p).z * point.z;
		}
		else if(p == 4)
		{
			glm::vec3 vectorSubtract = point - frustums[0].GetVertex(0);
			distance = frustums[0].GerPlaneNormal(p).x * vectorSubtract.x +
					   frustums[0].GerPlaneNormal(p).y * vectorSubtract.y +
					   frustums[0].GerPlaneNormal(p).z * vectorSubtract.z;
		}
		else if(p == 5)
		{
			glm::vec3 vectorSubtract = point - frustums[0].GetVertex(7);
			distance = frustums[0].GerPlaneNormal(p).x * vectorSubtract.x +
					   frustums[0].GerPlaneNormal(p).y * vectorSubtract.y +
					   frustums[0].GerPlaneNormal(p).z * vectorSubtract.z;
		}

		if( distance < -radius)
			return FRUSTUM_OUT;

		if((float)fabs(distance) < radius)
			return FRUSTUM_INTERSECT;
	}
	return FRUSTUM_IN;
}

int ViewCamera::ContainsBoundingBox(const BoundingBox& bbox) const
{
	glm::vec3 tCorners[8] = {glm::vec3(bbox.min.x, bbox.min.y, bbox.min.z),
						     glm::vec3(bbox.max.x, bbox.min.y, bbox.min.z),
							 glm::vec3(bbox.min.x, bbox.max.y, bbox.min.z),
							 glm::vec3(bbox.min.x, bbox.min.y, bbox.max.z),
							 glm::vec3(bbox.max.x, bbox.max.y, bbox.min.z),
							 glm::vec3(bbox.min.x, bbox.max.y, bbox.max.z),
							 glm::vec3(bbox.max.x, bbox.min.y, bbox.max.z),
							 glm::vec3(bbox.max.x, bbox.max.y, bbox.max.z)	};

	for (int i = 0; i<8; i++)
	{
		tCorners[i] = glm::vec3(glm::lookAt(vEye, vView, vUp) * glm::vec4(tCorners[i], 1.0f));
	}

	int iTotalIn = 0;

	for(int p=0; p<6; p++)
	{
		int iInCount = 8;
		int iPtIn = 1;

		for(int c=0; c<8; c++)
		{

			float side;
			if(p < 4)
			{
				side = frustums[0].GerPlaneNormal(p).x *  tCorners[c].x +
						   frustums[0].GerPlaneNormal(p).y *  tCorners[c].y +
						   frustums[0].GerPlaneNormal(p).z *  tCorners[c].z;
			}
			else if(p == 4)
			{
				glm::vec3 vectorSubtract =  tCorners[c] - frustums[0].GetVertex(0);
				side = frustums[0].GerPlaneNormal(p).x * vectorSubtract.x +
						   frustums[0].GerPlaneNormal(p).y * vectorSubtract.y +
						   frustums[0].GerPlaneNormal(p).z * vectorSubtract.z;
			}
			else if(p == 5)
			{
				glm::vec3 vectorSubtract =  tCorners[c] - frustums[0].GetVertex(7);
				side = frustums[0].GerPlaneNormal(p).x * vectorSubtract.x +
						   frustums[0].GerPlaneNormal(p).y * vectorSubtract.y +
						   frustums[0].GerPlaneNormal(p).z * vectorSubtract.z;
			}


			if(side < 0) {
				iPtIn = 0;
				iInCount--;
			}
		}

		if(iInCount == 0)
		{
			return FRUSTUM_OUT;
		}


		iTotalIn += iPtIn;
	}

	if(iTotalIn == 6)
		return FRUSTUM_IN;

	//return FRUSTUM_IN;
	return FRUSTUM_INTERSECT;
}
