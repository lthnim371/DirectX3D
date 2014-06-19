//140618 작성 //140619 수정

#pragma once

//#include "..\stdafx.h"

namespace graphic
{
	class cDX9IndexBuffer
	{
	public:
		cDX9IndexBuffer();
		~cDX9IndexBuffer();
	public:  //멤버함수
		//생성 후 버퍼 생성
		bool Create(int indexCount, DWORD flag = 0);

		//버퍼에 데이터 채우기
		WORD* Lock(DWORD flag = 0);
		void UnLock();

		//인덱스 설정
		void Bind();

		//버퍼 해제
		void Clear();
		
		//객체 반환
		inline LPDIRECT3DINDEXBUFFER9 GetIndexbuff() const { return m_pIdxBuff; };
		inline int GetFaceCount() const { return m_faceCount; };
			
	private:  //멤버객체
		LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
		int m_faceCount;
	};
}