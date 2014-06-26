//140618 �ۼ� //140619 ����

#pragma once

//#include "..\stdafx.h"

namespace graphic
{
	class cDX9Light
	{
	public:  //����Ʈ�Լ�
		cDX9Light();
		~cDX9Light();

	public:  //����Լ�
		//���� �� �ʱ�ȭ
		void InitPointLight(const Vector3& position,
			const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular);
		void InitDirectionalLight(const Vector3& direction,
			const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular);
		void InitSpotLight(const Vector3& position, const Vector3& direction,
			const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular,
			float theta, float phi);

		//���������� ���� �ʱ�ȭ �� ����
		void SetDirection(const Vector3& direction);
		void SetColor(const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular);
		
		//���� ����
		void Bind(int lightIndex = 0);
		
		//���� ����
		void LightOn(int lightIndex = 0);
		void LightOff(int lightIndex = 0);

		//��ü ��ȯ
		inline D3DLIGHT9 GetLight() const { return m_light; };

	private:  //�����ü
		D3DLIGHT9 m_light;
	};
}