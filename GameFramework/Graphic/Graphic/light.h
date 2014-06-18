//미완성

#pragma once

#include "..\stdafx.h"

class cLight
{
public:  //디폴트함수
	cLight();
	~cLight();
public:  //멤버함수
	void InitPointLight(const D3DVECTOR& position, const D3DXCOLOR& color);
	void InitDirectionalLight(const D3DVECTOR& direction, const D3DXCOLOR& color);
	void InitSpotLight(const D3DVECTOR& position, const D3DVECTOR& direction,
		const D3DXCOLOR& color, const float theta, const float phi);

	inline D3DLIGHT9 GetLight() const { return m_light; };
private:  //멤버객체
	D3DLIGHT9 m_light;
};