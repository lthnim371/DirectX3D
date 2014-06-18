//140618 작성 - 이태훈

#include "..\stdafx.h"
#include "MYmaterial.h"

using namespace graphic;

//디폴트함수
cMyMaterial::cMyMaterial()
{
	ZeroMemory( &m_material, sizeof(D3DMATERIAL9) );
}
cMyMaterial::cMyMaterial(const D3DXCOLOR ambient, const D3DXCOLOR diffuse,
		const D3DXCOLOR specular, const D3DXCOLOR emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
		const float spcePow = 0.f)
{
	ZeroMemory( &m_material, sizeof(D3DMATERIAL9) );
	Initial( ambient, diffuse, specular, emissive, spcePow );
}
cMyMaterial::~cMyMaterial()
{}

//멤버함수
void cMyMaterial::Initial(const D3DXCOLOR ambient, const D3DXCOLOR diffuse,
		const D3DXCOLOR specular, const D3DXCOLOR emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
		const float spcePow = 0.f)
{
	m_material.Ambient = ambient;
	m_material.Diffuse = diffuse;
	m_material.Specular = specular;
	m_material.Emissive = emissive;
	m_material.Power = spcePow;
}
void cMyMaterial::Bind() const
{
	GetDevice()->SetMaterial( &m_material );
}
void cMyMaterial::SetAmbient(const D3DXCOLOR ambient)
{
	m_material.Ambient = ambient;
}
void cMyMaterial::SetDiffuse(const D3DXCOLOR diffuse)
{
	m_material.Diffuse = diffuse;
}
void cMyMaterial::SetSpecular(const D3DXCOLOR specular)
{
	m_material.Specular = specular;
}
void cMyMaterial::SetEmissive(const D3DXCOLOR emissive)
{
	m_material.Emissive = emissive;
}
void cMyMaterial::SetSpecularPow(const float spcePow)
{
	m_material.Power = spcePow;
}