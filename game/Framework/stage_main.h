#pragma once

namespace framework
{
	class cStage_Main : public cStage  //처음 메인 스테이지
	{
	public:
		cStage_Main();
		~cStage_Main();

	//	virtual void Init() override;
	//	virtual void Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
		virtual void Init(const int nId) override;  //메인화면 및 버튼 이미지 생성 및 등록
		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;  //메인화면 애니메이션
		
		bool MessageProc( UINT message, WPARAM wParam, LPARAM lParam);  //버튼 클릭 인식

	private:
		LPD3DXSPRITE m_sprite;
		cTestScene* m_scene;  //메인화면 객체

		u_short usCount;  //메인화면 애니메이션을 위한 count
	};
}