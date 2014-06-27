#include "base.h"
#include "track.h"

using namespace graphic;

cTrack::cTrack( const tagRawAni& rawAni )
	: m_rawAni(rawAni), m_IndexMove(0), alpha(0.f)
{}
cTrack::~cTrack()
{}

//메인함수
void cTrack::Move( const int currFrame, OUT Matrix44& out )
{
	if( currFrame >= m_rawAni.pos[m_IndexMove + 1].t )
	{
		if( m_IndexMove >= (int)m_rawAni.pos.size() - 2 )
			m_IndexMove = 0;
		else
			m_IndexMove++;
	}

	alpha = currFrame - m_rawAni.pos[m_IndexMove].t /
		m_rawAni.pos[m_IndexMove + 1].t - m_rawAni.pos[m_IndexMove].t;

	Matrix44 move;
	move.SetTranslate( m_rawAni.pos[m_IndexMove].p.Interpolate(
		m_rawAni.pos[m_IndexMove + 1].p, alpha ));

	out = move;
}