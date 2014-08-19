#pragma once

namespace framework
{
	class cStage  //그때그때마다 자식클래스들의 형변환을 위한 추상클래스
	{
	public:
		struct tagIngameInfo
		{
			graphic::cCharacter* pCharacter1;
			graphic::cCharacter* pCharacter2;

			graphic::cShader* pShader;

			graphic::cTerrain* pTerrain;
			graphic::cShader* pTerrainShader;

			LPD3DXSPRITE pSprite;

			LPDIRECT3DTEXTURE9 pShadowTex;
			LPDIRECT3DSURFACE9 pShadowSurf;
			LPDIRECT3DSURFACE9 pShadowTexZ;

			graphic::cModel* pSkybox;

			short sWinner;
		};

	//	virtual void Init() = 0;
	//	virtual void Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) = 0;
	//	virtual void Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) = 0;
	//	virtual void Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) = 0;
		virtual void Init(const int nId = 0, tagIngameInfo* sIngameInfo = NULL) = 0;
		virtual void Release() = 0;
		virtual void Input(const float elapseTime) = 0;
		virtual void Update(const float elapseTime) = 0;
		virtual void Render(const float elapseTime) = 0;
	};
}