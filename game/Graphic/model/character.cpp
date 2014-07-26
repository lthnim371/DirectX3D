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
	Vector3 camDirN( camDir.Normal() );
	Vector3 camR( GetCamera()->GetRight() );
	
	switch( state )
	{
		case NONE:  //기본 동작
			if( m_animode )  //현재 다른 애니모션 상태라면..
			{
				SetAnimation( "..\\media\\valle(new)\\normal.ani" );
				m_animode = false;
			}
		break;

		case ROTATION:  //캐릭터 회전
			{
				Quaternion q( Vector3(0,1,0), -x * 0.005f ); 
				Matrix44 m = q.GetMatrix();
				SetTM( m * GetTM() );  //R * T
			}
		break;
		
		case FORWARD:  //앞으로 이동
			if( !m_animode )  //입력키가 처음 눌러졌다면..
			{
				SetAnimation( "..\\media\\valle(new)\\forward.ani" );
				m_animode = true;
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 5.f );  //카메라 방향으로
			MultiplyTM( mat );  //현재 위치에 더해주기
			GetCamera()->SetTranslation( GetTM() );  //카메라 위치도 갱신
		break;
		
		case BACKWARD:  //뒤로 이동
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle(new)\\backward.ani" );
				m_animode = true;
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * -5.f );
			MultiplyTM( mat );
			GetCamera()->SetTranslation( GetTM() );
		break;

		case LEFTWARD:  //왼쪽 이동
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle(new)\\forward.ani" );
				m_animode = true;
			}
			mat.SetTranslate( Vector3( camR.x, 0.f, camR.z ) * -5.f );  //카메라 좌우방향으로
			MultiplyTM( mat );
			GetCamera()->SetTranslation( GetTM() );
		break;

		case RIGHTWARD:  //오른쪽 이동
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle(new)\\forward.ani" );
				m_animode = true;
			}
			mat.SetTranslate( Vector3( camR.x, 0.f, camR.z ) * 5.f );
			MultiplyTM( mat );
			GetCamera()->SetTranslation( GetTM() );
		break;
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