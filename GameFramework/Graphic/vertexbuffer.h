
#pragma once

#include "graphic.h"

class cVertexBuffer
{
public:
	cVertexBuffer();
	~cVertexBuffer();
public:
	void Setup(UINT length, DWORD FVF, DWORD flag = 0);
private:
	IDirect3DVertexBuffer9* m_vertexBuffer;
};