//���� ����??

#pragma once

#include "..\stdafx.h"

class cVertexBuffer
{
public:  //����Ʈ�Լ�
	cVertexBuffer();
	~cVertexBuffer();
public:  //����Լ�
	IDirect3DVertexBuffer9* GetVertexbuff() const;
	int GetVertexSize() const;
	bool Setup(LPDIRECT3DDEVICE9 pdevice, const int& vtxSize,
		const DWORD& FVF, const DWORD& flag = 0);
	void Release() const;
	void Initial(const std::vector<Vertex>& vertex, const DWORD& flag = 0);
private:  //�����ü
	IDirect3DVertexBuffer9* m_vertexBuffer;
	int m_vtxbuffSize;
};