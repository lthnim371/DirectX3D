#include "base.h"
#include "track.h"

using namespace graphic;

cTrack::cTrack( const tagRawAni& rawAni )
	: m_rawAni(rawAni), m_keyIndexPos(1), m_keyIndexRot(1)
{}
cTrack::~cTrack()
{
}

//메인함수
void cTrack::Move( const int currFrame, OUT Matrix44& out )
{
	Matrix44 p, r, s;
	KeyPos( currFrame, p );
	KeyRot( currFrame, r );

	out = p * r;
}


void cTrack::KeyPos( const int currFrame, OUT Matrix44& out )
{
	switch( (int)m_rawAni.pos.size() )
	{
		case 0:
			out.SetIdentity();
			return;
		case 1:
			out.SetTranslate( m_rawAni.pos[0].p );
			return;
		default:
			break;
	}
	
	if( currFrame < m_rawAni.pos[m_keyIndexPos - 1].t )
		return;
	else if( currFrame >= m_rawAni.pos[m_keyIndexPos].t )
	{
		if( m_keyIndexPos >= (int)m_rawAni.pos.size() - 1)
		{
			//m_keyIndexPos = 0;
			out.SetTranslate( m_rawAni.pos[m_keyIndexPos].p );
			return;
		}
		m_keyIndexPos++;
	}
	
	const float alpha = ( (float)currFrame - m_rawAni.pos[m_keyIndexPos - 1].t ) /
		( m_rawAni.pos[m_keyIndexPos].t - m_rawAni.pos[m_keyIndexPos - 1].t );

	out.SetTranslate( m_rawAni.pos[m_keyIndexPos - 1].p.Interpolate(
		m_rawAni.pos[m_keyIndexPos].p, alpha ));
}

void cTrack::KeyRot( const int currFrame, OUT Matrix44& out )
{
	switch( (int)m_rawAni.rot.size() )
	{
		case 0:
			out.SetIdentity();
			return;
		case 1:
			out = m_rawAni.rot[0].q.GetMatrix();
			return;
		default:
			break;
	}
	
	if( currFrame < m_rawAni.rot[m_keyIndexRot - 1].t )
		return;
	else if( currFrame >= m_rawAni.rot[m_keyIndexRot].t )
	{
		if( m_keyIndexRot >= (int)m_rawAni.rot.size() - 1)
		{
			//m_keyIndexPos = 0;
			out = m_rawAni.rot[m_keyIndexRot].q.GetMatrix();
			return;
		}
		m_keyIndexRot++;
	}
	
	const float alpha = ( (float)currFrame - m_rawAni.rot[m_keyIndexRot - 1].t ) /
		( m_rawAni.rot[m_keyIndexRot].t - m_rawAni.rot[m_keyIndexRot - 1].t );

	Quaternion q =	m_rawAni.rot[m_keyIndexRot - 1].q.Interpolate(
		m_rawAni.rot[m_keyIndexRot].q,
		alpha );

	out = q.GetMatrix();
}