//140618 작성 - 이태훈

#pragma once

#include "..\stdafx.h"

namespace graphic
{
	class cMyLight
	{
	public:  //디폴트함수
		cMyLight();
		~cMyLight();
	public:  //멤버함수
		void InitPointLight(const D3DVECTOR& position, const D3DXCOLOR& color);
		void InitDirectionalLight(const D3DVECTOR& direction, const D3DXCOLOR& color);
		void InitSpotLight(const D3DVECTOR& position, const D3DVECTOR& direction,
			const D3DXCOLOR& color, const float theta, const float phi);

		inline D3DLIGHT9 GetLight() const { return m_light; };
	private:  //멤버객체
		D3DLIGHT9 m_light;
	};
}