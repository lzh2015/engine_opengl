#include "vertexBufferObject.h"

VertexBufferObject::VertexBufferObject():
	m_uiBuffer(0),
	m_iSize(0),
	m_iBufferType(0),
	m_data(0),
	m_bDataUploaded(false)
{

}


void VertexBufferObject::CreateVBO(int a_iSize)
{
	glGenBuffers(1, &m_uiBuffer);
	m_data.reserve(a_iSize);
	m_iSize = a_iSize;
}

void VertexBufferObject::ReleaseVBO()
{
	glDeleteBuffers(1, &m_uiBuffer);
	m_bDataUploaded = false;
	m_data.clear();
}


void* VertexBufferObject::mapBufferToMemory(int iUsageHint)
{
	if(!m_bDataUploaded)return NULL;
	void* ptrRes = glMapBuffer(m_iBufferType, iUsageHint);
	return ptrRes;
}

void* VertexBufferObject::mapSubBufferToMemory(int iUsageHint, UINT uiOffset, UINT uiLength)
{
	if(!m_bDataUploaded)return NULL;
	void* ptrRes = glMapBufferRange(m_iBufferType, uiOffset, uiLength, iUsageHint);
	return ptrRes;
}

void VertexBufferObject::unmapBuffer()
{
	glUnmapBuffer(m_iBufferType);
}

void VertexBufferObject::BindVBO(int a_iBufferType)
{
	m_iBufferType = a_iBufferType;
	glBindBuffer(m_iBufferType, m_uiBuffer);
}

void VertexBufferObject::UploadDataToGPU(int iDrawingHint)
{
	glBufferData(m_iBufferType, m_data.size(), &m_data[0], iDrawingHint);
	m_bDataUploaded = true;
	m_data.clear();
}

void VertexBufferObject::AddData(void* ptrData, UINT uiDataSize)
{
	m_data.insert(m_data.end(), (BYTE*)ptrData, (BYTE*)ptrData+uiDataSize);
}

void* VertexBufferObject::getDataPointer()
{
	if(m_bDataUploaded)return NULL;
	return (void*)m_data[0];
}

UINT VertexBufferObject::getBuffer()
{
	return m_uiBuffer;
}
