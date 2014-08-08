#pragma once

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
		virtual void Init(const int nId) override;
		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;

	private:
		POINT m_currMouse;
		POINT m_prevMouse;
//		bool m_bMouse;

		graphic::cCharacter* character1;
		graphic::cCharacter* character2;

		short m_id;
	};
}