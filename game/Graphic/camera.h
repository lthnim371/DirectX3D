#pragma once

//#include "graphic.h"  //������ ������...�׽�Ʈ �Ŀ� �ּ� ó�� �ٶ�

namespace graphic
{
	class cCamera : public cSingleton<cCamera>
	{
	public:  //friend��??
		cCamera();
		~cCamera();
		
		void Update();
		int Move(const Matrix44& character);

		void SetTranslation(const float step);
		void SetRotation(const Matrix44& character, const float x = 0.f, const float y = 0.f);
		void SetView();
		void SetPosition(const Vector3& characterPos);
				
		Vector3 GetPosition() const;
		Vector3 GetDirection() const;

	private:
		Vector3 m_lookPos;
		Vector3 m_pos;
		Vector3 m_look;
		Vector3 m_up;
		Vector3 m_right;
		Vector3 m_dir;
		Vector3 m_test;
	};

	inline cCamera* GetCamera() { return cCamera::Get(); };

	inline Vector3 cCamera::GetPosition() const { return m_pos; };
	inline Vector3 cCamera::GetDirection() const { return Vector3(m_dir.x, 0.f, m_dir.z); };
}