#pragma once


namespace graphic
{

	//struct sVertexDiffuse
	//{
	//	Vector3 p;
	//	DWORD c;
	//	enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	//};


	struct sVertexNormTex
	{
		sVertexNormTex() {}
		sVertexNormTex(float x0, float y0, float z0) : p(Vector3(x0, y0, z0)), n(Vector3(0,0,0)),
		u(-100), v(-100) {}
		Vector3 p;
		Vector3 n;
		float u,v;

		enum {FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
	};


}
