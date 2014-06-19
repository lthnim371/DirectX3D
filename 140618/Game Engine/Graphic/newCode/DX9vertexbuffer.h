//140618 작성 //140619 수정

#pragma once

//#include "..\stdafx.h"

namespace graphic
{
	class cDX9VertexBuffer
	{
	public:  //디폴트함수
		cDX9VertexBuffer();
		//생성과 동시에 버퍼 생성
		cDX9VertexBuffer(int vertexCount, int sizeofVertex,
			DWORD fvf, DWORD flag = D3DUSAGE_WRITEONLY);
		~cDX9VertexBuffer();

	public:  //멤버함수
		//생성 후 버퍼 생성
		bool Create(int vertexCount, int sizeofVertex,
			DWORD fvf, DWORD flag = D3DUSAGE_WRITEONLY);

		//버퍼에 데이터 채우기
		void* Lock(DWORD flag = 0);
		void Unlock();

		//정점버퍼 설정
		void Bind();

		//버퍼 해제
		void Clear();

		//객체 반환
		inline LPDIRECT3DVERTEXBUFFER9 GetVtxbuff() const { return m_pVtxBuff; };
		inline int GetVertexCount() const { return m_vertexCount; };
		inline int GetSizeOfVertex() const { return m_sizeOfVertex; };
		inline int GetFVF() const { return m_fvf; };
			
	private:  //멤버객체
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		int m_vertexCount;
		int m_sizeOfVertex;
		DWORD m_fvf;
	};
}