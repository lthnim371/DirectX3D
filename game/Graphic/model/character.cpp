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
//	m_weapon->SetBoneMgr( GetBoneMgr() );
	if(m_weapon)
	{
		m_weapon->GetBoneMgr()->SetPalette(m_bone->GetPalette());
		m_weapon->SetTM( GetTM() );
	}

//debug용	
	m_weapon->SetRenderBoundingBox(true);
}


bool cCharacter::Move(const float elapseTime)
{	
	bool bAniState = cModel::Move(elapseTime);  //애니메이션 결과 저장

/*
	if (m_weapon)// && m_weaponNode)
	{
	//	const Matrix44 mat = m_weaponNode->GetAccTM();
	//	m_weapon->SetTM(mat * m_matTM);
		m_weapon->Move(elapseTime);
	}
*/

	if(m_mode >= LATTACK)  //공격 상태 확인
	{
		Attack(bAniState);
	}

	if(m_weapon)
	{
		m_weapon->GetBoneMgr()->SetPalette(m_bone->GetPalette());
		m_weapon->SetTM( GetTM() );
	}

	return true;
}


void cCharacter::Render()
{
	cModel::Render();

	if (m_weapon)
		m_weapon->Render();
}

void cCharacter::RenderShader(cShader &shader)
{
	cModel::RenderShader(shader);

	//if (m_weapon)
	//	m_weapon->RenderShader(shader);

}

void cCharacter::Update(const short state, const float x, const float y)  //x = 0, y = 0
{
	if(m_mode >= LATTACK)  //공격 상태 확인
	{
		if( (state != LATTACK && state != RATTACK) )  //공격 이외의 키 입력시 무시
			return;
		else if( m_mode == RATTACK && state != RATTACK)
			return;
	}

	//연산에 필요한 지역변수 및 객체들 생성
	Matrix44 mat;
	Vector3 camDir( GetCamera()->GetDirection() );  //카메라 방향벡터
	Vector3 camDirN( camDir.Normal() );  //카메라 방향 단위벡터
	Vector3 camR( GetCamera()->GetRight() );  //카메라 우방벡터
		
	switch( state )
	{
		case NORMAL:  //기본 동작
			if( m_mode != NORMAL )  //현재 다른 애니모션 상태라면..
			{	//기본모션으로 한번만 셋팅
				SetAnimation( "..\\media\\valle\\valle_normal.ani" );
				m_mode = NORMAL;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
			}
		break;

		case ROTATION:  //캐릭터 회전
			{
				Quaternion q( Vector3(0,1,0), -x * 0.005f ); 
				Matrix44 m = q.GetMatrix();
				SetTM( m * GetTM() );  //R * T (이동한 뒤 회전하는걸 방지)
				GetCamera()->SetPosition( GetTM() );
				graphic::GetCamera()->SetRotation( x, y );  //카메라 회전도 갱신
			}
		break;
		
		case FORWARD:  //앞으로 이동
			if( m_mode != FORWARD )  //입력키가 처음 눌러졌다면..
			{	//한번만 셋팅
				SetAnimation( "..\\media\\valle\\valle_forward.ani" );
				m_mode = FORWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 5.f );  //카메라가 바라보는 방향으로
			MultiplyTM( mat );  //현재 위치에 더해주기
			GetCamera()->SetPosition( GetTM() );  //카메라 위치도 갱신
		break;
		
		case BACKWARD:  //뒤로 이동
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

		case LEFTWARD:  //왼쪽 이동
			if( m_mode != LEFTWARD )
			{
				SetAnimation( "..\\media\\valle\\valle_forward.ani" );
				m_mode = LEFTWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");

			}
			mat.SetTranslate( Vector3( camR.x, 0.f, camR.z ) * -5.f );  //카메라의 좌우방향으로
			MultiplyTM( mat );
			GetCamera()->SetPosition( GetTM() );
		break;

		case RIGHTWARD:  //오른쪽 이동
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
			if( m_mode != DASH )  //입력키가 처음 눌러졌다면..
			{	//한번만 셋팅
				SetAnimation( "..\\media\\valle\\valle_dash.ani" );
				m_mode = DASH;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 10.f );  //카메라가 바라보는 방향으로
			MultiplyTM( mat );  //현재 위치에 더해주기
			GetCamera()->SetPosition( GetTM() );  //카메라 위치도 갱신
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
					m_mode = LATTACK;
				}
				else  //한번 이상 공격하는 상태라면..
				{
					m_reserveL = true;  //공격 예약
					m_reserveR = false;
				}
			}
		break;

		case RATTACK:  //마우스 오른클릭
			{
				if(m_attackCnt == 0)  //첫 공격이라면..
				{
					m_bone->SetAniLoop(false);  //애니 반복 상태 끄기
					SetAnimation( "..\\media\\valle\\valle_RA.ani" );  //한번만 셋팅
					m_attackCnt++;  //공격 횟수 증가
				//	m_weapon->SetAniLoop(false);
				//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");
					m_mode = RATTACK;
				}
				else  //한번 이상 공격하는 상태라면..
				{
					m_reserveR = true;  //공격 예약
					m_reserveL = false;
				}
			}
		break;
	}

	if(m_weapon)
		m_weapon->SetTM( GetTM() );
}

bool cCharacter::Attack(bool bAniState)
{
	Vector3 camRight( GetCamera()->GetRight() );  //카메라 우방벡터 가져오기
	Vector3 camDir = camRight.CrossProduct(Vector3(0,1,0));  //방향벡터 구하기
	float fAniPos = (m_bone->GetPalette()[0]).GetPosition().z;  //현재 애니 동작의 이동한 값 가져오기
//	float fAniPos = ((m_bone->GetRoot())->GetAccTM()).GetPosition().z;  //root의 accTM을 이용해도 될듯싶다..
	float fCurrPos = fAniPos - m_prevAniPos;  //중첩되는 경우를 방지하고자 이전 값과 동일한지 판단
//	dbg::Print( "%f = %f - %f", fCurrPos, fAniPos, m_prevAniPos);
	m_prevAniPos = fAniPos;
	fCurrPos = ::fabs(fCurrPos);  //절대값으로 변환하여
	if( fCurrPos > MATH_EPSILON )  //값의 차이가 있을경우
	{
		//카메라가 바라보는 방향으로 (카메라 look을)차이값만큼 이동
		GetCamera()->SetTranslation( Vector3( camDir.x, 0.f, camDir.z ) * fCurrPos );
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
					m_attackCnt++;  //이 애니동작까지만 실행
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