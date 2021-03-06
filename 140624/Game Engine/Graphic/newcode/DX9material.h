//140618 작성 //140619 수정

#pragma once

//#include "..\stdafx.h"

namespace graphic  //graphic 지역안에 한데 모아 관리하기 편하게
{
	class cDX9Material
	{
	public:  //디폴트함수
		cDX9Material();

		//생성과 동시에 초기화
		cDX9Material(const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse,
			const D3DXCOLOR& specular, const D3DXCOLOR& emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
			float spcePow = 0.f);

		~cDX9Material();
	
	public:  //멤버함수
		//생성 후 초기화
		void Initial(const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse,
			const D3DXCOLOR& specular, const D3DXCOLOR& emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
			float spcePow = 0.f);		

		//개별적으로 변수 초기화 및 수정
		void SetAmbient(const D3DXCOLOR& ambient);
		void SetDiffuse(const D3DXCOLOR& diffuse);
		void SetSpecular(const D3DXCOLOR& specular);
		void SetEmissive(const D3DXCOLOR& emissive);
		void SetSpecularPow(float spcePow);
		
		//재질 설정
		void Bind() const;

		//객체 반환
		inline D3DMATERIAL9 GetMaterial() const { return m_material; };
			
	private:  //멤버객체
		D3DMATERIAL9 m_material;
	};
}	