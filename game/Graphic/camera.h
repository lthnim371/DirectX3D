#pragma once

//#include "graphic.h"  //빨간줄 방지용...테스트 후에 주석 처리 바람

namespace graphic
{
	class cCamera : public cSingleton<cCamera>
	{
	public:  //friend로??
		cCamera();
		~cCamera();
		
		void Update();
		bool Move();

		void SetTranslation(const float step);
		void SetRotation(const float x = 0.f, const float y = 0.f);
		void SetView();
	
		Vector3 GetPosition() const;

	private:
		Vector3 m_pos;
		Vector3 m_look;
		Vector3 m_up;
		Vector3 m_right;
		Vector3 m_dir;
	};

	inline cCamera* GetCamera() { return cCamera::Get(); };

	inline Vector3 cCamera::GetPosition() const { return m_pos; };
}