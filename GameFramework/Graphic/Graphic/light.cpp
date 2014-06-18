
#include "light.h"

//디폴트함수
cLight::cLight()
{
	ZeroMemory( &m_light, sizeof(D3DLIGHT9) );
}
cLight::~cLight()
{
}

//멤버함수
void cLight::InitPointLight(const D3DVECTOR& position, const D3DXCOLOR& color)
{
	m_light.Type = D3DLIGHT_POINT;
	m_light.Ambient = color * 0.4f;
	m_light.Diffuse = color;
	m_light.Specular = color * 0.6f;
	m_light.Position = position;
	m_light.Range = 1000.f;
	m_light.Attenuation0 = 1.f;
	m_light.Attenuation1 = 0.f;
	m_light.Attenuation2 = 0.f;
}
void cLight::InitDirectionalLight(const D3DVECTOR& direction, const D3DXCOLOR& color)
{
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Ambient = color * 0.4f;
	m_light.Diffuse = color;
	m_light.Specular = color * 0.6f;
	m_light.Direction = direction;
}
void cLight::InitSpotLight(const D3DVECTOR& position, const D3DVECTOR& direction,
		const D3DXCOLOR& color, const float theta, const float phi)
{
	m_light.Type = D3DLIGHT_SPOT;
	m_light.Ambient = color * 0.4f;
	m_light.Diffuse = color;
	m_light.Specular = color * 0.6f;
	m_light.Position = position;
	m_light.Direction = direction;
	m_light.Range = 1000.f;
	m_light.Falloff = 1.f;
	m_light.Attenuation0 = 1.f;
	m_light.Attenuation1 = 0.f;
	m_light.Attenuation2 = 0.f;
	m_light.Theta = theta;
	m_light.Phi = phi;
}