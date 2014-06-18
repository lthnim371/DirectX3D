
#include "material.h"

//디폴트함수
cMaterial::cMaterial()
{
	ZeroMemory( &m_material, sizeof(D3DMATERIAL9) );
}
cMaterial::cMaterial(const D3DXCOLOR ambient, const D3DXCOLOR diffuse,
		const D3DXCOLOR specular, const float specScale)
{
	ZeroMemory( &m_material, sizeof(D3DMATERIAL9) );
	Initial( ambient, diffuse, specular, specScale );
}
cMaterial::~cMaterial()
{}

//멤버함수
void cMaterial::Initial(const D3DXCOLOR ambient, const D3DXCOLOR diffuse,
		const D3DXCOLOR specular, const float specScale)
{
	m_material.Ambient = ambient;
	m_material.Diffuse = diffuse;
	m_material.Specular = specular;
	m_material.Emissive = D3DXCOLOR( 0.f, 0.f, 0.f, 0.f );
	m_material.Power = specScale;
}
void cMaterial::SetAmbient(const D3DXCOLOR ambient)
{
	m_material.Ambient = ambient;
}
void cMaterial::SetDiffuse(const D3DXCOLOR diffuse)
{
	m_material.Diffuse = diffuse;
}
void cMaterial::SetSpecular(const D3DXCOLOR specular)
{
	m_material.Specular = specular;
}
void cMaterial::SetEmissive(const D3DXCOLOR emissive)
{
	m_material.Emissive = emissive;
}
void cMaterial::SetSpecularScale(const float specScale)
{
	m_material.Power = specScale;
}