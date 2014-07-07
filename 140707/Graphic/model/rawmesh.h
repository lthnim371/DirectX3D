#pragma once


namespace graphic
{

	struct sMaterial
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector4 emissive;
		float power;
		string texture;

		sMaterial() {}
		sMaterial(const sMaterial &rhs);
		sMaterial& operator=(const sMaterial &rhs);
	};


	struct sWeight
	{
		int bone;
		float weight;
	};


	struct sVertexWeight
	{
		int vtxIdx;
		int size;
		sWeight w[6];
	};

	
	struct sRawBone
	{
		int id;
		int parentId;
		string name;
		Matrix44 localTm;
		Matrix44 worldTm;

		// debug ��.
		vector<Vector3> vertices;
		vector<Vector3> normals; // vertex ������ŭ ����ȴ�.
		vector<Vector3> tex;
		vector<int> indices;
	};


	struct sRawMesh
	{
		string name;
		vector<Vector3> vertices;
		vector<Vector3> normals; // vertex ������ŭ ����ȴ�.
		vector<Vector3> tex;
		vector<int> indices;
		vector<sVertexWeight> weights;
		sMaterial mtrl;
		int mtrlId;
	};


	// ���ϳ��� ������ �����ϴ� �ڷᱸ��.
	struct sRawMeshGroup
	{
		string name;
		vector<sMaterial> mtrls;
		vector<sRawMesh> meshes;
		vector<sRawBone> bones;
	};

}