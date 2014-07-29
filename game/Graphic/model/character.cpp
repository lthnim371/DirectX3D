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
	bool bAniState = cModel::Move(elapseTime);  //애니메이션 결과 저장

	if(m_attackCnt > 0)  //공격 상태 확인
	{
		Vector3 camDir( GetCamera()->GetDirection() );
		camDir.Normalize();
		float faniPos = (m_bone->GetPalette()[0]).GetPosition().z;
//		float faniPos = ((m_bone->GetRoot())->GetAccTM()).GetPosition().z;  //root의 accTM을 이용해도 될듯싶다..
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
	if(m_attackCnt > 0)  //공격 상태 확인
	{
		if( (state != LATTACK && state != RATTACK) )  //공격 키 이외의 키 입력시 무시
			return;
	}

	//연산에 필요한 지역변수 및 객체들 생성
	Matrix44 mat;
	Vector3 camDir( GetCamera()->GetDirection() );  //카메라 방향벡터
	Vector3 camDirN( camDir.Normal() );  //카메라 방향 단위벡터
	Vector3 camR( GetCamera()->GetRight() );  //카메라 우방벡터
		
	switch( state )
	{
		case NONE:  //기본 동작
			if( m_animode )  //현재 다른 애니모션 상태라면..
			{	//기본모션으로 한번만 셋팅
				SetAnimation( "..\\media\\valle\\valle_normal.ani" );
				m_animode = false;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
			}
		break;

		case ROTATION:  //캐릭터 회전
			{
				Quaternion q( Vector3(0,1,0), -x * 0.005f ); 
				Matrix44 m = q.GetMatrix();
				SetTM( m * GetTM() );  //R * T (이동한 뒤 회전하는걸 방지)
			}
		break;
		
		case FORWARD:  //앞으로 이동
			if( !m_animode )  //입력키가 처음 눌러졌다면..
			{	//한번만 셋팅
				SetAnimation( "..\\media\\valle\\valle_forward.ani" );
				m_animode = true;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 5.f );  //카메라가 바라보는 방향으로
			MultiplyTM( mat );  //현재 위치에 더해주기
			GetCamera()->SetPosition( GetTM() );  //카메라 위치도 갱신
		break;
		
		case BACKWARD:  //뒤로 이동
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

		case LEFTWARD:  //왼쪽 이동
			if( !m_animode )
			{
				SetAnimation( "..\\media\\valle\\valle_forward.ani" );
				m_animode = true;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");

			}
			mat.SetTranslate( Vector3( camR.x, 0.f, camR.z ) * -5.f );  //카메라의 좌우방향으로
			MultiplyTM( mat );
			GetCamera()->SetPosition( GetTM() );
		break;

		case RIGHTWARD:  //오른쪽 이동
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

		case LATTACK:  //마우스 왼클릭
			{
				if(m_attackCnt == 0)  //첫 공격이라면..
				{
					m_bone->SetAniLoop(false);  //애니 반복 상태 끄기
					SetAnimation( "..\\media\\valle\\valle_LA.ani" );  //한번만 셋팅
					m_attackCnt++;  //공격 횟수 증가
				//	m_weapon->SetAniLoop(false);
				//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");
				}
				else  //한번 이상 공격하는 상태라면..
					m_reserveL = true;  //공격 예약
			}
		break;
	}
}