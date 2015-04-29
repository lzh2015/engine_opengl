#include "sky.h"
#include "shader.h"

Sky& Sky::GetInstance() {
	static Sky instance; // Guaranteed to be destroyed.
						 // Instantiated on first use.
	return instance;
}

bool Sky::Initialize()
{
	//load obj
	if(!m_sky.Load("sun.obj"))
	{
		return false;
	}

//	static float points[] =
//	{
//	  -10.0f,  10.0f, -10.0f,
//	  -10.0f, -10.0f, -10.0f,
//	   10.0f, -10.0f, -10.0f,
//	   10.0f, -10.0f, -10.0f,
//	   10.0f,  10.0f, -10.0f,
//	  -10.0f,  10.0f, -10.0f,
//
//	  -10.0f, -10.0f,  10.0f,
//	  -10.0f, -10.0f, -10.0f,
//	  -10.0f,  10.0f, -10.0f,
//	  -10.0f,  10.0f, -10.0f,
//	  -10.0f,  10.0f,  10.0f,
//	  -10.0f, -10.0f,  10.0f,
//
//	   10.0f, -10.0f, -10.0f,
//	   10.0f, -10.0f,  10.0f,
//	   10.0f,  10.0f,  10.0f,
//	   10.0f,  10.0f,  10.0f,
//	   10.0f,  10.0f, -10.0f,
//	   10.0f, -10.0f, -10.0f,
//
//	  -10.0f, -10.0f,  10.0f,
//	  -10.0f,  10.0f,  10.0f,
//	   10.0f,  10.0f,  10.0f,
//	   10.0f,  10.0f,  10.0f,
//	   10.0f, -10.0f,  10.0f,
//	  -10.0f, -10.0f,  10.0f,
//
//	  -10.0f,  10.0f, -10.0f,
//	   10.0f,  10.0f, -10.0f,
//	   10.0f,  10.0f,  10.0f,
//	   10.0f,  10.0f,  10.0f,
//	  -10.0f,  10.0f,  10.0f,
//	  -10.0f,  10.0f, -10.0f,
//
//	  -10.0f, -10.0f, -10.0f,
//	  -10.0f, -10.0f,  10.0f,
//	   10.0f, -10.0f, -10.0f,
//	   10.0f, -10.0f, -10.0f,
//	  -10.0f, -10.0f,  10.0f,
//	   10.0f, -10.0f,  10.0f
//	};
//	glGenVertexArrays(1, &m_vao);
//	glBindVertexArray(m_vao);
//
//	m_vbo.CreateVBO();
//	m_vbo.BindVBO();
//	m_vbo.AddData(&points, 6*18*sizeof(float));
//	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray (0);
//	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	glBindVertexArray(0);

	return true;
}

void Sky::RenderSky(const TextureCubemap & cubemap, bool bInverse)
{
	glDepthMask(GL_FALSE);

	ViewCamera & cam = ViewCamera::GetInstance();

	glm::mat4 mvp;

	glm::mat4 transform = glm::translate(cam.GetEyePosition()) * glm::scale(glm::mat4(1.0f),glm::vec3(200.0f, 200.0f, 200.0f));
	//glm::mat4 transform = glm::scale(glm::mat4(1.0f),glm::vec3(10000.0f, 10000.0f, 10000.0f));

	if(bInverse)
	{
		glDisable(GL_CULL_FACE);
		glm::mat4 inverseTransform = glm::translate(cam.GetEyePosition()) * glm::scale(glm::mat4(1.0f),glm::vec3(1.0, -1.0, 1.0));
		mvp = cam.GetProjectionMatrix() * cam.LookAt() * inverseTransform;
	}
	else
	{
		if(VOLUMETRIC_LIGHT) mvp = cam.GetProjectionMatrix(90) * cam.LookAt() * transform;
		else mvp = cam.GetProjectionMatrix() * cam.LookAt() * transform;
	}

	//temp
	glm::vec3 lightDir = LightResource::GetInstance().GetLight(0).GetDirection();
	glm::vec4 orange(0.8f, 0.3f, 0.0f, 1.0f);
	glm::vec4 yellow(1.0f, 1.0f, 0.8f, 1.0f);

	glm::vec4 color;
	float light_color_factor = glm::clamp(lightDir.y * 5.0, -2.0, 1.0);
	if(lightDir.y < -0.2) color = glm::vec4(0,0,0,1);
	else color = glm::lerp(orange, yellow,  0.1f + light_color_factor * 0.9f);

	sky.UseProgram();
		sky.SetUniform("MVP", mvp);
		sky.SetUniform("transform", transform);
		sky.SetUniform("eyePosition", ViewCamera::GetInstance().GetEyePosition());
		//sky.SetUniform("sunLight.vColor", LightResource::GetInstance().GetLight(0).GetDiffuseColor());
		sky.SetUniform("sunLight.vColor", color);
		sky.SetUniform("sunLight.vDirection", LightResource::GetInstance().GetLight(0).GetDirection());
		sky.SetUniform("volumetric_light", VOLUMETRIC_LIGHT);
		cubemap.BindTexture();
//		glBindVertexArray(m_vao);
//		glDrawArrays (GL_TRIANGLES, 0, 36);
		m_sky.Render();
	sky.UnUseProgram();

	glDepthMask(GL_TRUE);
}

void Sky::Delete()
{
	m_sky.ReleaseObj();
}

Sky::Sky()
{

}

Sky::~Sky()
{
//	m_vbo.ReleaseVBO();
//	glDeleteVertexArrays(1, &m_vao);
}
