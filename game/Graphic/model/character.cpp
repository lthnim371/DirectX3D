//use debug
//	Vector3 test( GetTM().GetPosition() );
//	dbg::Print( "%f,%f,%f", test.x,test.y,test.z);

#include "stdafx.h"
//#include "..\stdafx.h"
#include "character.h"
#include "../../Framework/Sound.h"
#include <time.h>

using namespace graphic;

#define JUMPSPEED 30

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
	m_jumpCnt = 0;
//	m_countL = 0;
//	m_countR = 0;
	m_mode = NORMAL;
	m_aniPosGap = 0.f;
	m_prevAniPos = 0.f;
//	m_prevLastFrame = 0;
//	m_prevEndFrame = 0;
	m_currJumpAttack = false;
	m_prevJumpAttack = false;
	m_jumpSpeed = 0.f;
	m_weaponCube = NULL;
	m_cubeCheck = false;
	m_cubeStartFrame = 0;
	m_cubeMaximumFrame = 0;
//	m_cubeKeepCnt = 0;
	m_hp = 100;
	m_sp = 100;
	m_characterCube = NULL;
	m_weaponCubeNumber = 0;
	m_targetAttackCheck = false;
	m_moveControl = false;
	m_tick = 0.f;
	m_damage = 0.f;
	m_tick2 = 0.f;
}

cCharacter::~cCharacter()
{
	SAFE_DELETE(m_weapon);
	SAFE_DELETE(m_camera);
}


bool cCharacter::Create(const string &modelName)
{
	m_weaponNode = NULL;
	bool bResult = cModel::Create(modelName);
	if(!bResult)
		return bResult;

	m_characterCube = new cCube( Vector3(-35.f, 0.f, -35.f), Vector3(35.f, 175.f, 35.f) );
//	CreateCube();
//	m_characterCube = &m_cube;
//	SetRenderMesh(false);

	m_camera = new cCamera();

	::srand(unsigned int(NULL));
			
	return bResult;
}


void cCharacter::LoadWeapon(const string &fileName)//, const string &fileName2)
{
	SAFE_DELETE(m_weapon);

	RET(!m_bone);
/*
	m_weaponNode = m_bone->FindBone("dummy_weapon");
	m_weaponNode = m_bone->FindBone("Handle");
	m_weaponNode = m_bone->FindBone("HeadDummy");
	m_weaponNode = m_bone->FindBone("Bip01-L-Hand");
	RET(!m_weaponNode);
*/
	if (!m_weapon)
		m_weapon = new cModel(100);
	if (!m_weapon->Create(fileName))
		return;
	else if(m_weapon)
	{
		FindWeapon();
		GetWeaponBoundingBox();
	}
	
//	GetBoneMgr()->SwapBone( m_weapon->GetBoneMgr() );
//	m_weapon->SetBoneMgr( GetBoneMgr() );


//test
//	m_weapon->SetRenderBoundingBox(true);
//	m_weapon->SetRenderMesh(false);
}


bool cCharacter::Move(const float elapseTime)
{	
	bool bAniState = cModel::Move(elapseTime);  //애니메이션 결과 저장

	if(m_characterCube)
	{
/*
		cBoneNode* foundBone = m_bone->FindBone("Bip01-L-Clavicle");
		Matrix44 mat = foundBone->GetAccTM();
		Vector3 up = mat.GetPosition();
		foundBone = m_bone->FindBone("Bip01-R-Thigh");
		mat = foundBone->GetAccTM();
		Vector3 down = mat.GetPosition();
*/
		if( m_mode == LATTACK || m_mode == RATTACK )
		{
			Vector3 camLook( GetCamera()->GetLook() );
			Matrix44 currPos;
			currPos = GetTM();
			currPos._41 = camLook.x;
			currPos._42 = camLook.y;
			currPos._43 = camLook.z;
			m_characterCube->SetTransform( currPos );
		}
		else
			m_characterCube->SetTransform( GetTM() );
	}

	//	m_boundingBox.SetBoundingBox( Vector3(down.x, down.y, -50.f), Vector3(up.x, up.y, -50.f) );
//	m_boundingBox.SetTransform( GetTM() );

/*
	if (m_weapon)// && m_weaponNode)
	{
	//	const Matrix44 mat = m_weaponNode->GetAccTM();
	//	m_weapon->SetTM(mat * m_matTM);
		m_weapon->Move(elapseTime);
	}
*/

	if(m_mode == LATTACK || m_mode == RATTACK)  //공격 상태 확인
		UpdateAttack(bAniState);
	else if(m_mode >= JUMP && m_mode <= RIGHTJUMP)
		UpdateJump(bAniState, elapseTime);
	else if( m_mode == BEHIT && bAniState == false)
	{
		m_mode = NORMAL;
		m_bone->SetAniLoop(true);
		SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
	}
	else if(m_mode == GUARD_BE_HIT && bAniState == false)
	{
		m_mode = GUARD;
		m_bone->SetAniLoop(true);
		SetAnimation( "..\\media\\ani\\valle\\valle1_guard_loop.ani" );
	}

	if(m_weapon)
		UpdateWeapon();



	return bAniState;
}


void cCharacter::Render()
{
	cModel::Render();

	if (m_weapon)
		m_weapon->Render();

//	if( m_weaponCube != NULL && m_cubeCheck == true )
//		m_weaponCube->Render( Matrix44() );

//test
	if( m_isRenderBoundingBox )
	{
		if(m_characterCube)
			m_characterCube->Render( Matrix44() );
		if(m_weaponCube)
			m_weaponCube->Render( Matrix44() );
	}
/*
	if( m_font )
	{
		char buff[32];
		::_itoa_s( m_hp, buff, sizeof(buff), 10 );
		string str("HP : ");
		str.append( buff );
		sRect rect(10,740,110,840);
		m_font->DrawTextA( NULL, str.c_str(), -1, (RECT*)&rect,
			DT_NOCLIP, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );

		::_itoa_s( m_sp, buff, sizeof(buff), 10 );
		str.assign("SP : ");
		str.append( buff );
		rect.SetX(900);
		rect.SetY(740);
		m_font->DrawTextA( NULL, str.c_str(), -1, (RECT*)&rect,
			DT_NOCLIP, D3DXCOLOR( 1.0f, 0.2f, 0.0f, 1.0f ) );
	}
*/
}

void cCharacter::RenderShader(cShader &shader)
{	
	cModel::RenderShader(shader);

	if (m_weapon)
		m_weapon->RenderShader(shader);

//test
	if( m_isRenderBoundingBox )
	{
		if(m_characterCube)
			m_characterCube->Render( Matrix44() );
		if(m_weaponCube)
			m_weaponCube->Render( Matrix44() );
	}
}

void cCharacter::RenderShadow(cShader &shader)
{
	cModel::RenderShadow(shader);

	if (m_weapon)
		m_weapon->RenderShadow(shader);
}

void cCharacter::Update(const float elapseTime, const short state, const float x, const float y)  //x = 0, y = 0
{
	m_tick += elapseTime;
	m_tick2 += elapseTime;
	if( m_tick > 1 )
	{
		m_tick = 0.f;
		m_sp = m_sp >= 100 ? 100.f : m_sp + 0.5f;
	}

	if( m_mode == BEHIT || m_mode == GUARD_BE_HIT )
		return;
	else if(m_mode >= JUMP)  //공격 상태 확인
	{
		if( (state != LATTACK && state != RATTACK) )  //공격 이외의 키 입력시 무시
			return;
		else if( m_mode == RATTACK && state != RATTACK)
			return;
		else if( m_currJumpAttack || m_prevJumpAttack )
			return;
	}


	//연산에 필요한 지역변수 및 객체들 생성
	Matrix44 mat;
	Vector3 camRight( GetCamera()->GetRight() );  //카메라 우방벡터
	Vector3 camDir = camRight.CrossProduct(Vector3(0,1,0));  //카메라 방향벡터
	Vector3 camDirN( camDir.Normal() );  //카메라 방향 단위벡터
		
	switch( state )
	{
		case NORMAL:  //기본 동작
			if( m_mode != NORMAL )  //현재 다른 애니모션 상태라면..
			{	//기본모션으로 한번만 셋팅
				SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
				m_mode = NORMAL;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
			}
		break;

		case ROTATION:  //캐릭터 회전
		case LEFTROTATION:
		case RIGHTROTATION:
			{
				Quaternion q( Vector3(0,1,0), x * 0.005f ); 
				Matrix44 m = q.GetMatrix();
				SetTM( m * GetTM() );  //R * T (이동한 뒤 회전하는걸 방지)
				GetCamera()->SetPosition( GetTM() );
				GetCamera()->SetRotation( x, y );  //카메라 회전도 갱신
			}
		break;
		
		case FORWARD:  //앞으로 이동
			if( m_mode != FORWARD )  //입력키가 처음 눌러졌다면..
			{	//한번만 셋팅
				SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
				m_mode = FORWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
			if( m_tick2 * 30.f > 1.f )
			{
				m_tick2 = 0.f;
				mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * (25.f) );  //카메라가 바라보는 방향으로
				MultiplyTM( mat );  //현재 위치에 더해주기
				GetCamera()->SetPosition( GetTM() );  //카메라 위치도 갱신
			}
		break;
		
		case BACKWARD:  //뒤로 이동
			if( m_mode != BACKWARD )
			{
				SetAnimation( "..\\media\\ani\\valle\\valle1_backward.ani" );
				m_mode = BACKWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_backward.ani");
			}
			if( m_tick2 * 30.f > 1.f )
			{
				m_tick2 = 0.f;
				mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * (-25.f) );
				MultiplyTM( mat );
				GetCamera()->SetPosition( GetTM() );
			}
		break;

		case LEFTWARD:  //왼쪽 이동
			if( m_mode != LEFTWARD )
			{
				SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
				m_mode = LEFTWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");

			}
			if( m_tick2 * 30.f > 1.f )
			{
				m_tick2 = 0.f;
				mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * (-25.f) );  //카메라의 좌우방향으로
				MultiplyTM( mat );
				GetCamera()->SetPosition( GetTM() );
			}
		break;

		case RIGHTWARD:  //오른쪽 이동
			if( m_mode != RIGHTWARD )
			{
				SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
				m_mode = RIGHTWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");

			}
			if( m_tick2 * 30.f > 1.f )
			{
				m_tick2 = 0.f;
				mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * (25.f) );
				MultiplyTM( mat );
				GetCamera()->SetPosition( GetTM() );
			}
		break;

		case DASH:
			if( m_sp <= 0.05f )
			{
				if( m_mode != FORWARD )  //입력키가 처음 눌러졌다면..
				{	//한번만 셋팅
					SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
					m_mode = FORWARD;
				//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
				}
				if( m_tick2 * 30.f > 1.f )
				{
					m_tick2 = 0.f;
					mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * (25.f) );  //카메라가 바라보는 방향으로
					MultiplyTM( mat );  //현재 위치에 더해주기
					GetCamera()->SetPosition( GetTM() );  //카메라 위치도 갱신
				}
				break;
			}
			else if( m_mode != DASH )  //입력키가 처음 눌러졌다면..
			{	//한번만 셋팅
				SetAnimation( "..\\media\\ani\\valle\\valle1_dash.ani" );
				m_mode = DASH;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
			if( m_tick2 * 30.f > 1.f )
			{
				m_tick2 = 0.f;
				mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * (50.f) );  //카메라가 바라보는 방향으로
				MultiplyTM( mat );  //현재 위치에 더해주기
				GetCamera()->SetPosition( GetTM() );  //카메라 위치도 갱신
				m_sp -= ( 0.5f + elapseTime );
			}
		break;
				
		case LATTACK:  //마우스 왼클릭
			{
				if(m_attackCnt == 0 && m_jumpCnt == 0)  //첫 공격이라면..
				{
					m_bone->SetAniLoop(false);  //애니 반복 상태 끄기
					SetAnimation( "..\\media\\ani\\valle\\valle1_LA.ani" );  //한번만 셋팅
					m_attackCnt++;  //공격 횟수 증가
					m_mode = LATTACK;

					m_cubeStartFrame = 10;
					m_cubeMaximumFrame = 13;

					m_damage = 4.f;

				//	m_weapon->SetAniLoop(false);
				//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");

		//			m_weaponCube = new cCube( Vector3(-50,0,0), Vector3(50,175,50) );
		//			m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
		//			m_cubePos *= 100.f;
				}
				else if( 1 <= m_jumpCnt && m_jumpCnt <= 2 )
				{
					m_currJumpAttack = true;
					m_bone->SetAniLoop(false);
					SetAnimation( "..\\media\\ani\\valle\\valle1_JLA.ani" );

					m_cubeStartFrame = 8;
					m_cubeMaximumFrame = 11;

					m_damage = 10.f;

				/*	char cNumber[8];
					::_itoa_s( rand() % 4, cNumber, sizeof(cNumber) ,10);
					framework::SoundManager::Get()->get( string(cNumber) )->Play();*/
		//			m_weaponCube = new cCube( Vector3(-50,0,0), Vector3(50,200,150) );
		//			m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
		//			m_cubePos *= 100.f;
				}
				else //한번 이상 공격하는 상태라면..
				{
					m_reserveL = true;  //공격 예약
					m_reserveR = false;
				}
			}
		break;

		case RATTACK:  //마우스 오른클릭
			{
				if(m_attackCnt == 0 && m_jumpCnt == 0)  //첫 공격이라면..
				{
					m_bone->SetAniLoop(false);  //애니 반복 상태 끄기
					SetAnimation( "..\\media\\ani\\valle\\valle1_RA.ani" );  //한번만 셋팅
					m_attackCnt++;  //공격 횟수 증가
					m_mode = RATTACK;

					m_cubeStartFrame = 15;
					m_cubeMaximumFrame = 18;

					m_damage = 4.f;

				/*	char cNumber[8];
					::_itoa_s( rand() % 4, cNumber, sizeof(cNumber) ,10);
					framework::SoundManager::Get()->get( string(cNumber) )->Play();*/
			//	m_weapon->SetAniLoop(false);
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");

			//		m_weaponCube = new cCube( Vector3(-125,0,0), Vector3(125,175,125) );
			//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
			//		m_cubePos *= 100.f;
				}
		//		else if(m_attackCnt == 0 && m_currJumpAttack == false)
		//		{
		//		}
				else  //한번 이상 공격하는 상태라면..
				{
					m_reserveR = true;  //공격 예약
					m_reserveL = false;
				}
			}
		break;

		case JUMP:
		case FRONTJUMP:
		case BACKJUMP:
		case LEFTJUMP:
		case RIGHTJUMP:
			if( m_mode != state )  //입력키가 처음 눌러졌다면..
			{	
				m_bone->SetAniLoop(false);
				SetAnimation( "..\\media\\ani\\valle\\valle1_jump1.ani" );
				m_mode = state;
				m_jumpCnt++;

				m_jumpSpeed = JUMPSPEED;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
		//	mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 10.f );  //카메라가 바라보는 방향으로
		//	MultiplyTM( mat );  //현재 위치에 더해주기
		//	GetCamera()->SetPosition( GetTM() );  //카메라 위치도 갱신
		break;

		case GUARD:
			if( m_mode != state )
			{
				m_mode = GUARD;
				m_bone->SetAniLoop(true);
				SetAnimation( "..\\media\\ani\\valle\\valle1_guard_loop.ani" );
				if( ::GetForegroundWindow() == ::GetFocus() )
					framework::SoundManager::Get()->get( "Guard" )->Play();
			}
		break;
	}

	if(m_weapon)
		m_weapon->SetTM( GetTM() );
}

bool cCharacter::UpdateAttack(const bool bAniState)
{
	if( !m_moveControl )
	{
		Vector3 camRight( GetCamera()->GetRight() );  //카메라 우방벡터 가져오기
		Vector3 camDir = camRight.CrossProduct(Vector3(0,1,0));  //방향벡터 구하기
		float fCurrAniPos = (m_bone->GetPalette()[0]).GetPosition().z;  //현재 애니 동작의 이동한 값 가져오기
	//	float fAniPos = ((m_bone->GetRoot())->GetAccTM()).GetPosition().z;  //root의 accTM을 이용해도 될듯싶다..
		m_aniPosGap = fCurrAniPos - m_prevAniPos;  //중첩되는 경우를 방지하고자 이전 값과 동일한지 판단
		m_prevAniPos = fCurrAniPos;
		m_aniPosGap = ::fabs(m_aniPosGap);  //절대값으로 변환하여
		if( m_aniPosGap > MATH_EPSILON )  //값의 차이가 있을경우
		{
			//카메라가 바라보는 방향으로 (카메라 look을)차이값만큼 이동
			GetCamera()->SetTranslation( Vector3( camDir.x, 0.f, camDir.z ) * m_aniPosGap );
		}
	}

	if(bAniState == false)
	{
		m_cubeCheck = false;
		m_targetAttackCheck = false;

		if(m_reserveL)
		{
			m_bone->SetAniLoop(false);
			m_reserveL = false;

			switch(m_attackCnt)
			{
			case 1:
				
				SetAnimation( "..\\media\\ani\\valle\\valle1_LLA.ani" );
				m_attackCnt++;

				m_cubeStartFrame = 8;
				m_cubeMaximumFrame = 11;

				m_damage = 8.f;
		//		m_weaponCube->SetCube( Vector3(-50,0,0), Vector3(50,200,200) );
		//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
		//		m_cubePos *= 100.f;
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLA.ani");
				return true;
			break;
			case 2:
				SetAnimation( "..\\media\\ani\\valle\\valle1_LLLA.ani" );
				m_attackCnt++;

				m_cubeStartFrame = 18;
				m_cubeMaximumFrame = 21;

				m_damage = 12.f;
		//		m_weaponCube->SetCube( Vector3(-50,0,0), Vector3(50,200,200) );
		//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
		//		m_cubePos *= 100.f;
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLLA.ani");
				return true;
			break;
			}

		/*	char cNumber[8];
			::_itoa_s( rand() % 4, cNumber, sizeof(cNumber) ,10);
			framework::SoundManager::Get()->get( string(cNumber) )->Play();*/
		}
		else if(m_reserveR)
		{
			m_bone->SetAniLoop(false);
			m_reserveR = false;

			switch(m_attackCnt)
			{
				case 1:
					if(m_mode == RATTACK)
					{
						SetAnimation( "..\\media\\ani\\valle\\valle1_RRA.ani" );

						m_cubeStartFrame = 4;
						m_cubeMaximumFrame = 10;

						m_damage = 7.f;
				//		m_weaponCube->SetCube( Vector3(-175,0,0), Vector3(175,175,175) );
				//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
				//		m_cubePos *= 100.f;
					}
					else if(m_mode == LATTACK)
					{
						SetAnimation( "..\\media\\ani\\valle\\valle1_LRA.ani" );
						m_attackCnt = 10;  //이 애니동작까지만 실행

						m_cubeStartFrame = 11;
						m_cubeMaximumFrame = 14;

						m_damage = 10.f;
				//		m_weaponCube->SetCube( Vector3(-50,0,0), Vector3(50,200,200) );
				//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
				//		m_cubePos *= 100.f;
					}
				m_attackCnt++;
				
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLA.ani");
				return true;
			break;
			case 2:
				if(m_mode == RATTACK)
				{
					SetAnimation( "..\\media\\ani\\valle\\valle1_RRRA.ani" );

					m_cubeStartFrame = 3;
					m_cubeMaximumFrame = 20;

					m_damage = 10.f;
			//		m_weaponCube->SetCube( Vector3(-200,0,0), Vector3(200,175,200) );
			//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
			//		m_cubePos *= 100.f;
				}
				else if(m_mode == LATTACK)
				{
					SetAnimation( "..\\media\\ani\\valle\\valle1_LLRA.ani" );
					
					m_cubeStartFrame = 21;
					m_cubeMaximumFrame = 24;

					m_damage = 15.f;
			//		m_weaponCube->SetCube( Vector3(-50,0,0), Vector3(50,200,200) );
			//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
			//		m_cubePos *= 100.f;
				}
				m_attackCnt++;
		//		m_reserveR = false;
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLLA.ani");
				return true;
			break;
			}

		/*	char cNumber[8];
			::_itoa_s( rand() % 4, cNumber, sizeof(cNumber) ,10);
			framework::SoundManager::Get()->get( string(cNumber) )->Play();*/
		}
		
		m_prevAniPos = 0.f;

		Vector3 finalPos = GetCamera()->GetLook();
		Matrix44 mat;
		mat.SetTranslate( finalPos - GetTM().GetPosition() );
		MultiplyTM( mat );
		
		m_attackCnt = 0;
		m_bone->SetAniLoop(true);
		SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
		m_mode = NORMAL;
		m_reserveL = false;
		m_reserveR = false;

		m_cubeStartFrame = 0;
		m_cubeMaximumFrame = 0;

	//	if( GetBoneMgr()->GetRoot()->GetMoveControl() )
	//		GetBoneMgr()->MoveControl(false);
		MoveControl(false);
		
	//	m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
/*
		m_cubeCheck = false;
		if(m_weaponCube)
		{
			delete m_weaponCube;
			m_weaponCube = NULL;
		}
*/
		return true;
	}
	else if( m_targetAttackCheck == false )
	{
		short currFrame = m_bone->GetRoot()->GetCurrentFrame();
				
		if( m_cubeStartFrame <= currFrame && currFrame < m_cubeMaximumFrame )
		{
			m_cubeCheck = true;

//			SndDepot
	/*
			Vector3 currPos = GetCamera()->GetLook();
			currPos += m_cubePos;
			Matrix44 mat;
			mat = GetTM();
			mat._41 = currPos.x;
			mat._43 = currPos.z;
		//	mat.SetTranslate( currPos + m_cubePos );
			m_weaponCube->SetTransform( mat );
	*/
		}
		else if( currFrame >= m_cubeMaximumFrame )
		{
			m_cubeCheck = false;
		}
	}
	
	return false;
}

void cCharacter::UpdateJump(const bool bAniState, const float elapseTime)
{

	Vector3 camDir( GetCamera()->GetDirection().Normal() );
	Vector3 camRight( GetCamera()->GetRight() );
	Matrix44 mat;
	float y = GetTM().GetPosition().y;
		
	if( bAniState == false )
	{			
		if( m_currJumpAttack )
		{
			m_jumpCnt = m_jumpCnt == 1 ? 2 : m_jumpCnt;
			m_currJumpAttack = false;
			m_prevJumpAttack = true;			
		//	if(m_jumpSpeed > -7.5f)
			if( y > 30.f )//GetTM().GetPosition().y
			{
				m_bone->SetAniLoop(true);
				SetAnimation( "..\\media\\ani\\valle\\valle1_jump2.ani" );
			}
		}
		else if( m_jumpCnt == 1)
		{
			m_bone->SetAniLoop(true);
			SetAnimation( "..\\media\\ani\\valle\\valle1_jump2.ani" );
			m_jumpCnt++;
		}
	}
	else if( m_currJumpAttack == true && m_targetAttackCheck == false)
	{
		short currFrame = m_bone->GetRoot()->GetCurrentFrame();
				
		if( m_cubeStartFrame <= currFrame && currFrame < m_cubeMaximumFrame )
		{
			m_cubeCheck = true;
/*
			Matrix44 mat;
			mat.SetTranslate( m_cubePos );
			m_weaponCube->SetTransform( GetTM() * mat );
*/
		}
		else if( currFrame >= m_cubeMaximumFrame )
		{
			m_cubeCheck = false;
		}
	}
	else if(y < 0.f)
	{

		mat = GetTM();
		mat._42 = 0.f;
		SetTM(mat);
		GetCamera()->SetPosition( GetTM() );
		m_jumpCnt = 0;
		m_bone->SetAniLoop(true);
		SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
		m_mode = NORMAL;
				
		m_currJumpAttack = false;
		m_prevJumpAttack = false;
		m_jumpSpeed = 0.f;

		m_cubeStartFrame = 0;
		m_cubeMaximumFrame = 0;

		m_moveControl = false;
		m_targetAttackCheck = false;
/*
		if(m_weaponCube)
		{
			delete m_weaponCube;
			m_weaponCube = NULL;
		}
*/
		return;
	}

	if( m_tick2 * 30.f < 1.f )
		return;

	m_tick2 = 0.f;
	
	m_jumpSpeed = y >= 350.f ? -MATH_EPSILON : m_jumpSpeed;

	if( m_jumpCnt != 3 && m_jumpSpeed < 0 && y <= 15.f )
	{		
		switch( m_currJumpAttack )
		{
			case false:
				m_jumpCnt++;
				m_bone->SetAniLoop(false);
				SetAnimation( "..\\media\\ani\\valle\\valle1_jump3.ani" );
			break;

			case true:
				return;
			/*	mat = GetTM();
				float y = mat.GetPosition().y;

				if(y <= 10.f)
					return;
				else if(y > 10.f)
					m_jumpSpeed += ( 0.1f + elapseTime );
			break;	*/
		}
	}
		m_jumpSpeed -= (1.5f);
		mat.SetTranslate( Vector3( 0.f, 1.f, 0.f ) * m_jumpSpeed );
		MultiplyTM( mat );
		GetCamera()->SetPosition( GetTM() );

	if( m_mode != JUMP && m_moveControl == false )
	{
	//	if( m_tick2 * 30.f > 1.f )
	//	{
		//	m_tick2 = 0.f;
			switch(m_mode)
			{
				case FRONTJUMP:
					mat.SetTranslate( Vector3( camDir.x, 0.f, camDir.z ) * 20.f );
				//	mat.SetTranslate( Vector3( camDir.x, 0.f, camDir.z ) * ( (100 * elapseTime) * 10.f) );
					MultiplyTM( mat );
				break;
				case BACKJUMP:
					mat.SetTranslate( Vector3( camDir.x, 0.f, camDir.z ) * -20.f );
				//	mat.SetTranslate( Vector3( camDir.x, 0.f, camDir.z ) * ( (100 * elapseTime) * -10.f) );
					MultiplyTM( mat );
				break;
				case LEFTJUMP:
					mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * -20.f );
				//	mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * ( (100 * elapseTime) * -10.f) );
					MultiplyTM( mat );
				break;
				case RIGHTJUMP:
					mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * 20.f );
				//	mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * ( (100 * elapseTime) * 10.f) );
					MultiplyTM( mat );
				break;
	//		}
		}
	}

//	mat = GetTM();
//	float y = mat.GetPosition().y;
}

void cCharacter::UpdateBeHit(const bool bAniState, const Vector3& sourDir, const float fAniPosGap)
{
	Matrix44 mat;
	Vector3 newSourDir( Vector3(sourDir.x, 0.f, sourDir.z) );
	mat.SetTranslate( newSourDir.Normal() * fAniPosGap );
	MultiplyTM( mat );
	m_camera->SetPosition( GetTM() );

	if(m_weapon)
		m_weapon->SetTM( GetTM() );
//		UpdateWeapon();

/*
	if( bAniState == false )
	{
		m_mode = NORMAL;
		m_bone->SetAniLoop(true);
		SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
	}
	else if( bAniState == true )
	{

	}
*/	
}

void cCharacter::FindWeapon()
{
	//무기 bone 정보 가져오기
	vector<cBoneNode*>& weaponNode = m_weapon->GetBoneMgr()->GetAllBoneNode();

//	for(auto it = weaponNode.begin(); it != weaponNode.end(); ++it)
	BOOST_FOREACH(auto it, weaponNode)
	{
		//무기 bone 이름으로 캐릭터의 동일한 bone 찾아서 주소 가져오기
		cBoneNode* pNode = m_bone->FindBone( (*it).GetName() );
		
	//	if( !pNode )
	//		continue;
		
		m_characterWeapon.push_back( pNode );  //가져온 주소 보관하기
	}
}

void cCharacter::UpdateWeapon()
{
	//무기 bone의 node, palette 정보 가져오기
	vector<cBoneNode*>& weaponNode = m_weapon->GetBoneMgr()->GetAllBoneNode();
	vector<Matrix44>& weaponPalette = m_weapon->GetBoneMgr()->GetPalette();
	
	for( int i = 0; i < (int)weaponNode.size(); ++i )  //무기 bone accTM 갱신
	{
		if( m_characterWeapon[i] != NULL )
			weaponNode[i]->SetAccTM( m_characterWeapon[i]->GetAccTM() );
	}
	
	for( int i = 0; i < (int)weaponPalette.size(); ++i )  //무기 bone palette 갱신
	{
		//무기 bone의 id값으로 찾아서 값을 가져오기
		if( m_characterWeapon[i] != NULL )
			weaponPalette[i] = ( m_bone->GetPalette() )[ m_characterWeapon[i]->GetId() ];
	}
	
	m_weapon->SetTM( GetTM() );  //현재 캐릭터 위치로 무기 위치 갱신

	m_weaponCube->SetTransform( ( weaponNode[m_weaponCubeNumber]->GetAccTM() ) * ( m_weapon->GetTM() ) );
}

void cCharacter::GetWeaponBoundingBox()
{
	vector<cCube>& weaponCube = m_weapon->GetBoneMgr()->GetCube();
	
	for( int i = 0; i< (int)weaponCube.size(); ++i )
	{
		Vector3 min = weaponCube[i].GetMin();
		Vector3 max = weaponCube[i].GetMax();
		if( min.IsEmpty() == false || max.IsEmpty() == false )
		{
			m_weaponCube = &(weaponCube[i]);
			m_weaponCubeNumber = i;
		}
	}
}

bool cCharacter::CollisionCheck1( cCube& sourCube, const Vector3& sourPos, const float& damage, const Vector3& sourDir )
{
//	Vector3 MyDir( 0.f, 0.f, -1.f );  //임시로 무조건 앞만 보게끔 설정하였음...
	Vector3 MyDir( m_camera->GetDirection().Normal() );  //현재 사용자의 카메라 방향
	Vector3 destPos( GetTM().GetPosition() );  //현재 사용자의 위치

//cube를 boundingbox로 변환
	cBoundingBox destBox( *m_characterCube );
	cBoundingBox sourBox( sourCube );
	
	if( destBox.Collision( sourBox ) )  //충돌 확인..
	{
		Vector3 distanceDir( sourPos - destPos );   //상대방과의 거리(방향) 파악

		if( m_mode == GUARD )
		{
			if( MyDir.DotProduct( distanceDir.Normal() ) >= 0.2f )  //상대방이 전방에서 공격했다면..
			{
				m_mode = GUARD_BE_HIT;
				m_bone->SetAniLoop(false);
				SetAnimation( "..\\media\\ani\\valle\\valle1_guard_hit1.ani" );
				m_hp -= damage * 0.5f;
				return true;
			}
		}
		
		//원래 하던 동작과 관련된 변수들 초기화
			m_mode = BEHIT;
			m_hp -= damage;
		
			m_prevAniPos = 0.f;
			m_aniPosGap = 0.f;
			m_attackCnt = 0;
			m_jumpCnt = 0;
			m_currJumpAttack = 0;
			m_prevJumpAttack = 0;
		//	m_jumpSpeed 
			m_cubeStartFrame = 0;
			m_cubeMaximumFrame = 0;
			m_reserveL = false;
			m_reserveR = false;
			m_cubeCheck = false;

//			Vector3 finalPos = GetCamera()->GetLook();
		//	Matrix44 mat;
		//	mat.SetTranslate( sourPos - destPos );
		//	MultiplyTM( mat );
	//		Vector3 sourPos( sourTM.GetPosition() );		
		
			m_bone->SetAniLoop(false);
	//		if( destPos.Normal().DotProduct( sourPos.Normal() ) >= 0.f )
			if( MyDir.DotProduct( distanceDir.Normal() ) >= 0.f )  //상대방이 전방에서 공격했다면..
			{	
				SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front1.ani" );
			}
			else  //상대방이 후방에서 공격했다면..
			{
				SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back1.ani" );
			}
			/*if( ::GetForegroundWindow() == ::GetFocus() )
			{
				char cNumber[8];
				::_itoa_s( (rand() % 4) + 10, cNumber, sizeof(cNumber) ,10);
				framework::SoundManager::Get()->get( string(cNumber) )->Play();
			}*/

		return true;
	}  //if( destBox.Collision( sourBox ) )

	return false;
}

bool cCharacter::CollisionCheck2( cCube& sourCube, const Vector3& sourPos, const Vector3& sourDir )
{
//	Vector3 MyDir( 0.f, 0.f, -1.f );  //임시로 무조건 앞만 보게끔 설정하였음...
	Vector3 MyDir( m_camera->GetDirection().Normal() );
	Vector3 destPos( GetTM().GetPosition() );

	Vector3 distanceDir( destPos - sourPos );  //현재 사용자와 상대방과의 거리(방향) 파악
	Vector3 newSourDir( sourDir.x, 0.f, sourDir.z );  //상대방의 카메라방향벡터이기 때문에 y값을 제거해주어야 함
	if( distanceDir.Normal().DotProduct( newSourDir.Normal() ) < 0.8f)  //상대방이 바라보는 방향과 실제 상대방과 나와의 거리 방향이 거의 비슷할 경우에만 충돌 확인
		return false;  //상대방이 다른곳을 공격하고 있는 상태이다...

	cBoundingBox destBox( *m_characterCube );
	cBoundingBox sourBox( sourCube );
	
	if( destBox.Collision( sourBox ) )		
		return true;

	return false;
}

void cCharacter::MoveControl(const bool bCtl, const bool bOnlyJump)
{
	m_moveControl = bCtl;

	if( bOnlyJump )
		return;
	else if( m_moveControl )  //공격상태일 때 오브젝트에 닿으면 최종 위치를 결정함
	{
		Vector3 finalPos = GetCamera()->GetLook();
		Matrix44 mat;
		mat.SetTranslate( finalPos - GetTM().GetPosition() );
		MultiplyTM( mat );
	}

	m_bone->MoveControl(bCtl);
}

//test
void cCharacter::SetDrawCube(const bool bCubeDraw)
{
	SetRenderBoundingBox( bCubeDraw );
	m_weapon->SetRenderBoundingBox( bCubeDraw );
}

void cCharacter::UpdatePosition()
{

	Matrix44 mat;
	mat.SetTranslate( GetCamera()->GetLook() - GetTM().GetPosition() );
	MultiplyTM( mat );
	m_characterCube->SetTransform( GetTM() );
	UpdateWeapon();

//	GetCamera()->SetPosition( GetTM() );
}