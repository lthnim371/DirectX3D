/*  추후 업데이트 사항
카메라 상하 움직임
*/

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
		void Render(const int hp, const int sp);

		void SetPosition(const Matrix44& pos);  //카메라 look, pos 갱신
		void SetTranslation(const Vector3& pos);  //카메라 look, pos 이동
//		void SetTranslation(const float z);
//		void SetRotation(const POINT& ptMouse);  //카메라 회전 연산
		void SetRotation(const float x = 0, const float y = 0);  //카메라 회전 연산
		void SetView();
		void SetHeight(const float number);  //프로그램 테스트용
		
		Vector3 GetPosition() const;
		Vector3 GetDirection() const;
		Vector3 GetRight() const;
		Vector3 GetLook() const;

	private:
		Vector3 m_pos;
		Vector3 m_look;
		Vector3 m_up;
		Vector3 m_right;
		Vector3 m_dir;
		ID3DXFont* m_font;
	};

//	inline void  cCamera::SetHeight(const float number) { m_pos.y += number; }

	inline cCamera* GetCamera() { return cCamera::Get(); }  //graphic의 함수. this(camera)
	inline Vector3 cCamera::GetPosition() const { return m_pos; }
	inline Vector3 cCamera::GetDirection() const { return m_dir; }
	inline Vector3 cCamera::GetRight() const { return m_right; }
	inline Vector3 cCamera::GetLook() const { return m_look; }

}