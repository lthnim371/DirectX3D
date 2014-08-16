//use debug
//	Vector3 test( GetTM().GetPosition() );
//	dbg::Print( "%f,%f,%f", test.x,test.y,test.z);

#include "stdafx.h"
//#include "..\stdafx.h"
#include "character.h"

using namespace graphic;

#define JUMPSPEED 10

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
	bool bAniState = cModel::Move(elapseTime);  //�ִϸ��̼� ��� ����

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

	if(m_mode == LATTACK || m_mode == RATTACK)  //���� ���� Ȯ��
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
	if(m_characterCube)
		m_characterCube->Render( Matrix44() );
	if(m_weaponCube)
		m_weaponCube->Render( Matrix44() );
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
	if(m_characterCube)
		m_characterCube->Render( Matrix44() );
	if(m_weaponCube)
		m_weaponCube->Render( Matrix44() );
}

void cCharacter::Update(const short state, const float x, const float y)  //x = 0, y = 0
{
	if( m_mode == BEHIT || m_mode == GUARD_BE_HIT )
		return;
	else if(m_mode >= JUMP)  //���� ���� Ȯ��
	{
		if( (state != LATTACK && state != RATTACK) )  //���� �̿��� Ű �Է½� ����
			return;
		else if( m_mode == RATTACK && state != RATTACK)
			return;
		else if( m_currJumpAttack || m_prevJumpAttack )
			return;
	}

	//���꿡 �ʿ��� �������� �� ��ü�� ����
	Matrix44 mat;
	Vector3 camRight( GetCamera()->GetRight() );  //ī�޶� ��溤��
	Vector3 camDir = camRight.CrossProduct(Vector3(0,1,0));  //ī�޶� ���⺤��
	Vector3 camDirN( camDir.Normal() );  //ī�޶� ���� ��������
		
	switch( state )
	{
		case NORMAL:  //�⺻ ����
			if( m_mode != NORMAL )  //���� �ٸ� �ִϸ�� ���¶��..
			{	//�⺻������� �ѹ��� ����
				SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
				m_mode = NORMAL;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
			}
		break;

		case ROTATION:  //ĳ���� ȸ��
		case LEFTROTATION:
		case RIGHTROTATION:
			{
				Quaternion q( Vector3(0,1,0), x * 0.005f ); 
				Matrix44 m = q.GetMatrix();
				SetTM( m * GetTM() );  //R * T (�̵��� �� ȸ���ϴ°� ����)
				GetCamera()->SetPosition( GetTM() );
				GetCamera()->SetRotation( x, y );  //ī�޶� ȸ���� ����
			}
		break;
		
		case FORWARD:  //������ �̵�
			if( m_mode != FORWARD )  //�Է�Ű�� ó�� �������ٸ�..
			{	//�ѹ��� ����
				SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
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
				SetAnimation( "..\\media\\ani\\valle\\valle1_backward.ani" );
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
				SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
				m_mode = LEFTWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");

			}
			mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * -5.f );  //ī�޶��� �¿��������
			MultiplyTM( mat );
			GetCamera()->SetPosition( GetTM() );
		break;

		case RIGHTWARD:  //������ �̵�
			if( m_mode != RIGHTWARD )
			{
				SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
				m_mode = RIGHTWARD;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");

			}
			mat.SetTranslate( Vector3( camRight.x, 0.f, camRight.z ) * 5.f );
			MultiplyTM( mat );
			GetCamera()->SetPosition( GetTM() );
		break;

		case DASH:
			if( m_mode != DASH )  //�Է�Ű�� ó�� �������ٸ�..
			{	//�ѹ��� ����
				SetAnimation( "..\\media\\ani\\valle\\valle1_dash.ani" );
				m_mode = DASH;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
			mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 10.f );  //ī�޶� �ٶ󺸴� ��������
			MultiplyTM( mat );  //���� ��ġ�� �����ֱ�
			GetCamera()->SetPosition( GetTM() );  //ī�޶� ��ġ�� ����
		break;
				
		case LATTACK:  //���콺 ��Ŭ��
			{
				if(m_attackCnt == 0 && m_jumpCnt == 0)  //ù �����̶��..
				{
					m_bone->SetAniLoop(false);  //�ִ� �ݺ� ���� ����
					SetAnimation( "..\\media\\ani\\valle\\valle1_LA.ani" );  //�ѹ��� ����
					m_attackCnt++;  //���� Ƚ�� ����
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
					SetAnimation( "..\\media\\ani\\valle\\valle1_JLA.ani" );

					m_cubeStartFrame = 8;
					m_cubeMaximumFrame = 11;
		//			m_weaponCube = new cCube( Vector3(-50,0,0), Vector3(50,200,150) );
		//			m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
		//			m_cubePos *= 100.f;
				}
				else //�ѹ� �̻� �����ϴ� ���¶��..
				{
					m_reserveL = true;  //���� ����
					m_reserveR = false;
				}
			}
		break;

		case RATTACK:  //���콺 ����Ŭ��
			{
				if(m_attackCnt == 0 && m_jumpCnt == 0)  //ù �����̶��..
				{
					m_bone->SetAniLoop(false);  //�ִ� �ݺ� ���� ����
					SetAnimation( "..\\media\\ani\\valle\\valle1_RA.ani" );  //�ѹ��� ����
					m_attackCnt++;  //���� Ƚ�� ����
					m_mode = RATTACK;

					m_cubeStartFrame = 15;
					m_cubeMaximumFrame = 18;
			//	m_weapon->SetAniLoop(false);
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");

			//		m_weaponCube = new cCube( Vector3(-125,0,0), Vector3(125,175,125) );
			//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
			//		m_cubePos *= 100.f;
				}
		//		else if(m_attackCnt == 0 && m_currJumpAttack == false)
		//		{
		//		}
				else  //�ѹ� �̻� �����ϴ� ���¶��..
				{
					m_reserveR = true;  //���� ����
					m_reserveL = false;
				}
			}
		break;

		case JUMP:
		case FRONTJUMP:
		case BACKJUMP:
		case LEFTJUMP:
		case RIGHTJUMP:
			if( m_mode != state )  //�Է�Ű�� ó�� �������ٸ�..
			{	
				m_bone->SetAniLoop(false);
				SetAnimation( "..\\media\\ani\\valle\\valle1_jump1.ani" );
				m_mode = state;
				m_jumpCnt++;

				m_jumpSpeed = JUMPSPEED;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
		//	mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 10.f );  //ī�޶� �ٶ󺸴� ��������
		//	MultiplyTM( mat );  //���� ��ġ�� �����ֱ�
		//	GetCamera()->SetPosition( GetTM() );  //ī�޶� ��ġ�� ����
		break;

		case GUARD:
			if( m_mode != state )
			{
				m_mode = GUARD;
				m_bone->SetAniLoop(true);
				SetAnimation( "..\\media\\ani\\valle\\valle1_guard_loop.ani" );
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
		Vector3 camRight( GetCamera()->GetRight() );  //ī�޶� ��溤�� ��������
		Vector3 camDir = camRight.CrossProduct(Vector3(0,1,0));  //���⺤�� ���ϱ�
		float fCurrAniPos = (m_bone->GetPalette()[0]).GetPosition().z;  //���� �ִ� ������ �̵��� �� ��������
	//	float fAniPos = ((m_bone->GetRoot())->GetAccTM()).GetPosition().z;  //root�� accTM�� �̿��ص� �ɵ�ʹ�..
		m_aniPosGap = fCurrAniPos - m_prevAniPos;  //��ø�Ǵ� ��츦 �����ϰ��� ���� ���� �������� �Ǵ�
		m_prevAniPos = fCurrAniPos;
		m_aniPosGap = ::fabs(m_aniPosGap);  //���밪���� ��ȯ�Ͽ�
		if( m_aniPosGap > MATH_EPSILON )  //���� ���̰� �������
		{
			//ī�޶� �ٶ󺸴� �������� (ī�޶� look��)���̰���ŭ �̵�
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

			switch(m_attackCnt)
			{
				case 1:
					if(m_mode == RATTACK)
					{
						SetAnimation( "..\\media\\ani\\valle\\valle1_RRA.ani" );

						m_cubeStartFrame = 4;
						m_cubeMaximumFrame = 10;
				//		m_weaponCube->SetCube( Vector3(-175,0,0), Vector3(175,175,175) );
				//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
				//		m_cubePos *= 100.f;
					}
					else if(m_mode == LATTACK)
					{
						SetAnimation( "..\\media\\ani\\valle\\valle1_LRA.ani" );
						m_attackCnt = 10;  //�� �ִϵ��۱����� ����

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
					SetAnimation( "..\\media\\ani\\valle\\valle1_RRRA.ani" );

					m_cubeStartFrame = 3;
					m_cubeMaximumFrame = 20;
			//		m_weaponCube->SetCube( Vector3(-200,0,0), Vector3(200,175,200) );
			//		m_cubePos = Vector3(camDir.x, 0.f, camDir.z);
			//		m_cubePos *= 100.f;
				}
				else if(m_mode == LATTACK)
				{
					SetAnimation( "..\\media\\ani\\valle\\valle1_LLRA.ani" );
					
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
		
	if( bAniState == false )
	{			
		if( m_currJumpAttack )
		{
			m_currJumpAttack = false;
			m_prevJumpAttack = true;			
		//	if(m_jumpSpeed > -7.5f)
			if( GetTM().GetPosition().y > 30.f )
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
	
	if( m_jumpCnt == 2 && //m_jumpSpeed <= -7.5f )
		m_jumpSpeed <= -7.5f )
	{		
		switch( m_currJumpAttack )
		{
			case false:
				m_jumpCnt++;
				m_bone->SetAniLoop(false);
				SetAnimation( "..\\media\\ani\\valle\\valle1_jump3.ani" );
			break;

			case true:
				mat = GetTM();
				float y = mat.GetPosition().y;

				if(y <= 10.f)
					return;
				else if(y > 10.f)
					m_jumpSpeed += ( 30.f * elapseTime );
			break;
		}
	}
//	m_jumpSpeed -= 0.2f;
	m_jumpSpeed -= ( 30.f * elapseTime );
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
		SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
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
	//���� bone ���� ��������
	vector<cBoneNode*>& weaponNode = m_weapon->GetBoneMgr()->GetAllBoneNode();

	for(auto it = weaponNode.begin(); it != weaponNode.end(); ++it)
	{
		//���� bone �̸����� ĳ������ ������ bone ã�Ƽ� �ּ� ��������
		cBoneNode* pNode = m_bone->FindBone( (*it)->GetName() );
		
	//	if( !pNode )
	//		continue;
		
		m_characterWeapon.push_back( pNode );  //������ �ּ� �����ϱ�
	}
}

void cCharacter::UpdateWeapon()
{
	//���� bone�� node, palette ���� ��������
	vector<cBoneNode*>& weaponNode = m_weapon->GetBoneMgr()->GetAllBoneNode();
	vector<Matrix44>& weaponPalette = m_weapon->GetBoneMgr()->GetPalette();
	
	for( int i = 0; i < (int)weaponNode.size(); ++i )  //���� bone accTM ����
	{
		if( m_characterWeapon[i] != NULL )
			weaponNode[i]->SetAccTM( m_characterWeapon[i]->GetAccTM() );
	}
	
	for( int i = 0; i < (int)weaponPalette.size(); ++i )  //���� bone palette ����
	{
		//���� bone�� id������ ã�Ƽ� ���� ��������
		if( m_characterWeapon[i] != NULL )
			weaponPalette[i] = ( m_bone->GetPalette() )[ m_characterWeapon[i]->GetId() ];
	}
	
	m_weapon->SetTM( GetTM() );  //���� ĳ���� ��ġ�� ���� ��ġ ����

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

bool cCharacter::CollisionCheck1( cCube& sourCube, const Vector3& sourPos, const Vector3& sourDir )
{
//	Vector3 MyDir( 0.f, 0.f, -1.f );  //�ӽ÷� ������ �ո� ���Բ� �����Ͽ���...
	Vector3 MyDir( m_camera->GetDirection().Normal() );  //���� ������� ī�޶� ����
	Vector3 destPos( GetTM().GetPosition() );  //���� ������� ��ġ

//cube�� boundingbox�� ��ȯ
	cBoundingBox destBox( *m_characterCube );
	cBoundingBox sourBox( sourCube );
	
	if( destBox.Collision( sourBox ) )  //�浹 Ȯ��..
	{
		Vector3 distanceDir( sourPos - destPos );   //������� �Ÿ�(����) �ľ�

		if( m_mode == GUARD )
		{
			if( MyDir.DotProduct( distanceDir.Normal() ) >= 0.2f )  //������ ���濡�� �����ߴٸ�..
			{
				m_mode = GUARD_BE_HIT;
				m_bone->SetAniLoop(false);
				SetAnimation( "..\\media\\ani\\valle\\valle1_guard_hit1.ani" );
				return true;
			}
		}
		
		//���� �ϴ� ���۰� ���õ� ������ �ʱ�ȭ
			m_mode = BEHIT;
			m_hp -= 10;
		
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
			if( MyDir.DotProduct( distanceDir.Normal() ) >= 0.f )  //������ ���濡�� �����ߴٸ�..
			{	
				SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front1.ani" );
			}
			else  //������ �Ĺ濡�� �����ߴٸ�..
			{
				SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back1.ani" );
			}

		return true;
	}  //if( destBox.Collision( sourBox ) )

	return false;
}

bool cCharacter::CollisionCheck2( cCube& sourCube, const Vector3& sourPos, const Vector3& sourDir )
{
//	Vector3 MyDir( 0.f, 0.f, -1.f );  //�ӽ÷� ������ �ո� ���Բ� �����Ͽ���...
	Vector3 MyDir( m_camera->GetDirection().Normal() );
	Vector3 destPos( GetTM().GetPosition() );

	Vector3 distanceDir( destPos - sourPos );  //���� ����ڿ� ������� �Ÿ�(����) �ľ�
	Vector3 newSourDir( sourDir.x, 0.f, sourDir.z );  //������ ī�޶���⺤���̱� ������ y���� �������־�� ��
	if( distanceDir.Normal().DotProduct( newSourDir.Normal() ) < 0.8f)  //������ �ٶ󺸴� ����� ���� ����� ������ �Ÿ� ������ ���� ����� ��쿡�� �浹 Ȯ��
		return false;  //������ �ٸ����� �����ϰ� �ִ� �����̴�...

	cBoundingBox destBox( *m_characterCube );
	cBoundingBox sourBox( sourCube );
	
	if( destBox.Collision( sourBox ) )		
		return true;

	return false;
}

void cCharacter::MoveControl(const bool bCtl)
{
	m_moveControl = bCtl;

	if( m_moveControl )  //���ݻ����� �� ������Ʈ�� ������ ���� ��ġ�� ������
	{
		Vector3 finalPos = GetCamera()->GetLook();
		Matrix44 mat;
		mat.SetTranslate( finalPos - GetTM().GetPosition() );
		MultiplyTM( mat );
	}

	m_bone->MoveControl(bCtl);
}