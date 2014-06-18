//140618 �ۼ� - ������

#pragma once

#include "..\stdafx.h"

namespace graphic  //graphic �����ȿ� �ѵ� ��� �����ϱ� ���ϰ�
{
	class cMyMaterial
	{
	public:  //����Ʈ�Լ�
		cMyMaterial();

		//������ ���ÿ� �ʱ�ȭ
		cMyMaterial(const D3DXCOLOR ambient, const D3DXCOLOR diffuse,
			const D3DXCOLOR specular, const D3DXCOLOR emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
			const float spcePow = 0.f);

		~cMyMaterial();
	
	public:  //����Լ�
		//���� �� �ʱ�ȭ
		void Initial(const D3DXCOLOR ambient, const D3DXCOLOR diffuse,
			const D3DXCOLOR specular, const D3DXCOLOR emissive = D3DXCOLOR(0.f,0.f,0.f,1.f),
			const float spcePow = 0.f);		

		//���������� �ʱ�ȭ �� �� �ְԲ� �غ��Ͽ���
		void SetAmbient(const D3DXCOLOR ambient);
		void SetDiffuse(const D3DXCOLOR diffuse);
		void SetSpecular(const D3DXCOLOR specular);
		void SetEmissive(const D3DXCOLOR emissive);
		void SetSpecularPow(const float spcePow);
		
		//����̽� ��Ʈ���� ����
		void Bind() const;

		//�ڱ��ڽ�(��Ʈ����) ��ȯ
		inline D3DMATERIAL9 GetMaterial() const { return m_material; };
			
	private:  //�����ü
		D3DMATERIAL9 m_material;
	};
}	