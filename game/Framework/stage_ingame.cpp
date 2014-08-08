#include "stdafx.h"
#include "stage_ingame.h"

using namespace framework;

cStage_Ingame::cStage_Ingame()
{
	m_id = 0;
}
cStage_Ingame::~cStage_Ingame()
{
	SAFE_DELETE(character1);
	SAFE_DELETE(character2);
}

//void cStage_Ingame::Init()
void cStage_Ingame::Init(const int nId)
{
	character1 = new graphic::cCharacter(0);
	character2 = new graphic::cCharacter(1);
	m_id = nId;

	switch(nId)
	{
		case 0:
			{
				character1->Create( "..\\media\\mesh\\valle\\valle_character1.dat" );
				character1->LoadWeapon( "..\\media\\mesh\\valle\\valle_weapon1.dat" );
			//test
			//	character1->SetRenderBoundingBox(true);

				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_backward.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_dash.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LA.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LLA.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LLLA.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LRA.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LLRA.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_RA.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_RRA.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_RRRA.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_jump1.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_jump2.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_jump3.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_JLA.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back1.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back2.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front1.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front2.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );

				//Matrix44 rot;
				//rot.SetRotationY( -1.f );
				Matrix44 pos;
				pos.SetTranslate( Vector3( 0, 0, -5000.f) );
				character1->MultiplyTM( pos);
				graphic::GetCamera()->SetPosition( character1->GetTM() );
			}
		break;

		case 1:
			{
				character2->Create( "..\\media\\mesh\\valle\\valle_character1.dat" );
				character2->LoadWeapon( "..\\media\\mesh\\valle\\valle_weapon1.dat" );
			//test
			//	character2->SetRenderBoundingBox(true);

				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_backward.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_dash.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_LA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_LLA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_LLLA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_LRA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_LLRA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_RA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_RRA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_RRRA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_jump1.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_jump2.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_jump3.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_JLA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back1.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back2.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front1.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front2.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );

				//Matrix44 rot;
				//rot.SetRotationY( 180.f );
				Matrix44 pos;
				pos.SetTranslate( Vector3( 0, 0, 5000.f) );
				character2->MultiplyTM( pos);
				graphic::GetCamera()->SetPosition( character2->GetTM() );
			}
		break;
	}
	
	::GetCursorPos( &m_currMouse );
	::ScreenToClient( GetStageMgr()->GetWindowHandle(), &m_currMouse );
}

//void cStage_Ingame::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ingame::Input(const float elapseTime)
{
	graphic::cCharacter* pMe = ( m_id == 0 ? character1 : character2 );

	m_prevMouse = m_currMouse;
	::GetCursorPos( &m_currMouse );
	::ScreenToClient( GetStageMgr()->GetWindowHandle(), &m_currMouse );
	if( m_currMouse.x != m_prevMouse.x || m_currMouse.y != m_prevMouse.y )
	{
		//character1->Action( character1->ROTATION, m_currMouse.x );
		POINT ptMouse;
		ptMouse.x = m_currMouse.x - m_prevMouse.x;
		ptMouse.y = m_currMouse.y - m_prevMouse.y;
	//	character1->Update( character1->ROTATION, (float)ptMouse.x, (float)ptMouse.y );
		pMe->Update( pMe->ROTATION, (float)ptMouse.x, (float)ptMouse.y );
	}	
	
	//프로그램 테스트용
//	else if( InputMgr->isOnceKeyDown('1') )
	if( InputMgr->isOnceKeyDown('1') )
	{
		graphic::GetCamera()->SetHeight(-10.f);
	}
	else if( InputMgr->isOnceKeyDown('2') )
	{
		graphic::GetCamera()->SetHeight(10.f);
	}

	if( InputMgr->isOnceKeyDown( VK_LBUTTON ) )
	{
		pMe->Update( pMe->LATTACK );
	//	character1->Update( character1->LATTACK );
	}
	else if( InputMgr->isOnceKeyDown( VK_RBUTTON ) )
	{
		pMe->Update( pMe->RATTACK );
	//	character1->Update( character1->RATTACK );
	}
	else if( InputMgr->isStayKey('W') )
	{	
		if( InputMgr->isStayKey(VK_SHIFT) )
			pMe->Update( pMe->DASH );
		//	character1->Update( character1->DASH );
		else if( InputMgr->isOnceKeyDown(VK_SPACE) )
			pMe->Update( pMe->FRONTJUMP );
		//	character1->Update( character1->FRONTJUMP );
		else
			pMe->Update( pMe->FORWARD );
		//	character1->Update( character1->FORWARD );
	}
	else if( InputMgr->isStayKey('S') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			pMe->Update( pMe->BACKJUMP );
		//	character1->Update( character1->BACKJUMP );
		else
			pMe->Update( pMe->BACKWARD );
		//	character1->Update( character1->BACKWARD );
	}
	else if( InputMgr->isStayKey('A') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			pMe->Update( pMe->LEFTJUMP );
		//	character1->Update( character1->LEFTJUMP );
		else
			pMe->Update( pMe->LEFTWARD );
		//	character1->Update( character1->LEFTWARD );
	}
	else if( InputMgr->isStayKey('D') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			pMe->Update( pMe->RIGHTJUMP );
		//	character1->Update( character1->RIGHTJUMP );
		else
			pMe->Update( pMe->RIGHTWARD );
		//	character1->Update( character1->RIGHTWARD );
	}
	else if( InputMgr->isOnceKeyDown(VK_SPACE) )
	{	
		pMe->Update( pMe->JUMP );
	//	character1->Update( character1->JUMP );
	}
	else
	{
		pMe->Update( pMe->NORMAL );
	//	character1->Update( character1->NORMAL );
	}
}

//void cStage_Ingame::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ingame::Update(const float elapseTime)
{
	//	graphic::GetRenderer()->Update(elapseT);  //fps 업데이트
	
	bool bAniState = character1->Move(elapseTime);
	bool bAniState2 = character2->Move(elapseTime);
//	graphic::GetCamera()->SetPosition( character1->GetTM() );

	if( character1->GetCharacterCube() != NULL && character2->GetCharacterCube() != NULL )
	{
		if( character1->GetCubeCheck() == true )
		{
			if( true == character2->CollisionCheck( *(character1->GetWeaponCube()), graphic::GetCamera()->GetLook() ) )
			{
				character1->SetAttackSuccess();
			}
		}
		else if( character2->GetCubeCheck() == true )
		{
			if ( true == character1->CollisionCheck( *(character2->GetWeaponCube()), graphic::GetCamera()->GetLook() ) )
			{
				character2->SetAttackSuccess();
			}
		}

		if( character1->GetMode() == character1->BEHIT )
		{
			if( true == character1->CollisionCheck( *(character2->GetCharacterCube()), graphic::GetCamera()->GetLook(), graphic::GetCamera()->GetDirection() ) )
				character1->UpdateBeHit( bAniState, graphic::GetCamera()->GetDirection(), character2->GetAniPosGap()  );
		}
		else if( character2->GetMode() == character2->BEHIT )
		{
			if( true == character2->CollisionCheck( *(character1->GetCharacterCube()), graphic::GetCamera()->GetLook(), graphic::GetCamera()->GetDirection() ) )
				character2->UpdateBeHit( bAniState2, graphic::GetCamera()->GetDirection(), character1->GetAniPosGap() );
		}
	}
}

//void cStage_Ingame::Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ingame::Render(const float elapseTime)
{
		//fps 및 그리드 출력
		graphic::GetRenderer()->RenderFPS();
		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();


//		character1->Render();
//		character2->Render();

	//test
		character1->RenderShader( graphic::GetCamera()->GetShader() );
		character2->RenderShader( graphic::GetCamera()->GetShader() );				

	//test
		graphic::GetCamera()->Render( character1->GetHP(), character2->GetHP() );
}