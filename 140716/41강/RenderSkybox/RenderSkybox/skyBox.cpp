
#include <windows.h>
#include <string>
#include <fstream>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../math/Math.h"
#include "../../base/base.h"
#include "../../etc/Utility.h"
#include "skyBox.h"

using namespace graphic;


cSkyBox::cSkyBox()
{
}

cSkyBox::~cSkyBox()
{

}


//------------------------------------------------------------------------
// textureFilePath : 이 파일 경로에 skybox_top, skybox_front, skybox_back, 
//				skybox_left, skybox_right, skybox_bottom.jpg 파일이 있어야 한다.
//------------------------------------------------------------------------
bool cSkyBox::Create(const string &textureFilePath)
{
	string textureFileName[] = 
	{
		"skybox_front.jpg", "skybox_back.jpg", "skybox_left.jpg", 
		"skybox_right.jpg", "skybox_top.jpg", "skybox_bottom.jpg"
	};

	for (int i=0; i < MAX_FACE; ++i)
	{
		string fileName = textureFilePath + "/" + textureFileName[ i];
		m_textures[ i].Create( fileName );
	}

	if (!CreateVertexBuffer())
		return false;

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
bool  cSkyBox::CreateVertexBuffer()
{
	// Example diagram of "front" quad
	// The numbers are vertices
	// 
	// 2  __________ 4
	//   |\         |
	//	 |  \       |
	//   |    \     |
	//   |      \   |
	// 1 |        \ | 3
	//	  ----------	 
	const float size = 300;
	sVertexTex SkyboxMesh[24] =
	{
		// Front quad, NOTE: All quads face inward
		sVertexTex(-size, -size,  size,  0.0f, 1.0f ),
		sVertexTex(-size,  size,  size,  0.0f, 0.0f ),
		sVertexTex( size, -size,  size,  1.0f, 1.0f ),
		sVertexTex( size,  size,  size,  1.0f, 0.0f ),

		// Back quad
		sVertexTex( size, -size, -size,  0.0f, 1.0f ),
		sVertexTex( size,  size, -size,  0.0f, 0.0f ),
		sVertexTex(-size, -size, -size,  1.0f, 1.0f ),
		sVertexTex(-size,  size, -size,  1.0f, 0.0f ),

		// Left quad
		sVertexTex(-size, -size, -size,  0.0f, 1.0f ),
		sVertexTex(-size,  size, -size,  0.0f, 0.0f ),
		sVertexTex(-size, -size,  size,  1.0f, 1.0f ),
		sVertexTex(-size,  size,  size,  1.0f, 0.0f ),

		// Right quad
		sVertexTex( size, -size,  size,  0.0f, 1.0f ),
		sVertexTex( size,  size,  size,  0.0f, 0.0f ),
		sVertexTex( size, -size, -size,  1.0f, 1.0f ),
		sVertexTex( size,  size, -size,  1.0f, 0.0f ),

		// Top quad
		sVertexTex(-size,  size,  size,  0.0f, 1.0f ),
		sVertexTex(-size,  size, -size,  0.0f, 0.0f ),
		sVertexTex( size,  size,  size,  1.0f, 1.0f ),
		sVertexTex( size,  size, -size,  1.0f, 0.0f ),

		// Bottom quad
		sVertexTex(-size, -size, -size,  0.0f, 1.0f ),
		sVertexTex(-size, -size,  size,  0.0f, 0.0f ),
		sVertexTex( size, -size, -size,  1.0f, 1.0f ),
		sVertexTex( size, -size,  size,  1.0f, 0.0f ),
	};

	const int vtxSize = 24;
	m_vtxBuff.Create( vtxSize, sizeof(sVertexTex), sVertexTex::FVF);

	sVertexTex *pv = (sVertexTex*)m_vtxBuff.Lock();
    memcpy( pv, SkyboxMesh, sizeof(sVertexTex) * 24 );
	m_vtxBuff.Unlock();

	return true;
}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void cSkyBox::SetRenderState()
{
	GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	GetDevice()->SetRenderState( D3DRS_FOGENABLE, FALSE );

	GetDevice()->SetSamplerState( 0, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP );
	GetDevice()->SetSamplerState( 0, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP );

}


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------
void cSkyBox::Render()
{
	SetRenderState();

	//mat matView, matViewSave, matWorld;
	Matrix44 matView, matViewSave, matWorld;
	GetDevice()->GetTransform( D3DTS_VIEW, (D3DXMATRIX*)&matViewSave );
	matView = matViewSave;  //카메라 행렬 가져오기
	matView._41 = 0.0f; matView._42 = -0.4f; matView._43 = 0.0f;  //카메라 위치 값 빼기(-0.4는 공식이라 그런듯...?)
	GetDevice()->SetTransform( D3DTS_VIEW, (D3DXMATRIX*)&matView );
	// Set a default world matrix
	Matrix44 mat;  //그냥 단위행렬 넣어주었음
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&mat);

	//스카이박스는 카메라 이동은 무시하지만 회전은 적용됨

	// render
	m_vtxBuff.Bind();
	for (int i = 0 ; i < MAX_FACE; i++)
	{
		m_textures[ i].Bind(0);
		GetDevice()->DrawPrimitive( D3DPT_TRIANGLESTRIP, i*4, 2 );  //D3DPT_TRIANGLESTRIP : 트라이앵글 필요없이 그냥 출력. 점4개(사각형)로 출력하는데 인덱스 순서는 중요하다.
		//i*4 : 이 예제에서는 면 하나당 점이 4개이므로
	}

	GetDevice()->SetTransform( D3DTS_VIEW, (D3DXMATRIX*)&matViewSave );
}

