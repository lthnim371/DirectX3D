#pragma once

namespace framework
{
	class cStage  //그때그때마다 자식클래스들의 형변환을 위한 추상클래스
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
		virtual void Release() = 0;
	};
}