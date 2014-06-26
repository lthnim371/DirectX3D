//140618 �ۼ� //140619 ����

#pragma once

//#include "..\stdafx.h"

namespace graphic
{
	class cDX9IndexBuffer
	{
	public:
		cDX9IndexBuffer();
		~cDX9IndexBuffer();
	public:  //����Լ�
		//���� �� ���� ����
		bool Create(int indexCount, DWORD flag = 0);

		//���ۿ� ������ ä���
		WORD* Lock(DWORD flag = 0);
		void Unlock();

		//�ε��� ����
		void Bind() const;

		//���� ����
		void Clear();
		
		//��ü ����
		inline void SetFaceCount(int faceCount) { m_faceCount = faceCount; };
		//��ü ��ȯ
		inline LPDIRECT3DINDEXBUFFER9 GetIndexbuff() const { return m_pIdxBuff; };
		inline int GetFaceCount() const { return m_faceCount; };
			
	private:  //�����ü
		LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
		int m_faceCount;
	};
}