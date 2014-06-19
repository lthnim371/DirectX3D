//140618 �ۼ� //140619 ����

#include "..\stdafx.h"
#include "DX9vertexbuffer.h"

using namespace graphic;

//����Ʈ�Լ�
cDX9VertexBuffer::cDX9VertexBuffer()
	: m_pVtxBuff(NULL), m_vertexCount(0), m_sizeOfVertex(0), m_fvf(0)
{}
cDX9VertexBuffer::cDX9VertexBuffer(int vertexCount, int sizeofVertex,
			DWORD fvf, DWORD flag = D3DUSAGE_WRITEONLY)
{
	Create(vertexCount, sizeofVertex, fvf);
}
cDX9VertexBuffer::~cDX9VertexBuffer()
{
	Clear();
}

//���� �Լ�
bool cDX9VertexBuffer::Create(int vertexCount, int sizeofVertex,
			DWORD fvf, DWORD flag = D3DUSAGE_WRITEONLY)
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
void* cDX9VertexBuffer::Lock(DWORD flag = 0)
{
	if (!m_pVtxBuff)
		return NULL;

	void* vertices;
	m_pVtxBuff->Lock( 0, 0, (void**)&vertices, flag );
	
	return vertices;
}
void cDX9VertexBuffer::Unlock()
{
	m_pVtxBuff->Unlock();
}
void cDX9VertexBuffer::Bind()
{
	::GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, m_sizeOfVertex);
	::GetDevice()->SetFVF(m_fvf);
}
void cDX9VertexBuffer::Clear()
{
	m_pVtxBuff->Release();
}