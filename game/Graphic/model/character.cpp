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
	m_jumpCnt = 0;
//	m_countL = 0;
//	m_countR = 0;
	m_mode = NORMAL;
	m_prevLastFrame = 0;
	m_prevEndFrame = 0;
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
}

cCharacter::~cCharacter()
{
	SAFE_DELETE(m_weapon);
}


bool cCharacter::Create(const string &modelName)
{
	m_weaponNode = NULL;
	bool bResult = cModel::Create(modelName);
	if(!bResult)
		return bResult;

	m_characterCube = new cCube( Vector3(-25.f, 0.f, -25.f), Vector3(25.f, 175.f, 25.f) );
	
	return bResult;
}


void cCharacter::LoadWeapon(const string &fileName)
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
		UpdateJump(bAniState);

	if(m_weapon)
		UpdateWeapon();

	return true;
}


void cCharacter::Render()
{
	cModel::Render();

	if (m_weapon)
		m_weapon->Render();

//	if( m_weaponCube != NULL && m_cubeCheck == true )
//		m_weaponCube->Render( Matrix44() );

//test
	if(m_characterCube)
		m_characterCube->Render( Matrix44() );
	if(m_weaponCube)
		m_weaponCube->Render( Matrix44() );
}

void cCharacter::RenderShader(cShader &shader)
{
	cModel::RenderShader(shader);

	//if (m_weapon)
	//	m_weapon->RenderShader(shader);

}

void cCharacter::Update(const short state, const float x, const float y)  //x = 0, y = 0
{
	if(m_mode >= JUMP)  //공격 상태 확인
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
			mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * -5.f );  //카메라의 좌우방향으로
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
			mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * 5.f );
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
				if(m_attackCnt == 0 && m_jumpCnt == 0)  //첫 공격이라면..
				{
					m_bone->SetAniLoop(false);  //애니 반복 상태 끄기
					SetAnimation( "..\\media\\valle\\valle_LA.ani" );  //한번만 셋팅
					m_attackCnt++;  //공격 횟수 증가
					m_mode = LATTACK;

					m_cubeStartFrame = 10;
					m_cubeMaximumFrame = 13;

				//	m_weapon->SetAniLoop(false);
				//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");

		//			m_weaponCube = new cCube( Vector3(-50,0,0), Vector3(50,175,50) );
		//			m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
		//			m_cubePos *= 100.f;
				}
				else if( m_jumpCnt == 2 )
				{
					m_currJumpAttack = true;
					m_bone->SetAniLoop(false);
					SetAnimation( "..\\media\\valle\\valle_jump_LA.ani" );

					m_cubeStartFrame = 8;
					m_cubeMaximumFrame = 11;
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
					SetAnimation( "..\\media\\valle\\valle_RA.ani" );  //한번만 셋팅
					m_attackCnt++;  //공격 횟수 증가
					m_mode = RATTACK;

					m_cubeStartFrame = 15;
					m_cubeMaximumFrame = 18;
			//	m_weapon->SetAniLoop(false);
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");

			//		m_weaponCube = new cCube( Vector3(-125,0,0), Vector3(125,175,125) );
			//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
			//		m_cubePos *= 100.f;
				}
				else if(m_attackCnt == 0 && m_currJumpAttack == false)
				{
				}
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
				SetAnimation( "..\\media\\valle\\valle_jump1.ani" );
				m_mode = state;
				m_jumpCnt++;

				m_jumpSpeed = 10.f;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
		//	mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 10.f );  //카메라가 바라보는 방향으로
		//	MultiplyTM( mat );  //현재 위치에 더해주기
		//	GetCamera()->SetPosition( GetTM() );  //카메라 위치도 갱신
		break;
	}

	if(m_weapon)
		m_weapon->SetTM( GetTM() );
}

bool cCharacter::UpdateAttack(const bool bAniState)
{
	Vector3 camRight( GetCamera()->GetRight() );  //카메라 우방벡터 가져오기
	Vector3 camDir = camRight.CrossProduct(Vector3(0,1,0));  //방향벡터 구하기
	float fAniPos = (m_bone->GetPalette()[0]).GetPosition().z;  //현재 애니 동작의 이동한 값 가져오기
//	float fAniPos = ((m_bone->GetRoot())->GetAccTM()).GetPosition().z;  //root의 accTM을 이용해도 될듯싶다..
	float fCurrPos = fAniPos - m_prevAniPos;  //중첩되는 경우를 방지하고자 이전 값과 동일한지 판단
	m_prevAniPos = fAniPos;
	fCurrPos = ::fabs(fCurrPos);  //절대값으로 변환하여
	if( fCurrPos > MATH_EPSILON )  //값의 차이가 있을경우
	{
		//카메라가 바라보는 방향으로 (카메라 look을)차이값만큼 이동
		GetCamera()->SetTranslation( Vector3( camDir.x, 0.f, camDir.z ) * fCurrPos );
	}

	if(bAniState == false)
	{
		if(m_reserveL)
		{
			m_bone->SetAniLoop(false);
			m_reserveL = false;
			m_cubeCheck = false;

			switch(m_attackCnt)
			{
			case 1:
				
				SetAnimation( "..\\media\\valle\\valle_LLA.ani" );
				m_attackCnt++;

				m_cubeStartFrame = 8;
				m_cubeMaximumFrame = 11;
		//		m_weaponCube->SetCube( Vector3(-50,0,0), Vector3(50,200,200) );
		//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
		//		m_cubePos *= 100.f;
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLA.ani");
				return true;
			break;
			case 2:
				SetAnimation( "..\\media\\valle\\valle_LLLA.ani" );
				m_attackCnt++;

				m_cubeStartFrame = 18;
				m_cubeMaximumFrame = 21;
		//		m_weaponCube->SetCube( Vector3(-50,0,0), Vector3(50,200,200) );
		//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
		//		m_cubePos *= 100.f;
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLLA.ani");
				return true;
			break;
			}
		}
		else if(m_reserveR)
		{
			m_bone->SetAniLoop(false);
			m_reserveR = false;
			m_cubeCheck = false;

			switch(m_attackCnt)
			{
				case 1:
					if(m_mode == RATTACK)
					{
						SetAnimation( "..\\media\\valle\\valle_RRA.ani" );

						m_cubeStartFrame = 4;
						m_cubeMaximumFrame = 10;
				//		m_weaponCube->SetCube( Vector3(-175,0,0), Vector3(175,175,175) );
				//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
				//		m_cubePos *= 100.f;
					}
					else if(m_mode == LATTACK)
					{
						SetAnimation( "..\\media\\valle\\valle_LRA.ani" );
						m_attackCnt = 10;  //이 애니동작까지만 실행

						m_cubeStartFrame = 11;
						m_cubeMaximumFrame = 14;
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
					SetAnimation( "..\\media\\valle\\valle_RRRA.ani" );

					m_cubeStartFrame = 3;
					m_cubeMaximumFrame = 20;
			//		m_weaponCube->SetCube( Vector3(-200,0,0), Vector3(200,175,200) );
			//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
			//		m_cubePos *= 100.f;
				}
				else if(m_mode == LATTACK)
				{
					SetAnimation( "..\\media\\valle\\valle_LLRA.ani" );
					
					m_cubeStartFrame = 21;
					m_cubeMaximumFrame = 24;
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

		m_cubeStartFrame = 0;
		m_cubeMaximumFrame = 0;

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
	else if(bAniState == true)
	{
		short currFrame = m_bone->GetRoot()->GetCurrentFrame();
				
		if( m_cubeStartFrame <= currFrame && currFrame < m_cubeMaximumFrame )
		{
			m_cubeCheck = true;
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

void cCharacter::UpdateJump(const bool bAniState)
{
	Vector3 camDir( GetCamera()->GetDirection().Normal() );
	Vector3 camRight( GetCamera()->GetRight() );
	Matrix44 mat;
		
	if( bAniState == false )
	{			
		if( m_currJumpAttack )
		{
			m_currJumpAttack = false;
			m_prevJumpAttack = true;			
			m_bone->SetAniLoop(true);
			if(m_jumpSpeed > -7.5f)
				SetAnimation( "..\\media\\valle\\valle_jump2.ani" );
		}
		else if( m_jumpCnt == 1)
		{
			m_bone->SetAniLoop(true);
			SetAnimation( "..\\media\\valle\\valle_jump2.ani" );
			m_jumpCnt++;
		}
	}
	else if( m_currJumpAttack == true )
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
	
	if( m_jumpCnt == 2 && m_jumpSpeed <= -7.5f )
	{		
		switch( m_currJumpAttack )
		{
			case false:
				m_jumpCnt++;
				m_bone->SetAniLoop(false);
				SetAnimation( "..\\media\\valle\\valle_jump3.ani" );
			break;

			case true:
				mat = GetTM();
				float y = mat.GetPosition().y;

				if(y <= 10.f)
					return;
				else if(y > 10.f)
					m_jumpSpeed += 0.2f;
			break;
		}
	}
	m_jumpSpeed -= 0.2f;
	mat.SetTranslate( Vector3( 0.f, 1.f, 0.f ) * m_jumpSpeed );
	MultiplyTM( mat );
	GetCamera()->SetPosition( GetTM() );

	if( m_mode != JUMP )
	{
		switch(m_mode)
		{
			case FRONTJUMP:
				mat.SetTranslate( Vector3( camDir.x, 0.f, camDir.z ) * 5.f );
				MultiplyTM( mat );
			break;
			case BACKJUMP:
				mat.SetTranslate( Vector3( camDir.x, 0.f, camDir.z ) * -5.f );
				MultiplyTM( mat );
			break;
			case LEFTJUMP:
				mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * -5.f );
				MultiplyTM( mat );
			break;
			case RIGHTJUMP:
				mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * 5.f );
				MultiplyTM( mat );
			break;
		}
	}

	mat = GetTM();
	float y = mat.GetPosition().y;
	if(y <= 0.f)
	{
		mat._42 = 0.f;
		SetTM(mat);
		GetCamera()->SetPosition( GetTM() );
		m_jumpCnt = 0;
		m_bone->SetAniLoop(true);
		SetAnimation( "..\\media\\valle\\valle_normal.ani" );
		m_mode = NORMAL;
				
		m_currJumpAttack = false;
		m_prevJumpAttack = false;
		m_jumpSpeed = 0.f;

		m_cubeStartFrame = 0;
		m_cubeMaximumFrame = 0;
/*
		if(m_weaponCube)
		{
			delete m_weaponCube;
			m_weaponCube = NULL;
		}
*/
		return;
	}
}

void cCharacter::FindWeapon()
{
	//무기 bone 정보 가져오기
	vector<cBoneNode*>& weaponNode = m_weapon->GetBoneMgr()->GetAllBoneNode();

	for(auto it = weaponNode.begin(); it != weaponNode.end(); ++it)
	{
		//무기 bone 이름으로 캐릭터의 동일한 bone 찾아서 주소 가져오기
		cBoneNode* pNode = m_bone->FindBone( (*it)->GetName() );
		
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


	m_weaponCube.SetTransform( 
}

void cCharacter::GetWeaponBoundingBox()
{
	vector<cCube>& weapon = m_weapon->GetBoneMgr()->GetBoundingBox();

	for( auto it = weapon.begin(); it != weapon.end(); ++it )
	{
		Vector3 min = it->GetMin();
		Vector3 max = it->GetMin();
		if( min.IsEmpty() == false || max.IsEmpty() == false )
			m_weaponCube = &(*it);
	}
}

bool cCharacter::CollisionCheck( cCube& sourCube )
{
	cBoundingBox dest( *m_characterCube );
	cBoundingBox sour( sourCube );
	
	if( dest.Collision( sour ) )
	{
		m_hp -= 10;
		return true;
	}

	return false;
}