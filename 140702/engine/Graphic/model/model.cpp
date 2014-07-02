
#include "stdafx.h"
#include "model.h"
#include "node.h"
#include "../manager/resourcemanager.h"
#include "mesh.h"
#include "bone.h"


using namespace graphic;


cModel::cModel() :
	m_bone(NULL)
{
	
}

cModel::~cModel()
{
	Clear();
}


bool cModel::Create(const string &modelName)
{
	sRawMeshGroup *rawMeshes = cResourceManager::Get()->LoadModel(modelName);  //리소스매니저로 익스포트된 데이터 읽어오기
	RETV(!rawMeshes, false);

	Clear();

	const bool isSkinnedMesh = !rawMeshes->bones.empty();  //스키닝 애니메이션인지 확인  //empty() : (배열같은것들)비어 있으면 트루 반환(사이즈는 갯수를 세기때문에 시간이 좀 걸릴 수 있으나 엠티는 있는지 없는지만 판단

	// 스키닝 애니메이션이면 Bone을 생성한다.
	if (isSkinnedMesh)
	{
		m_bone = new cBone(0, *rawMeshes);
	}

	// 메쉬 생성.
	/*for(int i=0; i < rawMeshes->meshes.size(); ++i)
	{
		auto& mesh = rawMeshes->meshes[i];
	}*/
	BOOST_FOREACH (auto &mesh, rawMeshes->meshes)  //위 코드를 부스터로 표현한거라 보면 된다.
	{
		cMesh *p = new cRigidMesh(0, mesh);

		if (!isSkinnedMesh)
		{
			if (sRawAniGroup *rawAnies = cResourceManager::Get()->FindAni(modelName))
			{
				if (!rawAnies->anies.empty())
					((cRigidMesh*)p)->LoadAnimation(rawAnies->anies[0]);
			}
		}

		m_meshes.push_back(p);
	}

	return true;
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

	BOOST_FOREACH (auto node, m_meshes)
		node->Render(m_matTM);

	if (m_bone)
		m_bone->Render(m_matTM);
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
