#pragma once

//#include "../Network/Network/network.h"
//using namespace network;

namespace framework
{
	
	class cStage_Ingame : public cStage
	{
	public:
		cStage_Ingame();
		~cStage_Ingame();

	//	virtual void Init() override;
	//	virtual void Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
		virtual void Init(const int nId, tagIngameInfo* sIngameInfo) override;
		virtual void Release() override;
		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;

	protected:
		bool PacketSend(IN network::InfoProtocol& packetInfo);
	//	bool PacketSend(const int nState1, const int nState2, const POINT ptMouse);
	//	bool PacketSend(const network::InfoProtocol packetInfo);
		bool PacketReceive(OUT network::InfoProtocol& packetInfo);
		void LoadMapObject(const string& fileName);
		void ObjectCollisionCheck(const float elapseTime);  //맵오브젝트들과 충돌 확인
		void CharacterCollisionCheck(const float elapseTime);
		void MatchResult(const short sWinner, const bool bResult);  //게임이 끝났는지 확인

	private:
		POINT m_currMouse;
		POINT m_prevMouse;
//		bool m_bMouse;
		float fTick1;
		float fTick2;
		bool m_end;

		graphic::cCharacter* character1;
		graphic::cCharacter* character2;

		
//		bool m_access;
		network::InfoProtocol m_infoSend;
		network::InfoProtocol m_info1;
		network::InfoProtocol m_info2;

		graphic::cShader* m_shader;
		ID3DXFont* m_font;

		graphic::cTerrain* m_terrain;
		graphic::cShader* m_terrainShader;

		LPD3DXSPRITE m_sprite;
		graphic::cSprite* m_hpImage;
		graphic::cSprite* m_spImage;
		graphic::cSprite* m_helpImage;

		LPDIRECT3DTEXTURE9 m_pShadowTex;
		LPDIRECT3DSURFACE9 m_pShadowSurf;
		LPDIRECT3DSURFACE9 m_pShadowTexZ;

		graphic::cModel* m_skybox;

	//test
		bool m_cubeDraw;
		bool m_attackSound;
		bool m_guardLoop;
		bool m_attackLoop;
	};
}