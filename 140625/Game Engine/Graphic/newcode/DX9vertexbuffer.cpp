//140618 작성 //140619 수정

#include "stdafx.h"
#include "DX9vertexbuffer.h"

using namespace graphic;

//디폴트함수
cDX9VertexBuffer::cDX9VertexBuffer()
	: m_pVtxBuff(NULL), m_vertexCount(0), m_sizeOfVertex(0), m_fvf(0)
{}
cDX9VertexBuffer::cDX9VertexBuffer(int vertexCount, int sizeofVertex,
			DWORD fvf, DWORD flag)
{
	Create(vertexCount, sizeofVertex, fvf);
}
cDX9VertexBuffer::~cDX9VertexBuffer()
{
	Clear();
}

//메인 함수
bool cDX9VertexBuffer::Create(int vertexCount, int sizeofVertex,
			DWORD fvf, DWORD flag)
{
	m_vertexCount = vertexCount;
	m_sizeOfVertex = sizeofVertex;
	m_fvf = fvf;
	
	if(FAILED( ::GetDevice()->CreateVertexBuffer( m_vertexCount * m_sizeOfVertex,
		flag, m_fvf, D3DPOOL_MANAGED, &m_pVtxBuff, 0 )))
	{
		return false;
	}

	return true;
}
void* cDX9VertexBuffer::Lock(DWORD flag)
{
	if (!m_pVtxBuff)
		return NULL;

	void* vertices = NULL;
	m_pVtxBuff->Lock( 0, 0, (void**)&vertices, flag );
	
	return vertices;
}
void cDX9VertexBuffer::Unlock()
{
	m_pVtxBuff->Unlock();
}
void cDX9VertexBuffer::Bind() const
{
	::GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, m_sizeOfVertex);
	::GetDevice()->SetFVF(m_fvf);
}
void cDX9VertexBuffer::Clear()
{
	if(m_pVtxBuff)
		m_pVtxBuff->Release();
}