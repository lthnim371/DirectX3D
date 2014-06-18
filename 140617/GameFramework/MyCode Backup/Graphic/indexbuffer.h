
#pragma once

#include "..\stdafx.h"

class cIndexBuffer
{
public:
	cIndexBuffer();
	~cIndexBuffer();
public:  //¸â¹öÇÔ¼ö
	IDirect3DIndexBuffer9* GetIndexbuff() const;
	int GetIndexSize() const;
	bool Setup(LPDIRECT3DDEVICE9 pdevice, const int& indexSize,
		const DWORD& flag = 0);
	void Release() const;
	void Initial(const std::vector<int>& vertex, const DWORD& flag = 0);
private:  //¸â¹ö°´Ã¼
	IDirect3DIndexBuffer9* m_indexBuffer;
	int m_faceSize;
};