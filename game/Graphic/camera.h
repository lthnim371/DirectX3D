/*  ���� ������Ʈ ����
ī�޶� ���� ������
*/

#pragma once

//#include "graphic.h"  //������ ������...�׽�Ʈ �Ŀ� �ּ� ó�� �ٶ�

namespace graphic
{
	class cCamera// : public cSingleton<cCamera>
	{
	public:  //friend��??
		cCamera();
		~cCamera();
		
		void Init(const Vector3& look, const Vector3& pos);
		void Update();  //ī�޶� ���⺤��, ��溤�� ������Ʈ
//		void Render(const int hp, const int sp);

		void SetPosition(const Matrix44& pos);  //ī�޶� look, pos ����
		void SetTranslation(const Vector3& pos);  //ī�޶� look, pos �̵�
//		void SetTranslation(const float z);
//		void SetRotation(const POINT& ptMouse);  //ī�޶� ȸ�� ����
		void SetRotation(const float x = 0, const float y = 0);  //ī�޶� ȸ�� ����
		void SetView();
		void SetHeight(const float number);  //���α׷� �׽�Ʈ��
		void SetCamera(const Vector3& look, const Vector3& pos);
		
		const Vector3& GetPosition() const;
		const Vector3& GetDirection() const;
		const Vector3& GetRight() const;
		const Vector3& GetLook() const;
		const Matrix44& GetView() const;
		const Matrix44& GetProjection() const;
//		cShader& GetShader();

	private:
		Vector3 m_pos;
		Vector3 m_look;
		Vector3 m_up;
		Vector3 m_right;
		Vector3 m_dir;
		Matrix44 m_view;
		Matrix44 m_proj;
	
	//ī�޶�� ����
//		ID3DXFont* m_font;
//		cShader m_shader;
	};

//	inline void  cCamera::SetHeight(const float number) { m_pos.y += number; }

//	inline cCamera* GetCamera() { return cCamera::Get(); }  //graphic�� �Լ�. this(camera)
	inline const Vector3& cCamera::GetPosition() const { return m_pos; }
	inline const Vector3& cCamera::GetDirection() const { return m_dir; }
	inline const Vector3& cCamera::GetRight() const { return m_right; }
	inline const Vector3& cCamera::GetLook() const { return m_look; }
	inline const Matrix44& cCamera::GetView() const { return m_view; }
	inline const Matrix44& cCamera::GetProjection() const { return m_proj; }
//	inline cShader& cCamera::GetShader() { return m_shader; }
}