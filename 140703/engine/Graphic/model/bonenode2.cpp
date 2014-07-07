﻿
#include "stdafx.h"
#include "bonenode2.h"


using namespace graphic;


cBoneNode::cBoneNode(const int id, vector<Matrix44> &palette, const sRawMesh &rawMesh) :
	cNode(id)
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
	m_offset = rawMesh.worldTm.Inverse();
	m_localTM = rawMesh.localTm;

	m_mesh = new cMesh(id, rawMesh);

}

cBoneNode::~cBoneNode()
{
	SAFE_DELETE( m_track );
	SAFE_DELETE( m_mesh );
}


// 애니메이션 설정.
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

	m_aniStart = 0;
	m_aniEnd	 = aniend;
	m_incPlayFrame = 0;

	m_isLoop = bLoop;
	m_isAni = true;

	m_curPlayFrame = 0;

	SAFE_DELETE(m_track)
	m_track = new cTrack(rawAni);
}


// 에니메이션.
bool cBoneNode::Move(const float elapseTime)
{
	RETV(!m_isAni, true);
	RETV(!m_track, true);

	m_curPlayTime += elapseTime;
	m_incPlayTime += elapseTime;
	m_curPlayFrame = (int)(m_curPlayTime * 30.f);
	m_incPlayFrame = (int)(m_incPlayTime * 30.f);

	BOOL ani_loop_end = (m_curPlayFrame > m_aniEnd);	// 에니메이션 끝까지 갔다면 TRUE
	BOOL ani_end = (!m_isLoop) && (m_incPlayFrame > m_totalPlayTime);	// 총에니메이션 시간이 지났다면 TRUE

	if (ani_loop_end || ani_end)
	{
		// 보간에니메이션이 종료된후 반복 에니메이션이라면
		// 보간없이 에니메이션을 처음으로 돌린다.
		if (m_isLoop)
		{
			m_curPlayFrame = (int)(m_aniStart * 30.f);
			m_curPlayTime = 0;
			m_track->InitAnimation();
		}
		else
		{
			// 반복 에니메이션이 아니라면 
			// 총 에니메이션 시간이 지나면 에니메이션을 종료하고 FALSE를 리턴한다.
			// 그렇지 않다면 에니메이션을 처음으로 돌린다.				
			if (ani_loop_end)
			{
				m_curPlayFrame = (int)(m_aniStart * 30.f);
				m_curPlayTime = 0;

				// 총 에니메이션이 끝나지 않았다면 에니메이션 정보를 처음으로 되돌린다.
				// 총 에니메이션이 끝났다면 정보를 되돌리지 않고 마지막 프레임을 향하게 내버려둔다.
				// 다음 에니메이션에서 보간되기 위해서 마지막 프레임으로 두어야 한다.
				if (!ani_end)
					m_track->InitAnimation();
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

	m_accTM = m_localTM * m_aniTM * m_TM;  //
	//m_localTM은 자신의 위치를 알 수 있다.(부모로부터 부모의 위치들이 상대적으로 쌓여서 오기 떄문에)
	//m_aniTM은 애니메이션된(이동 및 회전 등) 월드행렬이다.
	//m_TM : 외부에서 임의의 행렬값으로 변형을 주고 싶을 때 이용한다.
	//★이대로 곱해주게 되면 문제가 생긴다.
	//m_aniTM은 3dsmax에서 월드행렬로 정해놓은 상태라 만약 애니메이션 중에 m_TM을 이용한다면 원하는 변형을 줄수가 없다.

<<<<<<< HEAD
=======
	//위의 식에서 위치(pos)값이 localTm과 aniTm의 두개 다 적용된 상태라 이상한 위치로 되버린다.
	//그래서 아래의 if문을 통해 aniTm의 위치값만 적용해준다.
>>>>>>> origin/master
	// 만약 pos키값이 없으면 local TM의 좌표를 사용한다
	if( m_aniTM._41 == 0.0f && m_aniTM._42 == 0.0f && m_aniTM._43 == 0.0f )
	{
		m_accTM._41 = m_localTM._41;
		m_accTM._42 = m_localTM._42;
		m_accTM._43 = m_localTM._43;
	}
	else	// pos키값을 좌표값으로 적용한다(이렇게 하지 않으면 TM의 pos성분이 두번적용된다)
	{
		m_accTM._41 = m_aniTM._41;
		m_accTM._42 = m_aniTM._42;
		m_accTM._43 = m_aniTM._43;
	}

<<<<<<< HEAD
	if (m_parent)  //★이 코드를 위의 if문 전에 두면 출력이 이상해진다. 즉, 현재 이 코드 순서가 올바르다
=======
	if (m_parent)  //그리고나서 상대적인 부모의 누적된 Tm도 곱해준다.
>>>>>>> origin/master
		m_accTM = m_accTM * ((cBoneNode*)m_parent)->m_accTM;

	m_palette[ m_id] = m_offset * m_accTM;

	//if (m_pBox)
	//	m_pBox->SetWorldTM(&m_pPalette[ m_nId]);

	BOOST_FOREACH (auto p, m_children)
		p->Move( elapseTime );

	return true;
}


void cBoneNode::Render(const Matrix44 &parentTm)
{
	RET(!m_mesh);
	m_mesh->Render(m_offset * m_accTM * parentTm);

	BOOST_FOREACH (auto p, m_children)
		p->Render( parentTm );
}
