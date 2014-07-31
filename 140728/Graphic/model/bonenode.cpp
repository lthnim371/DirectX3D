
#include "stdafx.h"
#include "bonenode.h"


using namespace graphic;


cBoneNode::cBoneNode(const int id, vector<Matrix44> &palette, const sRawBone &rawBone) :
	cNode(id, rawBone.name)
,	m_track(NULL)
,	m_mesh(NULL)
,	m_palette(palette)
,	m_aniStart(0)
,	m_aniEnd(0)
,	m_curPlayFrame(0)
,	m_incPlayFrame(0)
,	m_totalPlayTime(0)
,	m_curPlayTime(0)
,	m_incPlayTime(0)
,	m_isAni(false)
,	m_isLoop(false)
{
	m_offset = rawBone.worldTm.Inverse();
	m_localTM = rawBone.localTm;

	m_mesh = new cMesh(id, rawBone);

}

cBoneNode::~cBoneNode()
{
	SAFE_DELETE( m_track );
	SAFE_DELETE( m_mesh );
}


// �ִϸ��̼� ����.
void cBoneNode::SetAnimation( const sRawAni &rawAni, int nAniFrame, bool bLoop)
{
	int aniend = 0;
	if (0 == nAniFrame)
	{
		m_totalPlayTime = (int)rawAni.end;
		aniend = (int)rawAni.end;
	}
	else
	{
		m_totalPlayTime = nAniFrame;
		aniend = (int)rawAni.end;
	}

	m_aniStart = rawAni.start;
	m_aniEnd	 = aniend;
	m_incPlayFrame = 0;

	m_isLoop = bLoop;
	m_isAni = true;

	m_curPlayFrame = rawAni.start;
	m_curPlayTime = rawAni.start * 0.03333f;

	SAFE_DELETE(m_track)
	m_track = new cTrack(rawAni);
}


// ���ϸ��̼�.
bool cBoneNode::Move(const float elapseTime)
{
	RETV(!m_isAni, true);
	RETV(!m_track, true);

	m_curPlayTime += elapseTime;
	m_incPlayTime += elapseTime;
	m_curPlayFrame = (int)(m_curPlayTime * 30.f);
	m_incPlayFrame = (int)(m_incPlayTime * 30.f);

	BOOL ani_loop_end = (m_curPlayFrame > m_aniEnd);	// ���ϸ��̼� ������ ���ٸ� TRUE
	BOOL ani_end = (!m_isLoop) && (m_incPlayFrame > m_totalPlayTime);	// �ѿ��ϸ��̼� �ð��� �����ٸ� TRUE

	if (ani_loop_end || ani_end)
	{
		// �������ϸ��̼��� ������� �ݺ� ���ϸ��̼��̶��
		// �������� ���ϸ��̼��� ó������ ������.
		if (m_isLoop)
		{
			m_curPlayFrame = m_aniStart;
			m_curPlayTime = m_aniStart * 0.03334f;
			m_track->InitAnimation(m_aniStart);
		}
		else
		{
			// �ݺ� ���ϸ��̼��� �ƴ϶�� 
			// �� ���ϸ��̼� �ð��� ������ ���ϸ��̼��� �����ϰ� FALSE�� �����Ѵ�.
			// �׷��� �ʴٸ� ���ϸ��̼��� ó������ ������.				
			if (ani_loop_end)
			{
				m_curPlayFrame = m_aniStart;
				m_curPlayTime = m_aniStart * 0.03334f;

				// �� ���ϸ��̼��� ������ �ʾҴٸ� ���ϸ��̼� ������ ó������ �ǵ�����.
				// �� ���ϸ��̼��� �����ٸ� ������ �ǵ����� �ʰ� ������ �������� ���ϰ� �������д�.
				// ���� ���ϸ��̼ǿ��� �����Ǳ� ���ؼ� ������ ���������� �ξ�� �Ѵ�.
				if (!ani_end)
					m_track->InitAnimation(m_aniStart);
			}
			if (ani_end)
			{
				m_isAni = false;
				return false;
			}
		}
	}

	m_aniTM.SetIdentity();
	m_track->Move( m_curPlayFrame, m_aniTM );

	m_accTM = m_localTM * m_aniTM * m_TM;

	// ���� posŰ���� ������ local TM�� ��ǥ�� ����Ѵ�
	if( m_aniTM._41 == 0.0f && m_aniTM._42 == 0.0f && m_aniTM._43 == 0.0f )
	{
		m_accTM._41 = m_localTM._41;
		m_accTM._42 = m_localTM._42;
		m_accTM._43 = m_localTM._43;
	}
	else	// posŰ���� ��ǥ������ �����Ѵ�(�̷��� ���� ������ TM�� pos������ �ι�����ȴ�)
	{
		m_accTM._41 = m_aniTM._41;
		m_accTM._42 = m_aniTM._42;
		m_accTM._43 = m_aniTM._43;
	}

	if (m_parent)
		m_accTM = m_accTM * ((cBoneNode*)m_parent)->m_accTM;

	m_palette[ m_id] = m_offset * m_accTM;

	BOOST_FOREACH (auto p, m_children)
		p->Move( elapseTime );

	return true;
}


void cBoneNode::Render(const Matrix44 &parentTm)
{
	RET(!m_mesh);

	if (m_track)
		m_mesh->Render(m_offset * m_accTM * parentTm);
	else
		m_mesh->Render(parentTm);

	BOOST_FOREACH (auto p, m_children)
		p->Render( parentTm );
}


void cBoneNode::SetCurrentFrame(const int curFrame) 
{ 
	m_curPlayTime = curFrame / 30.f;
	m_curPlayFrame = curFrame; 
	if (m_track)
		m_track->SetCurrentFramePos(curFrame);
}


// m_accTM �� ������Ʈ �Ѵ�.
void cBoneNode::UpdateAccTM()
{
	m_accTM = m_localTM * m_aniTM * m_TM;
	if (m_parent)
		m_accTM = m_accTM * ((cBoneNode*)m_parent)->m_accTM;
	m_palette[ m_id] = m_offset * m_accTM;
}