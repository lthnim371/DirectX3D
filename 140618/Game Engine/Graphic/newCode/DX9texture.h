//140619 작성

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

	public:  //디폴트함수
		cDX9Texture();
		~cDX9Texture();

	public:  //멤버함수
		//텍스쳐 읽어오기
		bool Create(const string& fileName);

		//옵션 설정
		void SetFilter(int filter = LINEAR, int anisotropicLevel = 0);
		void SetMipmap(int filter = POINT);
		void SetAddressMode(int mode);

		//텍스쳐 설정
		void Bind(int stage);

		//텍스쳐객체 해제
		void Clear();

		//객체 반환
		inline IDirect3DTexture9* GetTexture() const { return m_pTexture; };

	private:  //멤버객체
		LPDIRECT3DTEXTURE9 m_pTexture;
	};
}