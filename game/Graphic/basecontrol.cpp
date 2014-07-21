#include "stdafx.h"
#include "basecontrol.h"

cBaseControl::cBaseControl()
{}
cBaseControl::~cBaseControl()
{}

string cBaseControl::Move( OUT Matrix44& outTM )
{
	Matrix44 pos;

	if( ::GetAsyncKeyState('W') & 0x8001 )
	{		
		pos.SetTranslate( Vector3( 0, 0, 5 ) );

		outTM *= pos;

		return "C:\\Users\\Lee\\Desktop\\ABresource\\scripts\\forward.ani";
	}
	else if( ::GetAsyncKeyState('S') & 0x8000 )
	{	
		pos.SetTranslate( Vector3( 0, 0, -5 ) );

		outTM *= pos;

		return "C:\\Users\\Lee\\Desktop\\ABresource\\scripts\\backward.ani";
	}

	return "C:\\Users\\Lee\\Desktop\\ABresource\\scripts\\idle.ani";
}