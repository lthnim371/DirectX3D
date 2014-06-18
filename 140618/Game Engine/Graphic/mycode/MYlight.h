//140618 �ۼ� - ������

#pragma once

#include "..\stdafx.h"

namespace graphic
{
	class cMyLight
	{
	public:  //����Ʈ�Լ�
		cMyLight();
		~cMyLight();
	public:  //����Լ�
		void InitPointLight(const D3DVECTOR& position, const D3DXCOLOR& color);
		void InitDirectionalLight(const D3DVECTOR& direction, const D3DXCOLOR& color);
		void InitSpotLight(const D3DVECTOR& position, const D3DVECTOR& direction,
			const D3DXCOLOR& color, const float theta, const float phi);

		inline D3DLIGHT9 GetLight() const { return m_light; };
	private:  //�����ü
		D3DLIGHT9 m_light;
	};
}