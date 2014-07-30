//use debug
//	Vector3 test( GetTM().GetPosition() );
//	dbg::Print( "%f,%f,%f", test.x,test.y,test.z);

#include "stdafx.h"
//#include "..\stdafx.h"
#include "character.h"

using namespace graphic;


cCharacter::cCharacter(const int id) :
	cModel(id)
,	m_weapon(NULL)
,	m_weaponNode(NULL)
, m_reserveL(false)
, m_reserveR(false)
, m_prevAniPos(0.f)
{
//	m_animode = true;
//	m_state = NORMAL;
	m_attackCnt = 0;
//	m_countL = 0;
//	m_countR = 0;
	m_mode = NORMAL;
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

	if (m_weapon)// && m_weaponNode)
	{
	//	const Matrix44 mat = m_weaponNode->GetAccTM();
	//	m_weapon->SetTM(mat * m_matTM);
		m_weapon->Move(elapseTime);
	}

	if(m_mode >= LATTACK)  //���� ���� Ȯ��
	{
		Attack(bAniState);
	}

	return true;
}


void cCharacter::Render()
{
	cModel::Render();

	if (m_weapon)
		m_weapon->Render();
}

void cCharacter::Update(const short state, const float x, const float y)
{
	if(m_mode >= LATTACK)  //���� ���� Ȯ��
	{
		if( (state != LATTACK && state != RATTACK) )  //���� �̿��� Ű �Է½� ����
			return;
		else if( m_mode == RATTACK && state != RATTACK)
			return;
	}

	//���꿡 �ʿ��� �������� �� ��ü�� ����
	Matrix44 mat;
	Vector3 camDir( GetCamera()->GetDirection() );  //ī�޶� ���⺤��
	Vector3 camDirN( camDir.Normal() );  //ī�޶� ���� ��������
	Vector3 camR( GetCamera()->GetRight() );  //ī�޶� ��溤��
		
	switch( state )
	{
		case NORMAL:  //�⺻ ����
			if( m_mode != NORMAL )  //���� �ٸ� �ִϸ�� ���¶��..
			{	//�⺻������� �ѹ��� ����
				SetAnimation( "..\\media\\valle\\valle_normal.ani" );
				m_mode = NORMAL;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
			}
		break;

		case ROTATION:  //ĳ���� ȸ��
			{
				Quaternion q( Vector3(0,1,0), -x * 0.005f ); 
				Matrix44 m = q.GetMatrix();
				SetTM( m * GetTM() );  //R * T (�̵��� �� ȸ���ϴ°� ����)
				graphic::GetCamera()->SetRotation( x, y );  //ī�޶� ȸ���� ����
			}
		break;
		
		case FORWARD:  //������ �̵�
			if( m_mode != FORWARD )  //�Է�Ű�� ó�� �������ٸ�..
			{	//�ѹ��� ����
				SetAnimation( "..\\media\\valle\\valle_forward.ani" );
				m_mode = FORWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 5.f );  //ī�޶� �ٶ󺸴� ��������
			MultiplyTM( mat );  //���� ��ġ�� �����ֱ�
			GetCamera()->SetPosition( GetTM() );  //ī�޶� ��ġ�� ����
		break;
		
		case BACKWARD:  //�ڷ� �̵�
			if( m_mode != BACKWARD )
			{
				SetAnimation( "..\\media\\valle\\valle_backward.ani" );
				m_mode = BACKWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_backward.ani");
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * -5.f );
			MultiplyTM( mat );
			GetCamera()->SetPosition( GetTM() );
		break;

		case LEFTWARD:  //���� �̵�
			if( m_mode != LEFTWARD )
			{
				SetAnimation( "..\\media\\valle\\valle_forward.ani" );
				m_mode = LEFTWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");

			}
			mat.SetTranslate( Vector3( camR.x, 0.f, camR.z ) * -5.f );  //ī�޶��� �¿��������
			MultiplyTM( mat );
			GetCamera()->SetPosition( GetTM() );
		break;

		case RIGHTWARD:  //������ �̵�
			if( m_mode != RIGHTWARD )
			{
				SetAnimation( "..\\media\\valle\\valle_forward.ani" );
				m_mode = RIGHTWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");

			}
			mat.SetTranslate( Vector3( camR.x, 0.f, camR.z ) * 5.f );
			MultiplyTM( mat );
			GetCamera()->SetPosition( GetTM() );
		break;

		case DASH:
			if( m_mode != DASH )  //�Է�Ű�� ó�� �������ٸ�..
			{	//�ѹ��� ����
				SetAnimation( "..\\media\\valle\\valle_dash.ani" );
				m_mode = DASH;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 10.f );  //ī�޶� �ٶ󺸴� ��������
			MultiplyTM( mat );  //���� ��ġ�� �����ֱ�
			GetCamera()->SetPosition( GetTM() );  //ī�޶� ��ġ�� ����
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
					m_mode = LATTACK;
				}
				else  //�ѹ� �̻� �����ϴ� ���¶��..
				{
					m_reserveL = true;  //���� ����
					m_reserveR = false;
				}
			}
		break;

		case RATTACK:  //���콺 ����Ŭ��
			{
				if(m_attackCnt == 0)  //ù �����̶��..
				{
					m_bone->SetAniLoop(false);  //�ִ� �ݺ� ���� ����
					SetAnimation( "..\\media\\valle\\valle_RA.ani" );  //�ѹ��� ����
					m_attackCnt++;  //���� Ƚ�� ����
				//	m_weapon->SetAniLoop(false);
				//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");
					m_mode = RATTACK;
				}
				else  //�ѹ� �̻� �����ϴ� ���¶��..
				{
					m_reserveR = true;  //���� ����
					m_reserveL = false;
				}
			}
		break;
	}
}

bool cCharacter::Attack(bool bAniState)
{
	Vector3 camDir( GetCamera()->GetDirection() );
	camDir.Normalize();
	float fAniPos = (m_bone->GetPalette()[0]).GetPosition().z;
//	float fAniPos = ((m_bone->GetRoot())->GetAccTM()).GetPosition().z;  //root�� accTM�� �̿��ص� �ɵ�ʹ�..
	//	Vector3 test( GetTM().GetPosition() );
	//	dbg::Print( "%f,%f,%f", test.x,test.y,test.z);
	float fCurrPos = fAniPos - m_prevAniPos;
	m_prevAniPos = fAniPos;
	fCurrPos = ::fabs(fCurrPos);
	if( fCurrPos >= 0.1f  )
	{
		//	dbg::Print( "%f", fCurrPos);
		GetCamera()->SetTranslation( Vector3( camDir.x, 0.f, camDir.z ) * fCurrPos );
	//	GetCamera()->SetTranslation( fCurrPos );
	}
//		MultiplyTM( mat );
//		bool baniState = m_bone->GetAniState();
//		if(m_bone->GetAniState() == false)
	if(bAniState == false)
	{
				//	m_prevAniPos = Vector3(0.f,0.f,0.f);
		
	//	mat = GetTM();
	//	mat.SetIdentity();
		
	//	test *= Vector3( camDir.x, 0.f, camDir.z );		
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
				return true;
			break;
			case 2:
				m_bone->SetAniLoop(false);
				SetAnimation( "..\\media\\valle\\valle_LLLA.ani" );
				m_attackCnt++;
				m_reserveL = false;
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLLA.ani");
				return true;
			break;
			}
		}
		else if(m_reserveR)
		{
			switch(m_attackCnt)
			{
			case 1:
				m_bone->SetAniLoop(false);
				if(m_mode == RATTACK)
					SetAnimation( "..\\media\\valle\\valle_RRA.ani" );
				else if(m_mode == LATTACK)
				{
					SetAnimation( "..\\media\\valle\\valle_LRA.ani" );
					m_attackCnt++;  //�� �ִϵ��۱����� ����
				}
				m_attackCnt++;
				m_reserveR = false;
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLA.ani");
				return true;
			break;
			case 2:
				m_bone->SetAniLoop(false);
				if(m_mode == RATTACK)
					SetAnimation( "..\\media\\valle\\valle_RRRA.ani" );
				else if(m_mode == LATTACK)
					SetAnimation( "..\\media\\valle\\valle_LLRA.ani" );
				m_attackCnt++;
				m_reserveR = false;
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLLA.ani");
				return true;
			break;
			}
		}
		
		m_prevAniPos = 0.f;

		Vector3 finalPos = GetCamera()->GetLook();
		Matrix44 mat;
		mat.SetTranslate( finalPos - GetTM().GetPosition() );
		MultiplyTM( mat );
		
		m_attackCnt = 0;
		m_bone->SetAniLoop(true);
		SetAnimation( "..\\media\\valle\\valle_normal.ani" );
		m_mode = NORMAL;
		m_reserveL = false;
		m_reserveR = false;
	//	m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
		return true;
	}
	
	return false;
}