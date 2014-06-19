//140618 작성 //140619 수정

#include "..\stdafx.h"
#include "DX9light.h"

using namespace graphic;

//디폴트함수
cDX9Light::cDX9Light()
{
	ZeroMemory( &m_light, sizeof(D3DLIGHT9) );
}
cDX9Light::~cDX9Light()
{
}

//멤버함수
void cDX9Light::InitPointLight(const Vector3& position,
	const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular)
{
	m_light.Type = D3DLIGHT_POINT;
	m_light.Ambient = ambient;
	m_light.Diffuse = diffuse;
	m_light.Specular = specular;
	m_light.Position = *(D3DVECTOR*)&position;
	m_light.Range = 1000.f;
	m_light.Attenuation0 = 1.f;
	m_light.Attenuation1 = 0.f;
	m_light.Attenuation2 = 0.f;
}
void cDX9Light::InitDirectionalLight(const Vector3& direction,
	const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular)
{
	m_light.Type = D3DLIGHT_DIRECTIONAL;
	m_light.Ambient = ambient;
	m_light.Diffuse = diffuse;
	m_light.Specular = specular;
	m_light.Direction = *(D3DVECTOR*)&direction;
}
void cDX9Light::InitSpotLight(const Vector3& position, const Vector3& direction,
		const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular,
		float theta, float phi)
{
	m_light.Type = D3DLIGHT_SPOT;
	m_light.Ambient = ambient;
	m_light.Diffuse = diffuse;
	m_light.Specular = specular;
	m_light.Position = *(D3DVECTOR*)&position;
	m_light.Direction = *(D3DVECTOR*)&direction;
	m_light.Range = 1000.f;
	m_light.Falloff = 1.f;
	m_light.Attenuation0 = 1.f;
	m_light.Attenuation1 = 0.f;
	m_light.Attenuation2 = 0.f;
	m_light.Theta = theta;
	m_light.Phi = phi;
}
void cDX9Light::SetColor(const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular)
{
	m_light.Ambient = ambient;
	m_light.Diffuse = diffuse;
	m_light.Specular = specular;
}
void cDX9Light::SetDirection(const Vector3& direction)
{
	m_light.Direction = *(D3DVECTOR*)&direction;
}
void cDX9Light::Bind(int lightIndex = 0)
{
	::GetDevice()->SetLight( lightIndex, &m_light );
}
void cDX9Light::LightOn(int lightIndex = 0)
{
	::GetDevice()->LightEnable(lightIndex, true);
}
void cDX9Light::LightOff(int lightIndex = 0)
{
	::GetDevice()->LightEnable(lightIndex, false);
}