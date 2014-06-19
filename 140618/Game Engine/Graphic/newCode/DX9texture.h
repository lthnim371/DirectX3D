//140619 �ۼ�

#pragma once

//#include "..\stdafx.h"

namespace graphic
{
	class cDX9Texture
	{
		enum TYPE
		{
			// for Filter
			POINT = 1, LINEAR, ANISOTROPIC,
			// for Addresmode
			WRAP = 1, MIRROR, CLAMP, BORDER,
		};

	public:  //����Ʈ�Լ�
		cDX9Texture();
		~cDX9Texture();

	public:  //����Լ�
		//�ؽ��� �о����
		bool Create(const string& fileName);

		//�ɼ� ����
		void SetFilter(int filter = LINEAR, int anisotropicLevel = 0);
		void SetMipmap(int filter = POINT);
		void SetAddressMode(int mode);

		//�ؽ��� ����
		void Bind(int stage);

		//�ؽ��İ�ü ����
		void Clear();

		//��ü ��ȯ
		inline IDirect3DTexture9* GetTexture() const { return m_pTexture; };

	private:  //�����ü
		LPDIRECT3DTEXTURE9 m_pTexture;
	};
}