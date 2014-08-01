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
	m_prevFrame = 0;
	m_jumpAttack = false;
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
		FindWeapon();

//debug��	
	m_weapon->SetRenderBoundingBox(true);
}


bool cCharacter::Move(const float elapseTime)
{	
	bool bAniState = cModel::Move(elapseTime);  //�ִϸ��̼� ��� ����

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
}

void cCharacter::RenderShader(cShader &shader)
{
	cModel::RenderShader(shader);

	//if (m_weapon)
	//	m_weapon->RenderShader(shader);

}

void cCharacter::Update(const short state, const float x, const float y)  //x = 0, y = 0
{
	if(m_mode >= JUMP)  //���� ���� Ȯ��
	{
		if( (state != LATTACK && state != RATTACK) )  //���� �̿��� Ű �Է½� ����
			return;
		else if( m_mode == RATTACK && state != RATTACK)
			return;
		else if( m_jumpAttack || m_jumpCnt == 1 || m_jumpCnt == 4)
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
				GetCamera()->SetPosition( GetTM() );
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
				if(m_mode != LATTACK)  //ù �����̶��..
				{
					m_bone->SetAniLoop(false);  //�ִ� �ݺ� ���� ����
					SetAnimation( "..\\media\\valle\\valle_LA.ani" );  //�ѹ��� ����
					m_attackCnt++;  //���� Ƚ�� ����
				//	m_weapon->SetAniLoop(false);
				//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");
					m_mode = LATTACK;
				}
				else if(!m_jumpAttack)
				{
					m_jumpAttack = true;
					int endFrame = m_bone->GetRoot()->GetEndFrame();
					int currFrame = m_bone->GetRoot()->GetCurrentFrame();
					m_prevFrame = ( currFrame == 0 ? 0 : (endFrame - currFrame) );
					m_bone->SetAniLoop(false);
					SetAnimation( "..\\media\\valle\\valle_jump_LA.ani" );
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
				if(m_mode != RATTACK)  //ù �����̶��..
				{
					m_bone->SetAniLoop(false);  //�ִ� �ݺ� ���� ����
					SetAnimation( "..\\media\\valle\\valle_RA.ani" );  //�ѹ��� ����
					m_attackCnt++;  //���� Ƚ�� ����
				//	m_weapon->SetAniLoop(false);
				//	m_weapon->SetAnimation("..\\media\\valle\\valle_LA.ani");
					m_mode = RATTACK;
				}
				else if(m_mode == JUMP)
				{
				}
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
				SetAnimation( "..\\media\\valle\\valle_jump1.ani" );
				m_mode = state;
				m_jumpCnt++;
			//	m_weapon->SetAnimation("..\\media\\valle\\valle_forward.ani");
			}
		//	mat.SetTranslate( Vector3( camDirN.x, 0.f, camDirN.z ) * 10.f );  //ī�޶� �ٶ󺸴� ��������
		//	MultiplyTM( mat );  //���� ��ġ�� �����ֱ�
		//	GetCamera()->SetPosition( GetTM() );  //ī�޶� ��ġ�� ����
		break;
	}

	if(m_weapon)
		m_weapon->SetTM( GetTM() );
}

bool cCharacter::UpdateAttack(const bool bAniState)
{
	Vector3 camRight( GetCamera()->GetRight() );  //ī�޶� ��溤�� ��������
	Vector3 camDir = camRight.CrossProduct(Vector3(0,1,0));  //���⺤�� ���ϱ�
	float fAniPos = (m_bone->GetPalette()[0]).GetPosition().z;  //���� �ִ� ������ �̵��� �� ��������
//	float fAniPos = ((m_bone->GetRoot())->GetAccTM()).GetPosition().z;  //root�� accTM�� �̿��ص� �ɵ�ʹ�..
	float fCurrPos = fAniPos - m_prevAniPos;  //��ø�Ǵ� ��츦 �����ϰ��� ���� ���� �������� �Ǵ�
//	dbg::Print( "%f = %f - %f", fCurrPos, fAniPos, m_prevAniPos);
	m_prevAniPos = fAniPos;
	fCurrPos = ::fabs(fCurrPos);  //���밪���� ��ȯ�Ͽ�
	if( fCurrPos > MATH_EPSILON )  //���� ���̰� �������
	{
		//ī�޶� �ٶ󺸴� �������� (ī�޶� look��)���̰���ŭ �̵�
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
			m_bone->SetAniLoop(false);
			m_reserveL = false;

			switch(m_attackCnt)
			{
			case 1:
				
				SetAnimation( "..\\media\\valle\\valle_LLA.ani" );
				m_attackCnt++;
				
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLA.ani");
				return true;
			break;
			case 2:
		//		m_bone->SetAniLoop(false);
				SetAnimation( "..\\media\\valle\\valle_LLLA.ani" );
				m_attackCnt = 10;
		//		m_reserveL = false;
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
					SetAnimation( "..\\media\\valle\\valle_RRA.ani" );
				else if(m_mode == LATTACK)
				{
					SetAnimation( "..\\media\\valle\\valle_LRA.ani" );
					m_attackCnt = 10;  //�� �ִϵ��۱����� ����
				}
				m_attackCnt++;
				
		//		m_weapon->SetAniLoop(false);
		//		m_weapon->SetAnimation("..\\media\\valle\\valle_LLA.ani");
				return true;
			break;
			case 2:
		//		m_bone->SetAniLoop(false);
				if(m_mode == RATTACK)
					SetAnimation( "..\\media\\valle\\valle_RRRA.ani" );
				else if(m_mode == LATTACK)
					SetAnimation( "..\\media\\valle\\valle_LLRA.ani" );
				m_attackCnt = 10;
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
	//	m_weapon->SetAnimation("..\\media\\valle\\valle_normal.ani");
		return true;
	}
	
	return false;
}

void cCharacter::UpdateJump(const bool bAniState)
{
	Vector3 camDir( GetCamera()->GetDirection() );
	Vector3 camRight( GetCamera()->GetRight() );
	Matrix44 mat;

	
	
	if( !bAniState )
	{
		m_bone->SetAniLoop(false);
		/*
		if(m_jumpAttack)
		{
			m_jumpAttack = false;
			int endFrame = m_bone->GetRoot()->GetEndFrame();
			m_prevFrame = endFrame - m_prevFrame;
			
			
			SetAnimation( "..\\media\\valle\\valle_jump2.ani" );
			if( m_prevFrame >= m_bone->GetRoot()->GetEndFrame() )
				SetAnimation( "..\\media\\valle\\valle_jump3.ani" );
			else
				m_bone->SetCurrentAnimationFrame( endFrame );



			return;
		}		
		*/
		switch(m_jumpCnt)
		{
			case 1:
				SetAnimation( "..\\media\\valle\\valle_jump2.ani" );
			break;

			case 3:
				SetAnimation( "..\\media\\valle\\valle_jump3.ani" );
			break;

			case 4:
				mat = GetTM();
				Vector3 pos = mat.GetPosition();
				mat.SetTranslate( Vector3(pos.x, 0.f, pos.z) );
				SetTM(mat);
				m_jumpCnt = 0;
				m_bone->SetAniLoop(true);
				SetAnimation( "..\\media\\valle\\valle_normal.ani" );
				m_mode = NORMAL;
			return;
		}

		m_jumpCnt++;
	}
	else if( bAniState )
	{
		switch(m_jumpCnt)
		{
			case 1:
				mat.SetTranslate( Vector3( 0.f, 1.f, 0.f ) * 3 );
				MultiplyTM( mat );
				GetCamera()->SetPosition( GetTM() );
			break;

			case 2:
				mat.SetTranslate( Vector3( 0.f, 1.f, 0.f ) * 3 );
				MultiplyTM( mat );
				GetCamera()->SetPosition( GetTM() );
				if( m_bone->GetRoot()->GetCurrentFrame() > 
					m_bone->GetRoot()->GetEndFrame() / 2.f )
					m_jumpCnt++;
			break;

			case 3:
				mat.SetTranslate( Vector3( 0.f, 1.f, 0.f ) * -3 );
				MultiplyTM( mat );
				GetCamera()->SetPosition( GetTM() );
			break;

			case 4:
				mat.SetTranslate( Vector3( 0.f, 1.f, 0.f ) * -3 );
				MultiplyTM( mat );
				GetCamera()->SetPosition( GetTM() );
			break;
		}
	}
}

void cCharacter::FindWeapon()
{
	//���� bone ���� ��������
	vector<cBoneNode*>& weaponNode = m_weapon->GetBoneMgr()->GetAllBoneNode();

	for(auto it = weaponNode.begin(); it != weaponNode.end(); ++it)
	{
		//���� bone �̸����� ĳ������ ������ bone ã�Ƽ� �ּ� ��������
		cBoneNode* pNode = m_bone->FindBone( (*it)->GetName() );
		
		if( !pNode )
			continue;
		
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
		weaponNode[i]->SetAccTM( m_characterWeapon[i]->GetAccTM() );
	}
	
	for( int i = 0; i < (int)weaponPalette.size(); ++i )  //���� bone palette ����
	{
		//���� bone�� id������ ã�Ƽ� ���� ��������
		weaponPalette[i] = ( m_bone->GetPalette() )[ m_characterWeapon[i]->GetId() ];
	}
	
	m_weapon->SetTM( GetTM() );  //���� ĳ���� ��ġ�� ���� ��ġ ����
}