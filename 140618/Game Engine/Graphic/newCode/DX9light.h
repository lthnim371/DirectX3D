//140618 작성 //140619 수정

#pragma once

//#include "..\stdafx.h"

namespace graphic
{
	class cDX9Light
	{
	public:  //디폴트함수
		cDX9Light();
		~cDX9Light();

	public:  //멤버함수
		//생성 후 초기화
		void InitPointLight(const Vector3& position,
			const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular);
		void InitDirectionalLight(const Vector3& direction,
			const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular);
		void InitSpotLight(const Vector3& position, const Vector3& direction,
			const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular,
			float theta, float phi);

		//개별적으로 변수 초기화 및 수정
		void SetDirection(const Vector3& direction);
		void SetColor(const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular);
		
		//광원 설정
		void Bind(int lightIndex = 0);
		
		//광원 전원
		void LightOn(int lightIndex = 0);
		void LightOff(int lightIndex = 0);

		//객체 반환
		inline D3DLIGHT9 GetLight() const { return m_light; };

	private:  //멤버객체
		D3DLIGHT9 m_light;
	};
}