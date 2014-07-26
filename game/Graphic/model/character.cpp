#include "stdafx.h"
//#include "..\stdafx.h"
#include "character.h"

using namespace graphic;


cCharacter::cCharacter(const int id) :
	cModel(id)
,	m_weapon(NULL)
,	m_weaponNode(NULL)
, m_animode(true)
, m_position(0, 0, 0)
, m_state(NONE)
{

}

cCharacter::~cCharacter()
{
	SAFE_DELETE(m_weapon);
}


bool cCharacter::Create(const string &modelName)
{
	m_weaponNode = NULL;
	return cModel::Create(modelName);
}


void cCharacter::LoadWeapon(const string &fileName)
{
	SAFE_DELETE(m_weapon);

	RET(!m_bone);
//	m_weaponNode = m_bone->FindBone("dummy_weapon");
//	m_weaponNode = m_bone->FindBone("Handle");
//	m_weaponNode = m_bone->FindBone("HeadDummy");
//	m_weaponNode = m_bone->FindBone("Bip01-L-Hand");
//	RET(!m_weaponNode);

	if (!m_weapon)
		m_weapon = new cModel(100);

	if (!m_weapon->Create(fileName))
		return;

//	GetBoneMgr()->SwapBone( m_weapon->GetBoneMgr() );

	m_weapon->SetAnimation("..\\media\\valle(new)\\forward.ani");
}


bool cCharacter::Move(const float elapseTime)
{
	//test¿ë
	//Matrix44 mat;
	//SetTM(mat);
	//SetAnimation(str);

//	Action();
	
	
	cModel::Move(elapseTime);
	
	if (m_weapon)// && m_weaponNode)
	{
	//	const Matrix44 mat = m_weaponNode->GetAccTM();
	//	m_weapon->SetTM(mat * m_matTM);
		m_weapon->Move(elapseTime);
	}

	return true;
}


void cCharacter::Render()
{
	cModel::Render();

	if (m_weapon)
		m_weapon->Render();
}

void cCharacter::Action(const int state, const float x)
{
	Matrix44 mat;
	Vector3 camDir( GetCamera()->GetDirection() );
	Vector3 camR( GetCamera()->GetRight() );
	
	switch( state )
	{
		case NONE:
			if( m_animode )
			{
				SetAnimation( "..\\media\\valle(new)\\normal.ani" );
				m_animode = false;
			}
		break;
		
		case FORWARD:
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle(new)\\forward.ani" );
				m_animode = true;

				/*Matrix44 rot;
				rot.SetRotationY( GetCamera()->GetRotation() );
				MultiplyTM( rot );*/

				/*
				Vector3 pos( m_matTM.GetPosition() );
				float theta = pos.Normal().DotProduct( camDir.Normal() );
				theta = acos(theta);
				theta = theta * (180 / MATH_PI);
				if( theta != 0.f )
				{
					theta *= ( (camDir.x < pos.x) ? 1 : -1 );
					Quaternion q( Vector3(0,1,0), theta ); 
					Matrix44 m = q.GetMatrix();
					MultiplyTM( m );
				}
				*/
			}
		//	MultiplyTM( GetCamera()->GetRotation() );
		//	mat.SetTranslate( Vector3( camDir.x, 0.f, camDir.z ) * 5.f );
			mat.SetTranslate( Vector3( camDir.Normal().x, 0.f, camDir.Normal().z ) * 5.f );
			MultiplyTM( mat );
			GetCamera()->SetTranslation( GetTM() );
		//	GetCamera()->SetTranslation( 5.f );
		break;
		
		case BACKWARD:
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle(new)\\backward.ani" );
				m_animode = true;
			}
		//	SetTM( m_matTM * GetCamera()->GetRotation() );
		//	mat.SetTranslate( Vector3( camDir.x, 0.f, camDir.z ) * -5.f );
			mat.SetTranslate( Vector3( camDir.Normal().x, 0.f, camDir.Normal().z ) * -5.f );
			MultiplyTM( mat );
			GetCamera()->SetTranslation( GetTM() );
		//	GetCamera()->SetTranslation( -5.f );
		break;

		case LEFTWALK:
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle(new)\\forward.ani" );
				m_animode = true;
			}
			mat.SetTranslate( Vector3( camR.x, 0.f, camR.z ) * -5.f );
			MultiplyTM( mat );
			GetCamera()->SetTranslation( GetTM() );
		break;

		case RIGHTWALK:
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle(new)\\forward.ani" );
				m_animode = true;
			}
			mat.SetTranslate( Vector3( camR.x, 0.f, camR.z ) * 5.f );
			MultiplyTM( mat );
			GetCamera()->SetTranslation( GetTM() );
		break;

		/*case ROTATION:
			{
				Quaternion q( Vector3(0,1,0), -x * 0.005f ); 
				Matrix44 m = q.GetMatrix();
				MultiplyTM( m );				
			}
		break;*/


	}
	
}

//void cCharacter::SetRotation(const float x, const float y)
//{
//	if( x != 0 )
//	{ // rotate Y-Axis
//	//	Quaternion q( Vector3(0,1,0), -x * 0.005f ); 
//	//	Matrix44 m = q.GetMatrix();
//		Matrix44 m;
//		m.SetRotationY( x * 0.005f );
//		MultiplyTM( m );
//	}
//	
//	GetCamera()->SetRotation( x, y );
//}

//void cCharacter::Test()
//{
//	Matrix44 mat;
//
//	if( GetCamera()->Move() )
//	{
//		if( !m_animode )
//		{
//			SetAnimation( "..\\media\\valle(new)\\forward.ani" );
//			m_animode = true;
//		}		
//		mat.SetTranslate( GetCamera()->GetPosition() + Vector3(0,-300,150) );
//		SetTM( mat );
//	}
//	else if(m_animode)
//	{
//		SetAnimation( "..\\media\\valle(new)\\normal.ani" );
//		m_animode = false;
//	}
//	
//
//	//SetAnimation( "C:\\Users\\Lee\\Desktop\\ABresource\\scripts\\idle.ani" );
//}

/*	backup1
	if( (::GetAsyncKeyState('W') & 0x8000) == 0x8000 )
	{
		if( !animode )
		{
			SetAnimation( "..\\media\\valle(new)\\forward.ani" );
			animode = true;
		}
		mat.SetTranslate( Vector3( 0,0,5 ) );
		MultiplyTM( mat );

		return;
	}
	else if( (::GetAsyncKeyState('S') & 0x8000) == 0x8000 )
	{
		if( !animode )
		{
			SetAnimation( "..\\media\\valle(new)\\backward.ani" );
			animode = true;
		}
		mat.SetTranslate( Vector3( 0,0,-5 ) );
		MultiplyTM( mat );

		return;
	}
*/


/* backup2
void cCharacter::Action()
{
	Matrix44 mat;

	if( (::GetAsyncKeyState('W') & 0x8000) == 0x8000 )
	{		
		if( !m_animode )
		{
			SetAnimation( "..\\media\\valle(new)\\forward.ani" );
			m_animode = true;
		}
		mat.SetTranslate( GetCamera()->GetDirection() * 5.f );
		MultiplyTM( mat );
		GetCamera()->SetPosition( GetTM().GetPosition() );
	}
	else if( (::GetAsyncKeyState('S') & 0x8000) == 0x8000 )
	{	
		if( !m_animode )
		{
			SetAnimation( "..\\media\\valle(new)\\backward.ani" );
			m_animode = true;
		}
		mat.SetTranslate( GetCamera()->GetDirection() * -5.f );
		MultiplyTM( mat );
		GetCamera()->SetPosition( GetTM().GetPosition() );
	}
	else
	{
		if( m_animode )
		{
			SetAnimation( "..\\media\\valle(new)\\normal.ani" );
			m_animode = false;
		}
	}
}
*/