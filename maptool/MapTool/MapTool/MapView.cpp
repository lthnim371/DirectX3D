// MapView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MapView.h"


using namespace graphic;


// CMapView
CMapView::CMapView() :
	m_dxInit(false)
,	m_RButtonDown(false)
,	m_LButtonDown(false)
,	m_MButtonDown(false)
,	m_modelShader(NULL)
//�߰�
, m_objectCount(0)
, m_currSelectObj(NULL)
{

}

CMapView::~CMapView()
{
}

BEGIN_MESSAGE_MAP(CMapView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CMapView �׸����Դϴ�.

void CMapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CMapView �����Դϴ�.

#ifdef _DEBUG
void CMapView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMapView �޽��� ó�����Դϴ�.

bool CMapView::Init()
{
	graphic::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	graphic::GetDevice()->LightEnable (
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ


	m_grid.Create(64,64,50.f);
	m_cube.SetCube(Vector3(-10,-10,-10), Vector3(10,10,10));
	m_cube.SetColor( 0xFF0000FF );
	m_dxInit = true;

	m_terrainShader.Create( "../../media/shader/hlsl_terrain.fx", "TShader" );
	m_terrainShader2.Create( "../../media/shader/hlsl_terrain_splatting.fx", "TShader" );
	m_modelShader = graphic::cResourceManager::Get()->LoadShader(  "hlsl_skinning_no_light.fx" );

//�߰�
	m_objectShader.Create( "../../media/shader/hlsl_rigid.fx", "TShader" );

	return true;
}


void CMapView::Render()
{
	if (!m_dxInit)
		return;

	//ȭ�� û��
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(150, 150, 150),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
		graphic::GetDevice()->BeginScene();
		graphic::GetRenderer()->RenderFPS();
		//graphic::GetRenderer()->RenderGrid();

		const Matrix44 matIdentity;

		cCamera &camera = cMapController::Get()->GetCamera();
		m_terrainShader.SetMatrix( "mVP", camera.GetViewProjectionMatrix());
		m_terrainShader.SetVector( "vLightDir", Vector3(0,-1,0) );
		m_terrainShader.SetVector( "vEyePos", camera.GetEyePos());
		m_terrainShader.SetMatrix( "mWIT", matIdentity);
		m_terrainShader.SetMatrix( "mWorld", matIdentity);
		//m_terrainShader.SetTexture("ShadowMap", m_pShadowTex);

		m_modelShader->SetMatrix( "mVP", camera.GetViewProjectionMatrix());
		m_modelShader->SetVector( "vLightDir", Vector3(0,-1,0) );
		m_modelShader->SetVector( "vEyePos", camera.GetEyePos());

		m_terrainShader2.SetMatrix( "mVP", camera.GetViewProjectionMatrix());
		m_terrainShader2.SetVector( "vLightDir", Vector3(0,-1,0) );
		m_terrainShader2.SetVector( "vEyePos", camera.GetEyePos());
		m_terrainShader2.SetMatrix( "mWIT", matIdentity);
		m_terrainShader2.SetMatrix( "mWorld", matIdentity);
		//m_terrainShader.2SetTexture("ShadowMap", m_pShadowTex);

	//�߰�
		m_objectShader.SetMatrix( "mVP", camera.GetViewProjectionMatrix());
		m_objectShader.SetVector( "vLightDir", Vector3(0,-1,0) );
		m_objectShader.SetVector( "vEyePos", camera.GetEyePos());
		m_objectShader.SetMatrix( "mWIT", matIdentity);
		m_objectShader.SetMatrix( "mWorld", matIdentity);

		//m_terrainShader.SetRenderPass(1);
		//cMapController::Get()->GetTerrain().Render();
		//cMapController::Get()->GetTerrain().RenderShader(m_terrainShader);

		m_terrainShader2.SetRenderPass(1);
		cMapController::Get()->GetTerrain().RenderShader(m_terrainShader2);


		switch (cMapController::Get()->GetEditMode())
		{
		case EDIT_MODE::MODE_BRUSH:
			cMapController::Get()->GetTerrainCursor().RenderBrush();
			break;
		case EDIT_MODE::MODE_MODEL:
			cMapController::Get()->GetTerrainCursor().RenderModel();
			break;
	//�߰�
		case EDIT_MODE::MODE_OBJECT:
			{
				if( cMapController::Get()->GetObject().empty() == false )
				{
					for( auto it = cMapController::Get()->GetObject().begin();
						it != cMapController::Get()->GetObject().end();		++it )
					{
						(*it)->RenderShader( m_objectShader );
					}
				}

				if( graphic::cModel* pCurrObject = cMapController::Get()->GetCurrObject() )
					pCurrObject->RenderShader( m_objectShader );
			}
			break;
		}


		graphic::GetRenderer()->RenderAxis();

		//������ ��
		graphic::GetDevice()->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}

}


void CMapView::Update(float elapseT)
{
	graphic::GetRenderer()->Update(elapseT);
}


void CMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_LButtonDown = true;
	m_curPos = point;
	CView::OnLButtonDown(nFlags, point);
}


void CMapView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	// �������� ���� ��ġ ��Ų��.
	if (m_LButtonDown && 
		(cMapController::Get()->GetEditMode() == EDIT_MODE::MODE_MODEL))
	{
		// ���� ���õǾ� �ִ� ���¶��, ���� �������� ��ġ ��Ų��.
		if (cMapController::Get()->GetTerrainCursor().IsSelectModel())
		{
			if (const graphic::cModel *model = cMapController::Get()->GetTerrainCursor().GetSelectModel())
			{
				cMapController::Get()->GetTerrain().AddRigidModel(*model);
				cMapController::Get()->UpdatePlaceModel();
			}
		}
		else
		{
			// ���� ���õǾ� ���� �ʴٸ�, �������� ���� ��ŷ�ؼ� �����Ѵ�.
			cCamera &camera = cMapController::Get()->GetCamera();
			m_ray.Create(point.x, point.y, VIEW_WIDTH, VIEW_HEIGHT, 
				camera.GetProjectionMatrix(), camera.GetViewMatrix() );

			// �� ��ŷ.
			if (graphic::cModel *model = 
				cMapController::Get()->GetTerrain().PickModel(m_ray.orig, m_ray.dir))
			{
				cMapController::Get()->GetTerrain().RemoveRigidModel(model, false);
				cMapController::Get()->GetTerrainCursor().SelectModel(model);
				cMapController::Get()->UpdatePlaceModel();
			}
		}
	}
//�߰�
	if( m_LButtonDown && 
		cMapController::Get()->GetEditMode() == EDIT_MODE::MODE_OBJECT )
	{	
		graphic::cModel* pCurrObject = cMapController::Get()->GetCurrObject();
		m_ray.Create(point.x, point.y, VIEW_WIDTH, VIEW_HEIGHT, 
					cMapController::Get()->GetCamera().GetProjectionMatrix(), cMapController::Get()->GetCamera().GetViewMatrix() );
		if( pCurrObject )
		{
			Vector3 pickPos;
			cMapController::Get()->GetTerrain().Pick(m_ray.orig, m_ray.dir, pickPos);
			
			Matrix44 matPos;
			matPos.SetTranslate( pickPos );

			graphic::cModel* pNewObj = new graphic::cModel( m_objectCount++ );
			pNewObj->Create( cMapController::Get()->GetCurrObjFileName(), graphic::MODEL_TYPE::RIGID );
			pNewObj->SetTM( matPos );
			pNewObj->CreateCube();
			pNewObj->SetRenderBoundingBox( true );

			cMapController::Get()->AddObject( pNewObj );
		}
		else if( !pCurrObject )
		{			
			vector<graphic::cModel*>& rObj = cMapController::Get()->GetObject();

			if( rObj.empty() == false )
			{
				if( m_currSelectObj )
					m_currSelectObj = NULL;

				for( auto it = rObj.begin(); it != rObj.end(); ++it )
				{
					if( (*it)->Pick( m_ray.orig, m_ray.dir ) == true )
					{
						m_currSelectObj = (*it);
						break;
					}  //if
				}  //for
			}  //if
		}  //else if
	}  //if

	m_LButtonDown = false;
	CView::OnLButtonUp(nFlags, point);
}


void CMapView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_RButtonDown = true;
	m_curPos = point;
	CView::OnRButtonDown(nFlags, point);
}


void CMapView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_RButtonDown = false;	
	CView::OnRButtonUp(nFlags, point);
}


void CMapView::OnMouseMove(UINT nFlags, CPoint point)
{
	cCamera &camera = cMapController::Get()->GetCamera();

	if (m_LButtonDown)
	{
		CPoint pos = point - m_curPos;
		m_curPos = point;

		if (cMapController::Get()->GetEditMode() == EDIT_MODE::MODE_BRUSH)
		{
			cMapController::Get()->Brush(point);
		}
	}
	else if (m_RButtonDown)
	{
		CPoint pos = point - m_curPos;
		m_curPos = point;
		camera.Pitch2(pos.y * 0.005f); 
		camera.Yaw2(pos.x * 0.005f); 
	}
	else if (m_MButtonDown)
	{
		CPoint pos = point - m_curPos;
		m_curPos = point;

		Vector3 dir = camera.GetDirection();
		dir.y = 0;
		dir.Normalize();

		const float len = camera.GetDistance();
		camera.MoveRight( -pos.x * len * 0.001f );
		camera.MoveAxis( dir, pos.y * len * 0.001f );
	}
//�߰�
	else if( cMapController::Get()->GetEditMode() == EDIT_MODE::MODE_OBJECT )
	{
		CPoint pos = point - m_curPos;
		m_curPos = point;

		if( graphic::cModel* pCurrObject = cMapController::Get()->GetCurrObject() )
		{
			m_ray.Create(m_curPos.x, m_curPos.y, VIEW_WIDTH, VIEW_HEIGHT, 
				camera.GetProjectionMatrix(), camera.GetViewMatrix() );

			Vector3 pickPos;
			cMapController::Get()->GetTerrain().Pick( m_ray.orig, m_ray.dir, pickPos);

			Matrix44 matT;
			matT.SetTranslate( pickPos );
			pCurrObject->SetTM( matT );
		}
		//�߰�
		else if( m_currSelectObj )
		{
			Quaternion q( Vector3(0,1,0), pos.x * -0.005f );
			Matrix44 matR = q.GetMatrix();
			m_currSelectObj->SetTM( matR * m_currSelectObj->GetTM() );
			m_currSelectObj->GetCube().SetTransform( m_currSelectObj->GetTM() );
		}
	}
	else
	{
		m_curPos = point;

		if ((cMapController::Get()->GetEditMode() == EDIT_MODE::MODE_BRUSH) ||
			(cMapController::Get()->GetEditMode() == EDIT_MODE::MODE_MODEL))
		{
			m_ray.Create(m_curPos.x, m_curPos.y, VIEW_WIDTH, VIEW_HEIGHT, 
				camera.GetProjectionMatrix(), camera.GetViewMatrix() );

			Vector3 pickPos;
			cMapController::Get()->GetTerrain().Pick(m_ray.orig, m_ray.dir, pickPos);
			cMapController::Get()->GetTerrainCursor().UpdateCursor(cMapController::Get()->GetTerrain(), pickPos );
		}

	}

	CView::OnMouseMove(nFlags, point);
}


void CMapView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_MButtonDown = true;
	CView::OnMButtonDown(nFlags, point);
}


void CMapView::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_MButtonDown = false;
	ReleaseCapture();
	CView::OnMButtonUp(nFlags, point);
}


BOOL CMapView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	cCamera &camera = cMapController::Get()->GetCamera();
	
	const float len = camera.GetDistance();
	float zoomLen = (len > 100)? 50 : (len/4.f);
	if (nFlags & 0x4)
		zoomLen = zoomLen/10.f;

	camera.Zoom( (zDelta<0)? -zoomLen : zoomLen );	

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_TAB:
		{
			static bool flag = false;
			graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, flag? D3DCULL_CCW : D3DCULL_NONE);
			graphic::GetDevice()->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
			flag = !flag;
		}
		break;

	case VK_ESCAPE:
		{
			cMapController::Get()->GetTerrainCursor().CancelSelectModel();
		//�߰�
			cMapController::Get()->DeleteCurrObject();

			if(m_currSelectObj)
				m_currSelectObj = NULL;
		}
		break;

	case VK_DELETE:
		{
			if( m_currSelectObj )
			{			
				vector<graphic::cModel*>& rObj = cMapController::Get()->GetObject();

				if( rObj.empty() == false )
				{
					for( auto it = rObj.begin(); it != rObj.end(); ++it )
					{
						if( (*it) == m_currSelectObj )
						{
							rObj.erase(it);
							m_currSelectObj = NULL;
							break;
						}  //if
					}  //for
				}  //if
			}  //if
		}
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
