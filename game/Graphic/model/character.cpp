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

//	RET(!m_bone);
////	m_weaponNode = m_bone->FindBone("dummy_weapon");
////	m_weaponNode = m_bone->FindBone("Handle");
//	m_weaponNode = m_bone->FindBone("HeadDummy");
////	m_weaponNode = m_bone->FindBone("Bip01-L-Hand");
//	RET(!m_weaponNode);
//
	if (!m_weapon)
		m_weapon = new cModel(100);

	if (!m_weapon->Create(fileName))
		return;
}


bool cCharacter::Move(const float elapseTime)
{
	//test¿ë
	//Matrix44 mat;
	//SetTM(mat);
	//SetAnimation(str);

	Action();
	//
	
	cModel::Move(elapseTime);
	
	if (m_weapon && m_weaponNode)
	{
		const Matrix44 mat = m_weaponNode->GetAccTM();
		m_weapon->SetTM(mat * m_matTM);
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


void cCharacter::Action()
{
	Test();	
}

void cCharacter::Test()
{
	Matrix44 mat;

	if( GetCamera()->Move() )
	{
		if( !m_animode )
		{
			SetAnimation( "..\\media\\valle(new)\\forward.ani" );
			m_animode = true;
		}		
		mat.SetTranslate( GetCamera()->GetPosition() + Vector3(0,-300,150) );
		SetTM( mat );
	}
	else if(m_animode)
	{
		SetAnimation( "..\\media\\valle(new)\\normal.ani" );
		m_animode = false;
	}
	

	//SetAnimation( "C:\\Users\\Lee\\Desktop\\ABresource\\scripts\\idle.ani" );
}
//backup
	//if( (::GetAsyncKeyState('W') & 0x8000) == 0x8000 )
	//{
	//	if( !animode )
	//	{
	//		SetAnimation( "..\\media\\valle(new)\\forward.ani" );
	//		animode = true;
	//	}
	//	mat.SetTranslate( Vector3( 0,0,5 ) );
	//	MultiplyTM( mat );

	//	return;
	//}
	//else if( (::GetAsyncKeyState('S') & 0x8000) == 0x8000 )
	//{
	//	if( !animode )
	//	{
	//		SetAnimation( "..\\media\\valle(new)\\backward.ani" );
	//		animode = true;
	//	}
	//	mat.SetTranslate( Vector3( 0,0,-5 ) );
	//	MultiplyTM( mat );

	//	return;
	//}