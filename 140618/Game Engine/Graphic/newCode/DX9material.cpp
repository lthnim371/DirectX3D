//140618 작성 //140619 수정

#include "..\stdafx.h"
#include "DX9material.h"

using namespace graphic;

//디폴트함수
cDX9Material::cDX9Material()
{
	ZeroMemory( &m_material, sizeof(D3DMATERIAL9) );
}
cDX9Material::cDX9Material(const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse,
		const D3DXCOLOR& specular, const D3DXCOLOR& emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
		float spcePow = 0.f)
{
	ZeroMemory( &m_material, sizeof(D3DMATERIAL9) );
	Initial( ambient, diffuse, specular, emissive, spcePow );
}
cDX9Material::~cDX9Material()
{}

//멤버함수
void cDX9Material::Initial(const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse,
		const D3DXCOLOR& specular, const D3DXCOLOR& emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
		float spcePow = 0.f)
{
	m_material.Ambient = ambient;
	m_material.Diffuse = diffuse;
	m_material.Specular = specular;
	m_material.Emissive = emissive;
	m_material.Power = spcePow;
}
void cDX9Material::Bind() const
{
	::GetDevice()->SetMaterial( &m_material );
}
void cDX9Material::SetAmbient(const D3DXCOLOR& ambient)
{
	m_material.Ambient = ambient;
}
void cDX9Material::SetDiffuse(const D3DXCOLOR& diffuse)
{
	m_material.Diffuse = diffuse;
}
void cDX9Material::SetSpecular(const D3DXCOLOR& specular)
{
	m_material.Specular = specular;
}
void cDX9Material::SetEmissive(const D3DXCOLOR& emissive)
{
	m_material.Emissive = emissive;
}
void cDX9Material::SetSpecularPow(float spcePow)
{
	m_material.Power = spcePow;
}