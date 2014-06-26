//140618 �ۼ� //140619 ����

#pragma once

//#include "..\stdafx.h"

namespace graphic  //graphic �����ȿ� �ѵ� ��� �����ϱ� ���ϰ�
{
	class cDX9Material
	{
	public:  //����Ʈ�Լ�
		cDX9Material();

		//������ ���ÿ� �ʱ�ȭ
		cDX9Material(const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse,
			const D3DXCOLOR& specular, const D3DXCOLOR& emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
			float spcePow = 0.f);

		~cDX9Material();
	
	public:  //����Լ�
		//���� �� �ʱ�ȭ
		void Initial(const D3DXCOLOR& ambient, const D3DXCOLOR& diffuse,
			const D3DXCOLOR& specular, const D3DXCOLOR& emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
			float spcePow = 0.f);		

		//���������� ���� �ʱ�ȭ �� ����
		void SetAmbient(const D3DXCOLOR& ambient);
		void SetDiffuse(const D3DXCOLOR& diffuse);
		void SetSpecular(const D3DXCOLOR& specular);
		void SetEmissive(const D3DXCOLOR& emissive);
		void SetSpecularPow(float spcePow);
		
		//���� ����
		void Bind() const;

		//��ü ��ȯ
		inline D3DMATERIAL9 GetMaterial() const { return m_material; };
			
	private:  //�����ü
		D3DMATERIAL9 m_material;
	};
}	