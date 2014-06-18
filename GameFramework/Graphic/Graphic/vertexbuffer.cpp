
#include "vertexbuffer.h"

//디폴트함수
cVertexBuffer::cVertexBuffer()
	: m_vertexBuffer(0)
{}
cVertexBuffer::~cVertexBuffer()
{
	Release();
}

//메인 함수
IDirect3DVertexBuffer9* cVertexBuffer::GetVertexbuff() const
{
	return m_vertexBuffer;
}
int cVertexBuffer::GetVertexSize() const
{
	return m_vtxbuffSize;
}
bool cVertexBuffer::Setup(LPDIRECT3DDEVICE9 pdevice, const int& vtxSize,
	const DWORD& FVF, const DWORD& flag = 0)
{
	m_vtxbuffSize = vtxSize;
	if(FAILED( pdevice->CreateVertexBuffer( m_vtxbuffSize * sizeof(Vertex),
		flag, FVF, D3DPOOL_MANAGED, &m_vertexBuffer, 0 )))
	{
		return false;
	}
	return true;
}
void cVertexBuffer::Release() const
{
	m_vertexBuffer->Release();
}
void cVertexBuffer::Initial(const std::vector<Vertex>& vertex, const DWORD& flag = 0)
{
	Vertex* vertices;
	ZeroMemory( vertices, sizeof(Vertex) );
	m_vertexBuffer->Lock( 0, sizeof( Vertex ), (void**)vertices, flag );
	for(int i=0; i<m_vtxbuffSize; ++i)
	{
		vertices[i] = vertex[i];
	}
	m_vertexBuffer->Unlock();
}