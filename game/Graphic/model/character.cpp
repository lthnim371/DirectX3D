#include "stdafx.h"
//#include "..\stdafx.h"
#include "character.h"

using namespace graphic;


cCharacter::cCharacter(const int id) :
	cModel(id)
,	m_weapon(NULL)
,	m_weaponNode(NULL)
, m_animode(true)
, m_attackCnt(0)
, m_reserveL(false)
, m_prevAniPos(0.f)
{
	//m_state = NONE;
	//m_mode = NORMAL;
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

//	m_weapon->SetAnimation("..\\media\\valle(new)\\forward.ani");
}


bool cCharacter::Move(const float elapseTime)
{	
	bool bAniState = cModel::Move(elapseTime);  //�ִϸ��̼� ��� ����

	if(m_attackCnt > 0)  //���� ���� Ȯ��
	{
		Vector3 camDir( GetCamera()->GetDirection() );
		camDir.Normalize();
		float faniPos = (m_bone->GetPalette()[0]).GetPosition().z;
//		float faniPos = ((m_bone->GetRoot())->GetAccTM()).GetPosition().z;  //root�� accTM�� �̿��ص� �ɵ�ʹ�..
		float fcurrPos = faniPos - m_prevAniPos;
		m_prevAniPos = faniPos;
		fcurrPos = ::fabs(fcurrPos);
//		dbg::Print( "%f", fcurrPos );
		Matrix44 mat;
		if( fcurrPos >= 1  )
		{
//			mat.SetTranslate(  );
			GetCamera()->SetTranslation( Vector3( camDir.x, 0.f, camDir.z ) * fcurrPos );
		}
//		MultiplyTM( mat );
		//	Vector3 test( GetTM().GetPosition() );
		//	dbg::Print( "%f,%f,%f", test.x,test.y,test.z);
	
		
//		bool baniState = m_bone->GetAniState();

//		if(m_bone->GetAniState() == false)
		if(bAniState == false)
		{
			if(m_reserveL)
			{
				switch(m_attackCnt)
				{
				case 1:
					m_bone->SetAniLoop(false);
					SetAnimation( "..\\media\\valle\\valle_LLA.ani" );
					m_attackCnt++;
					m_reserveL = false;
			//		m_weapon->SetAniLoop(false);
			//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLA.ani");
				break;
				case 2:
					m_bone->SetAniLoop(false);
					SetAnimation( "..\\media\\valle\\valle_LLLA.ani" );
					m_attackCnt++;
					m_reserveL = false;
			//		m_weapon->SetAniLoop(false);
			//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLLA.ani");
				break;
				case 3:
					m_attackCnt = 0;
					m_bone->SetAniLoop(true);
					SetAnimation( "..\\media\\valle\\valle_normal.ani" );
					m_animode = false;
					m_reserveL = false;
			//		m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
				break;
				}
			}
			else
			{
				m_attackCnt = 0;
				m_bone->SetAniLoop(true);
				SetAnimation( "..\\media\\valle\\valle_normal.ani" );
				m_animode = false;
				m_reserveL = false;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
			}
		//	m_prevAniPos = Vector3(0.f,0.f,0.f);
			m_prevAniPos = 0.f;
		//	mat = GetTM();
		//	mat.SetIdentity();
			Vector3 test = GetCamera()->GetLook();
		//	test *= Vector3( camDir.x, 0.f, camDir.z );
			mat.SetTranslate( test - GetTM().GetPosition() );
			MultiplyTM( mat );
		}
	}

//	cModel::Move(elapseTime);
	
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

void cCharacter::Update(const short state, const float x)
{
	if(m_attackCnt > 0)  //���� ���� Ȯ��
	{
		if( (state != LATTACK && state != RATTACK) )  //���� Ű �̿��� Ű �Է½� ����
			return;
	}

	//���꿡 �ʿ��� �������� �� ��ü�� ����
	Matrix44 mat;
	Vector3 camDir( GetCamera()->GetDirection() );  //ī�޶� ���⺤��
	Vector3 camDirN( camDir.Normal() );  //ī�޶� ���� ��������
	Vector3 camR( GetCamera()->GetRight() );  //ī�޶� ��溤��
		
	switch( state )
	{
		case NONE:  //�⺻ ����
			if( m_animode )  //���� �ٸ� �ִϸ�� ���¶��..
			{	//�⺻������� �ѹ��� ����
				SetAnimation( "..\\media\\valle\\valle_normal.ani" );
				m_animode = false;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
			}
		break;

		case ROTATION:  //ĳ���� ȸ��
			{
				Quaternion q( Vector3(0,1,0), -x * 0.005f ); 
				Matrix44 m = q.GetMatrix();
				SetTM( m * GetTM() );  //R * T (�̵��� �� ȸ���ϴ°� ����)
			}
		break;
		
		case FORWARD:  //������ �̵�
			if( !m_animode )  //�Է�Ű�� ó�� �������ٸ�..
			{	//�ѹ��� ����
				SetAnimation( "..\\media\\valle\\valle_forward.ani" );
				m_animode = true;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 5.f );  //ī�޶� �ٶ󺸴� ��������
			MultiplyTM( mat );  //���� ��ġ�� �����ֱ�
			GetCamera()->SetPosition( GetTM() );  //ī�޶� ��ġ�� ����
		break;
		
		case BACKWARD:  //�ڷ� �̵�
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle\\valle_backward.ani" );
				m_animode = true;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_backward.ani");
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * -5.f );
			MultiplyTM( mat );
			GetCamera()->SetPosition( GetTM() );
		break;

		case LEFTWARD:  //���� �̵�
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle\\valle_forward.ani" );
				m_animode = true;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");

			}
			mat.SetTranslate( Vector3( camR.x, 0.f, camR.z ) * -5.f );  //ī�޶��� �¿��������
			MultiplyTM( mat );
			GetCamera()->SetPosition( GetTM() );
		break;

		case RIGHTWARD:  //������ �̵�
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle\\valle_forward.ani" );
				m_animode = true;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");

			}
			mat.SetTranslate( Vector3( camR.x, 0.f, camR.z ) * 5.f );
			MultiplyTM( mat );
			GetCamera()->SetPosition( GetTM() );
		break;

		case LATTACK:  //���콺 ��Ŭ��
			{
				if(m_attackCnt == 0)  //ù �����̶��..
				{
					m_bone->SetAniLoop(false);  //�ִ� �ݺ� ���� ����
					SetAnimation( "..\\media\\valle\\valle_LA.ani" );  //�ѹ��� ����
					m_attackCnt++;  //���� Ƚ�� ����
				//	m_weapon->SetAniLoop(false);
				//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");
				}
				else  //�ѹ� �̻� �����ϴ� ���¶��..
					m_reserveL = true;  //���� ����
			}
		break;
	}
}