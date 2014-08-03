/*  ���� ������Ʈ ����
ī�޶� ���� ������
*/

#pragma once

//#include "graphic.h"  //������ ������...�׽�Ʈ �Ŀ� �ּ� ó�� �ٶ�

namespace graphic
{
	class cCamera : public cSingleton<cCamera>
	{
	public:  //friend��??
		cCamera();
		~cCamera();
		
		void Update();  //ī�޶� ���⺤��, ��溤�� ������Ʈ

		void SetPosition(const Matrix44& pos);  //ī�޶� look, pos ����
		void SetTranslation(const Vector3& pos);  //ī�޶� look, pos �̵�
//		void SetTranslation(const float z);
//		void SetRotation(const POINT& ptMouse);  //ī�޶� ȸ�� ����
		void SetRotation(const float x = 0, const float y = 0);  //ī�޶� ȸ�� ����
		void SetView();
		void SetHeight(const float number);  //���α׷� �׽�Ʈ��
		
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
	};

//	inline void  cCamera::SetHeight(const float number) { m_pos.y += number; }

	inline cCamera* GetCamera() { return cCamera::Get(); }  //graphic�� �Լ�. this(camera)
	inline Vector3 cCamera::GetPosition() const { return m_pos; }
	inline Vector3 cCamera::GetDirection() const { return m_dir; }
	inline Vector3 cCamera::GetRight() const { return m_right; }
	inline Vector3 cCamera::GetLook() const { return m_look; }

}