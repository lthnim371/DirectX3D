#pragma once

namespace framework
{
	class cStage_IngameStart : public cStage
	{
	public:
		cStage_IngameStart();
		~cStage_IngameStart();

		virtual void Init(const int nId, tagIngameInfo* sIngameInfo) override;
		virtual void Release() override;
		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;

		void LoadMapObject(const string& fileName);
		
	private:
		short m_id;
		bool m_start;

		graphic::cCharacter* character1;
		graphic::cCharacter* character2;

		graphic::cShader* m_shader;

		graphic::cTerrain* m_terrain;
		graphic::cShader* m_terrainShader;

		LPD3DXSPRITE m_sprite;
		graphic::cSprite* m_image;
	//	graphic::cSprite* m_readyImage;
	//	graphic::cSprite* m_startImage;

		LPDIRECT3DTEXTURE9 m_pShadowTex;
		LPDIRECT3DSURFACE9 m_pShadowSurf;
		LPDIRECT3DSURFACE9 m_pShadowTexZ;

		graphic::cModel* m_skybox;
	};
}