
#pragma once

//#include "..\stdafx.h"

class cVertexBuffer
{
public:  //µðÆúÆ®ÇÔ¼ö
	cVertexBuffer();
	~cVertexBuffer();
public:  //¸â¹öÇÔ¼ö
	IDirect3DVertexBuffer9* GetVertexbuff() const;
	int GetVertexSize() const;
	bool Setup(LPDIRECT3DDEVICE9 pdevice, const int& vtxSize,
		const DWORD& FVF, const DWORD& flag = 0);
	void Release() const;
	void Initial(const std::vector<Vertex>& vertex, const DWORD& flag = 0);
private:  //¸â¹ö°´Ã¼
	IDirect3DVertexBuffer9* m_vertexBuffer;
	int m_vtxbuffSize;
};