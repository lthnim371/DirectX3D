#pragma once

namespace framework
{
	class cStage_IngameEnd : public cStage
	{
	public:
		cStage_IngameEnd();
		~cStage_IngameEnd();

		virtual void Init(const int nId, tagIngameInfo* sIngameInfo) override;
		virtual void Release() override;
		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;
		
	private:
		short m_user;
		Vector3 m_camDirOriginal;

		graphic::cCharacter* character1;
		graphic::cCharacter* character2;

		graphic::cShader* m_shader;
		ID3DXFont* m_font;

		graphic::cTerrain* m_terrain;
		graphic::cShader* m_terrainShader;

		LPD3DXSPRITE m_sprite;
		graphic::cSprite* m_hpImage;
		graphic::cSprite* m_spImage;

		LPDIRECT3DTEXTURE9 m_pShadowTex;
		LPDIRECT3DSURFACE9 m_pShadowSurf;
		LPDIRECT3DSURFACE9 m_pShadowTexZ;

		graphic::cModel* m_skybox;
	};
}