
#include "stdafx.h"
#include "model.h"
#include "node.h"
#include "../manager/resourcemanager.h"
#include "mesh.h"
#include "bonemgr.h"


using namespace graphic;


// Bone���� ���ڽ��� �����Ѵ�.
struct sMinMax
{
	Vector3 Min;
	Vector3 Max;
	sMinMax() : Min(Vector3(0,0,0)), Max(Vector3(0,0,0)) {}
};



cModel::cModel() :
	m_bone(NULL)
,	m_isRenderMesh(true)
,	m_isRenderBone(false)
{
	
}

cModel::~cModel()
{
	Clear();
}


bool cModel::Create(const string &modelName)
{
	sRawMeshGroup *rawMeshes = cResourceManager::Get()->LoadModel(modelName);
	RETV(!rawMeshes, false);

	Clear();

	const bool isSkinnedMesh = !rawMeshes->bones.empty();

	// ��Ű�� �ִϸ��̼��̸� Bone�� �����Ѵ�.
	if (isSkinnedMesh)
	{
		m_bone = new cBoneMgr(0, *rawMeshes);
	}

	// �޽� ����.
	int id = 0;
	BOOST_FOREACH (auto &mesh, rawMeshes->meshes)
	{
		cMesh *p = NULL;
		if (isSkinnedMesh)
		{
			p = new cSkinnedMesh(id++, m_bone->GetPalette(), mesh);
		}
		else
		{
			p = new cRigidMesh(id++, mesh);
		}

		if (p)
			m_meshes.push_back(p);
	}

	CreateBoneBoundingBox(modelName);

	return true;
}


void cModel::SetAnimation( const string &aniFileName)
{
	if (sRawAniGroup *rawAnies = cResourceManager::Get()->LoadAnimation(aniFileName))
	{
		if (m_bone)
		{
			m_bone->SetAnimation(*rawAnies, 0);
		}
		else
		{
			for (u_int i=0; i < m_meshes.size(); ++i)
			{
				((cRigidMesh*)m_meshes[ i])->LoadAnimation(rawAnies->anies[0]);
			}
		}
	}
}


bool cModel::Move(const float elapseTime)
{
	BOOST_FOREACH (auto node, m_meshes)
		node->Move(elapseTime);

	if (m_bone)
		m_bone->Move(elapseTime);

	return true;
}


void cModel::Render()
{
	Matrix44 identity;
	GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&identity);

	if (m_isRenderMesh)
	{
		BOOST_FOREACH (auto node, m_meshes)
			node->Render(m_matTM);
	}

	if (m_isRenderBone && m_bone)
		m_bone->Render(m_matTM);

	// render bounding Box
	for (int i=0; i < (int)m_boundingBox.size(); ++i)
		m_boundingBox[ i].Render( m_bone->GetPalette()[ i] * m_matTM);  //m_matTM�� ���� ���� �������� �־ ���� ������(���ڽ�)
}


// remove all data
void cModel::Clear()
{
	BOOST_FOREACH (auto mesh, m_meshes)
	{
		SAFE_DELETE(mesh);
	}
	m_meshes.clear();

	SAFE_DELETE(m_bone);
}


// �޽��� ã�Ƽ� �����Ѵ�.
cMesh* cModel::FindMesh(const string &meshName)
{
	BOOST_FOREACH (auto &mesh, m_meshes)
	{
		if (mesh->GetName() == meshName)
			return (cMesh*)mesh;
	}
	return NULL;
}


// Bone bounding box
void cModel::CreateBoneBoundingBox(const string &modelName)
{
	RET(!m_bone);

	sRawMeshGroup *rawMeshes = cResourceManager::Get()->LoadModel(modelName);
	RET(!rawMeshes);

	const int boneCount = rawMeshes->bones.size();
	vector<sMinMax> boundingBox(boneCount);
	vector<Matrix44> boneInvers(boneCount);
	for (int i=0; i < boneCount; ++i)
	{
		boneInvers[ i] = rawMeshes->bones[ i].worldTm.Inverse();  //���� ������ǥ�� ��ȯ�ϱ� ���� �غ�
		boundingBox[ i] = sMinMax();
	}


	BOOST_FOREACH (const sRawMesh &mesh, rawMeshes->meshes)
	{
		BOOST_FOREACH (const sVertexWeight &weight, mesh.weights)
		{
			const int vtxIdx = weight.vtxIdx;
			
			for( int k=0; k < weight.size; ++k )
			{
				const sWeight *w = &weight.w[ k];
				const Vector3 pos = mesh.vertices[ vtxIdx] * boneInvers[ w->bone];  //������ǥ�� ��ȯ

				//������ǥü�� ������ �Ǵ�
				if (boundingBox[ w->bone].Min.x > pos.x)
					boundingBox[ w->bone].Min.x = pos.x;
				if (boundingBox[ w->bone].Min.y > pos.y)
					boundingBox[ w->bone].Min.y = pos.y;
				if (boundingBox[ w->bone].Min.z > pos.z)
					boundingBox[ w->bone].Min.z = pos.z;

				if (boundingBox[ w->bone].Max.x < pos.x)
					boundingBox[ w->bone].Max.x = pos.x;
				if (boundingBox[ w->bone].Max.y < pos.y)
					boundingBox[ w->bone].Max.y = pos.y;
				if (boundingBox[ w->bone].Max.z < pos.z)
					boundingBox[ w->bone].Max.z = pos.z;
			}
		}
	}


	m_boundingBox.resize(boneCount);
	for (int i=0; i < boneCount; ++i)
	{
		// ���� ��ǥ�������� �̵���Ų��. palette �� �����ϱ� ���ؼ��� ��������� �־�� ��.
		const Vector3 wMin = boundingBox[ i].Min;// *  rawMeshes->bones[ i].worldTm;
		const Vector3 wMax = boundingBox[ i].Max;// *  rawMeshes->bones[ i].worldTm;
		m_boundingBox[ i].SetCube( wMin, wMax );
		m_boundingBox[ i].SetTransform(rawMeshes->bones[ i].worldTm);  //����TM����
	}
}
