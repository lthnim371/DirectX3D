#pragma once

namespace framework
{
	class cStage
	{
	public:
	//	virtual void Init() = 0;
	//	virtual void Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) = 0;
	//	virtual void Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) = 0;
	//	virtual void Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) = 0;
		virtual void Init(const int nId = 0) = 0;
		virtual void Input(const float elapseTime) = 0;
		virtual void Update(const float elapseTime) = 0;
		virtual void Render(const float elapseTime) = 0;
	};
}