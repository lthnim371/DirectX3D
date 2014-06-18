
#pragma once

#include "..\stdafx.h"

class cMaterial
{
public:  //����Ʈ�Լ�
	cMaterial();
	cMaterial(const D3DXCOLOR ambient, const D3DXCOLOR diffuse,
		const D3DXCOLOR specular, const float specScale);
	~cMaterial();
public:  //����Լ�
	void Initial(const D3DXCOLOR ambient, const D3DXCOLOR diffuse,
		const D3DXCOLOR specular, const float specScale);
	void SetAmbient(const D3DXCOLOR ambient);
	void SetDiffuse(const D3DXCOLOR diffuse);
	void SetSpecular(const D3DXCOLOR specular);
	void SetEmissive(const D3DXCOLOR emissive);
	void SetSpecularScale(const float specScale);

	inline D3DMATERIAL9 GetMaterial() const { return m_material; };
private:  //�����ü
	D3DMATERIAL9 m_material;
};