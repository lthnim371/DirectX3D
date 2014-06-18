
#include "indexbuffer.h"

//디폴트함수
cIndexBuffer::cIndexBuffer()
	: m_indexBuffer(0)
{}
cIndexBuffer::~cIndexBuffer()
{
	Release();
}

//메인 함수
IDirect3DIndexBuffer9* cIndexBuffer::GetIndexbuff() const
{
	return m_indexBuffer;
}
int cIndexBuffer::GetIndexSize() const
{
	return m_faceSize;
}
bool cIndexBuffer::Setup(LPDIRECT3DDEVICE9 pdevice, const int& indexSize,
	const DWORD& flag = 0)
{
	m_faceSize = indexSize;
	if(FAILED( pdevice->CreateIndexBuffer( m_faceSize * 3 * sizeof(WORD), flag,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_indexBuffer, 0 )))
	{
		return false;
	}
	return true;
}
void cIndexBuffer::Release() const
{
	m_indexBuffer->Release();
}
void cIndexBuffer::Initial(const std::vector<int>& index, const DWORD& flag = 0)
{
	WORD* indices = NULL;
	m_indexBuffer->Lock( 0, 0, (void**)indices, flag );
	for(int i=0; i<m_faceSize * 3; ++i)
	{
		indices[i] = index[i];
	}
	m_indexBuffer->Unlock();
}