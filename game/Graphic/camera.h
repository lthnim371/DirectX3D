#pragma once

//#include "graphic.h"  //빨간줄 방지용...테스트 후에 주석 처리 바람

namespace graphic
{
	class cCamera : public cSingleton<cCamera>
	{
	public:  //friend로??
		cCamera();
		~cCamera();

		void SetTranslation(const Vector3& position);
		void SetRotation(const float x_axis = 0.f, const float y_axis = 0.f);
		void Update();

	private:
		Vector3 m_pos;
		Vector3 m_look;
		Vector3 m_up;
		Vector3 m_right;
		Vector3 m_dir;
	};

	inline cCamera* GetCamera() { return cCamera::Get(); };
}