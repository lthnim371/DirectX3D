
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


// ���ϸ��̼�.
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
	m_mesh->Render(m_offset * m_accTM  * parentTm);  //��������� ������ķ� ��ȯ�ϱ� ���� �ϴ� �����(m_offset)�� ���Ͽ� ���ѵ� �θ����� ����� �Ÿ� ���ѵ�(m_accTM) ���콺 ��ġ�� ���� ȸ��(parentTm)
}