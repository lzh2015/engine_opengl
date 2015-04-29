
#ifndef VERTEXBUFFEROBJECT_H_
#define VERTEXBUFFEROBJECT_H_

#include "global.h"
using namespace std;

class VertexBufferObject
{
public:
	void CreateVBO(int a_iSize = 0);
	void ReleaseVBO();

	void* mapBufferToMemory(int iUsageHint);
	void* mapSubBufferToMemory(int iUsageHint, UINT uiOffset, UINT uiLength);
	void unmapBuffer();

	void BindVBO(int a_iBufferType = GL_ARRAY_BUFFER);
	void UploadDataToGPU(int iUsageHint);
	
	void AddData(void* ptrData, UINT uiDataSize);

	void* getDataPointer();
	UINT getBuffer();

	VertexBufferObject();

private:
	UINT m_uiBuffer;
	int m_iSize;
	int m_iBufferType;
	vector<BYTE> m_data;

	bool m_bDataUploaded;
};

#endif
