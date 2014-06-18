//140618 작성 - 이태훈

#pragma once

#include "..\stdafx.h"

namespace graphic  //graphic 지역안에 한데 모아 관리하기 편하게
{
	class cMyMaterial
	{
	public:  //디폴트함수
		cMyMaterial();

		//생성과 동시에 초기화
		cMyMaterial(const D3DXCOLOR ambient, const D3DXCOLOR diffuse,
			const D3DXCOLOR specular, const D3DXCOLOR emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
			const float spcePow = 0.f);

		~cMyMaterial();
	
	public:  //멤버함수
		//생성 후 초기화
		void Initial(const D3DXCOLOR ambient, const D3DXCOLOR diffuse,
			const D3DXCOLOR specular, const D3DXCOLOR emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
			const float spcePow = 0.f);		

		//개별적으로 초기화 할 수 있게끔 준비하였음
		void SetAmbient(const D3DXCOLOR ambient);
		void SetDiffuse(const D3DXCOLOR diffuse);
		void SetSpecular(const D3DXCOLOR specular);
		void SetEmissive(const D3DXCOLOR emissive);
		void SetSpecularPow(const float spcePow);
		
		//디바이스 메트리얼 셋팅
		void Bind() const;

		//자기자신(메트리얼) 반환
		inline D3DMATERIAL9 GetMaterial() const { return m_material; };
			
	private:  //멤버객체
		D3DMATERIAL9 m_material;
	};
}	