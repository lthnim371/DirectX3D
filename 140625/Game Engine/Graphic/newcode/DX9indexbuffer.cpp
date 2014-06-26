//140618 작성 //140619 수정

#include "stdafx.h"
#include "DX9indexbuffer.h"

using namespace graphic;

//디폴트함수
cDX9IndexBuffer::cDX9IndexBuffer()
	: m_pIdxBuff(NULL), m_faceCount(0)
{}
cDX9IndexBuffer::~cDX9IndexBuffer()
{
	Clear();
}

//메인 함수
bool cDX9IndexBuffer::Create(int indexCount, DWORD flag)
{
	m_faceCount = indexCount;

	if( FAILED( ::GetDevice()->CreateIndexBuffer( m_faceCount * 3 * sizeof(WORD),
		flag, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, 0 )))
	{
		return false;
	}

	return true;
}

WORD* cDX9IndexBuffer::Lock(DWORD flag)
{
	WORD* indices = NULL;

	m_pIdxBuff->Lock( 0, 0, (void**)&indices, flag );

	return indices;
	
}
void cDX9IndexBuffer::Unlock()
{
	m_pIdxBuff->Unlock();
}

void cDX9IndexBuffer::Bind() const
{
	::GetDevice()->SetIndices(m_pIdxBuff);
}

void cDX9IndexBuffer::Clear()
{
	if(m_pIdxBuff)
		m_pIdxBuff->Release();
}