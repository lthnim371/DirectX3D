//140618 �ۼ� //140619 ����

#pragma once

//#include "..\stdafx.h"

namespace graphic
{
	class cDX9VertexBuffer
	{
	public:  //����Ʈ�Լ�
		cDX9VertexBuffer();
		//������ ���ÿ� ���� ����
		cDX9VertexBuffer(int vertexCount, int sizeofVertex,
			DWORD fvf, DWORD flag = D3DUSAGE_WRITEONLY);
		~cDX9VertexBuffer();

	public:  //����Լ�
		//���� �� ���� ����
		bool Create(int vertexCount, int sizeofVertex,
			DWORD fvf, DWORD flag = D3DUSAGE_WRITEONLY);

		//���ۿ� ������ ä���
		void* Lock(DWORD flag = 0);
		void Unlock();

		//�������� ����
		void Bind();

		//���� ����
		void Clear();

		//��ü ��ȯ
		inline LPDIRECT3DVERTEXBUFFER9 GetVtxbuff() const { return m_pVtxBuff; };
		inline int GetVertexCount() const { return m_vertexCount; };
		inline int GetSizeOfVertex() const { return m_sizeOfVertex; };
		inline int GetFVF() const { return m_fvf; };
			
	private:  //�����ü
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		int m_vertexCount;
		int m_sizeOfVertex;
		DWORD m_fvf;
	};
}