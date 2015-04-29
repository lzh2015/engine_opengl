#ifndef SKY_H_
#define SKY_H_

#pragma once

#include "textureCubemap.h"
#include "vertexBufferObject.h"
#include "viewCamera.h"
#include "light.h"
#include "objModel.h"

class Sky
{
public:
	static Sky& GetInstance();
	bool Initialize();
	void RenderSky(const TextureCubemap & cubemap, bool bInverse);
	void Delete();
private:
	Sky();
	~Sky();
	Sky(Sky const&);  // Don't Implement
	void operator=(Sky const&); // Don't implement
	ObjModel	   m_sky;
//	VertexBufferObject m_vbo;
//	UINT m_vao;
};


#endif /* SKY_H_ */
