//140619 작성

#include "stdafx.h"
#include "DX9texture.h"

using namespace graphic;

//디폴트함수
cDX9Texture::cDX9Texture()
	: m_pTexture(NULL)
{}
cDX9Texture::~cDX9Texture()
{
	Clear();
}

//멤버함수
bool cDX9Texture::Create(const string& fileName)
{
	if( FAILED( ::D3DXCreateTextureFromFileA( GetDevice(), fileName.c_str(), &m_pTexture )))
		return false;

	return true;
}
void cDX9Texture::SetFilter(int filter, int anisotropicLevel)
{	
	::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, filter);
	::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, filter);

	if( filter == ANISOTROPIC )
		::GetDevice()->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, anisotropicLevel);
}
void cDX9Texture::SetMipmap(int filter)
{
	::GetDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, filter);
}
void cDX9Texture::SetAddressMode(int mode)
{
	::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSU, mode);
	::GetDevice()->SetSamplerState(0, D3DSAMP_ADDRESSV, mode);
}
void cDX9Texture::Bind(int stage)
{
	::GetDevice()->SetTexture(stage, m_pTexture);
}
void cDX9Texture::Clear()
{
	SAFE_RELEASE(m_pTexture);
}