#pragma once

namespace framework
{
	class cStage_Main : public cStage
	{
	public:
		cStage_Main();
		~cStage_Main();

	//	virtual void Init() override;
	//	virtual void Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
		virtual void Init(const int nId) override;
		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;
		
		bool MessageProc( UINT message, WPARAM wParam, LPARAM lParam);

	private:
		LPD3DXSPRITE m_sprite;
		cTestScene* m_scene;
	};
}