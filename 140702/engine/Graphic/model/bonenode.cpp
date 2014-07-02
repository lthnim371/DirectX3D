
#include "stdafx.h"
#include "bonenode.h"


using namespace graphic;


cBoneNode::cBoneNode(const int id, const sRawMesh &rawMesh) :
	cNode(id)
,	m_mesh(NULL)
{
	m_offset = rawMesh.worldTm.Inverse();
	m_localTM = rawMesh.localTm;

	m_mesh = new cMesh(id, rawMesh);
}

cBoneNode::~cBoneNode()
{
	SAFE_DELETE( m_mesh );
}


// 에니메이션.
bool cBoneNode::Move(const float elapseTime)
{
	m_aniTM.SetIdentity();

	m_accTM = m_localTM * m_aniTM * m_TM;

	if (m_parent)
		m_accTM = m_accTM * ((cBoneNode*)m_parent)->m_accTM;

	return true;
}


void cBoneNode::Render(const Matrix44 &parentTm)
{
	RET(!m_mesh);
	m_mesh->Render(m_offset * m_accTM  * parentTm);  //월드행렬을 로컬행렬로 변환하기 위해 일단 역행렬(m_offset)을 곱하여 구한뒤 부모들과의 상대적 거리 곱한뒤(m_accTM) 마우스 위치에 따른 회전(parentTm)
}
