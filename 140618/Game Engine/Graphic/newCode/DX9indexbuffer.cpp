//140618 �ۼ� //140619 ����

#include "..\stdafx.h"
#include "DX9indexbuffer.h"

using namespace graphic;

//����Ʈ�Լ�
cDX9IndexBuffer::cDX9IndexBuffer()
	: m_pIdxBuff(NULL), m_faceCount(0)
{}
cDX9IndexBuffer::~cDX9IndexBuffer()
{
	Clear();
}

//���� �Լ�
bool cDX9IndexBuffer::Create(int indexCount, DWORD flag = 0)
{
	m_faceCount = indexCount;

	if( FAILED( ::GetDevice()->CreateIndexBuffer( m_faceCount * 3 * sizeof(WORD),
		flag, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, 0 )))
	{
		return false;
	}

	return true;
}

WORD* cDX9IndexBuffer::Lock(DWORD flag = 0)
{
	WORD* indices = NULL;

	m_pIdxBuff->Lock( 0, 0, (void**)&indices, flag );

	return indices;
	
}
void cDX9IndexBuffer::UnLock()
{
	m_pIdxBuff->Unlock();
}

void cDX9IndexBuffer::Bind()
{
	::GetDevice()->SetIndices(m_pIdxBuff);
}

void cDX9IndexBuffer::Clear()
{
	m_pIdxBuff->Release();
}