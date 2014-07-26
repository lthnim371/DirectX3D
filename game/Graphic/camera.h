#pragma once

//#include "graphic.h"  //빨간줄 방지용...테스트 후에 주석 처리 바람

namespace graphic
{
	class cCamera : public cSingleton<cCamera>
	{
	public:  //friend로??
		cCamera();
		~cCamera();
		
		void Update();  //카메라 방향벡터, 우방벡터 업데이트

		void SetTranslation(const Matrix44& character);  //카메라 look, 위치 갱신
		void SetRotation(const POINT& ptMouse);  //카메라 회전 연산
		void SetView();
		
		Vector3 GetPosition() const;
		Vector3 GetDirection() const;
		Vector3 GetRight() const;

	private:
		Vector3 m_pos;
		Vector3 m_look;
		Vector3 m_up;
		Vector3 m_right;
		Vector3 m_dir;
	};

	inline cCamera* GetCamera() { return cCamera::Get(); };  //this
	inline Vector3 cCamera::GetPosition() const { return m_pos; };
	inline Vector3 cCamera::GetDirection() const { return m_dir; };
	inline Vector3 cCamera::GetRight() const { return m_right; };

}

/*  backup
//		int Move(const Matrix44& character);

//		void SetTranslation(const Matrix44& character);
//		void SetTranslation(const float step);
//		void SetRotation(const float x = 0.f, const float y = 0.f);
//		void SetPosition(const Vector3& characterPos);

//		float GetRotation();
//		POINT GetRotation() const;

//	inline Vector3 cCamera::GetDirection() const { return m_dir; };
//	inline Matrix44 cCamera::GetRotation() const { return m_rot; };
//	inline POINT cCamera::GetRotation() const { return m_ptRot; };

//		Vector3 m_lookPos;  //= m_dir
//		Matrix44 m_rot;
//		float m_rot;
*/