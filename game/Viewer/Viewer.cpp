
#include "stdafx.h"
#include "../wxMemMonitorLib/wxMemMonitor.h"
#include "TestScene.h"


DECLARE_TYPE_NAME(cViewer)
class cViewer : public framework::cGameMain
						, public memmonitor::Monitor<cViewer, TYPE_NAME(cViewer)>
{
public:
	cViewer();
	virtual ~cViewer();

	virtual bool OnInit() override;
	virtual void OnUpdate(const float elapseT) override;
	virtual void OnRender(const float elapseT) override;
	virtual void OnShutdown() override;
	virtual void MessageProc( UINT message, WPARAM wParam, LPARAM lParam) override;


private:
	LPD3DXSPRITE m_sprite;

	graphic::cLight m_light;
	graphic::cMaterial m_mtrl;
	graphic::cTexture m_texture;
	graphic::cModel m_model;
	graphic::cModel m_model2;
	graphic::cCharacter m_character;

	graphic::cSprite *m_image;
	graphic::cShader m_shader;
	graphic::cShader m_shaderSkin;
	graphic::cTerrain m_terrain;
	graphic::cCube m_cube;

	LPDIRECT3DTEXTURE9 m_pShadowTex;
	LPDIRECT3DSURFACE9 m_pShadowSurf;
	LPDIRECT3DSURFACE9 m_pShadowTexZ;
	Vector3 m_light2;
	Vector3 m_pos;

	cTestScene *m_scene;
	//graphic::cCollisionManager collisionMgr;


	string m_filePath;

	POINT m_curPos;
	bool m_LButtonDown;
	bool m_RButtonDown;
	bool m_MButtonDown;
	Matrix44 m_rotateTm;

	graphic::cCamera m_camera;

	Vector3 m_boxPos;

};

INIT_FRAMEWORK(cViewer);


static const UINT MAP_SIZE = 256;
// �����ũ��
#define RS   graphic::GetDevice()->SetRenderState
#define TSS  graphic::GetDevice()->SetTextureStageState
#define SAMP graphic::GetDevice()->SetSamplerState



cViewer::cViewer() :
	m_sprite(NULL)
,	m_image(NULL)
,	m_scene(NULL)
,	m_model(100)
,	m_model2(1)
,	m_character(1000)
{
	m_windowName = L"Viewer";
	const RECT r = {0, 0, 1024, 768};
	m_windowRect = r;
	m_LButtonDown = false;
	m_RButtonDown = false;
	m_MButtonDown = false;
}

cViewer::~cViewer()
{
	SAFE_DELETE(m_image);
	SAFE_DELETE(m_scene);
	SAFE_RELEASE(m_sprite);
	graphic::ReleaseRenderer();
}


bool cViewer::OnInit()
{
	DragAcceptFiles(m_hWnd, TRUE);

	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);

	//m_scene = new cTestScene(m_sprite);
	//m_scene->SetPos(Vector3(100,100,0));

	//m_model->Create( "../media/weapon.dat" );
	m_model.Create( "../media/max script/valle1.dat" );
	m_model.SetAnimation( "../media/max script/valle_forward.ani" );
	m_model.SetRenderBoundingBox(true);

	m_character.Create( "../media/max script/tiac.dat" );
	//m_character.SetAnimation( "../media/max script/tiac_normal.ani" );
	m_character.SetAnimation( "../media/max script/tiac_la.ani" );
	m_character.LoadWeapon( "../media/max script/tiac_weapon.dat" );
	m_character.SetRenderBoundingBox(true);
	m_character.SetRenderWeaponBoundingBox(true);


	m_shader.Create( "../media/shader/hlsl_rigid_phong.fx", "TShader" );
	//m_shader.Create( "../media/shader/hlsl_rigid.fx", "TShader" );
	m_shaderSkin.Create( "../media/shader/hlsl_skinning_using_texcoord.fx", "TShader" );
	m_terrain.CreateFromHeightMap( "../media/terrain/flat_terrain2.jpg", "../media/terrain/grass_spring1.bmp", 7.f);

	m_cube.SetCube(Vector3(-50,-50,-50), Vector3(50,50,50));



	// �׸��� �ؽ�ó ����
	if (FAILED(graphic::GetDevice()->CreateTexture(MAP_SIZE, MAP_SIZE, 1, 
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &m_pShadowTex, NULL)))
		return false;
	if (FAILED(m_pShadowTex->GetSurfaceLevel(0, &m_pShadowSurf)))
		return false;
	if (FAILED(graphic::GetDevice()->CreateDepthStencilSurface(
		MAP_SIZE, MAP_SIZE, D3DFMT_D24S8, 
		D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pShadowTexZ, NULL)))
		return false;


	m_mtrl.InitWhite();

	Vector4 color(1,1,1,1);
	m_light.Init( graphic::cLight::LIGHT_DIRECTIONAL, 
		color * 0.3f, 
		color * 0.7f, 
		color, 
		Vector3(0,-1,0));
	m_light.Bind(0);

	
	const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
	const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
	m_camera.SetCamera(Vector3(100,500,-500), Vector3(0,0,0), Vector3(0,1,0));
	m_camera.SetProjection(D3DX_PI / 4.f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 10000.0f);

	graphic::GetDevice()->LightEnable (
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ

	return true;
}


void cViewer::OnUpdate(const float elapseT)
{
	m_model.Move(elapseT);
	m_character.Move(elapseT);

	//collisionMgr.UpdateCollisionBox();
	//collisionMgr.CollisionTest(1);
}


void cViewer::OnRender(const float elapseT)
{
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
		graphic::GetRenderer()->RenderAxis();

		Matrix44 matIdentity;
		if (m_scene)
			m_scene->Render(matIdentity);


		//---------------------------------------------------------------
		// �� ��� + �׸���.
		LPDIRECT3DSURFACE9 pOldBackBuffer, pOldZBuffer;
		D3DVIEWPORT9 oldViewport;

		graphic::GetDevice()->GetRenderTarget(0, &pOldBackBuffer);
		graphic::GetDevice()->GetDepthStencilSurface(&pOldZBuffer);
		graphic::GetDevice()->GetViewport(&oldViewport);

		graphic::GetDevice()->SetRenderTarget(0, m_pShadowSurf);
		graphic::GetDevice()->SetDepthStencilSurface(m_pShadowTexZ);
		// ����Ʈ����  x y  width    height   minz maxz
		D3DVIEWPORT9 viewport = {0,0, MAP_SIZE,MAP_SIZE,0.0f,1.0f};
		graphic::GetDevice()->SetViewport(&viewport);

		// �׸��ڸ� Ŭ����
		graphic::GetDevice()->Clear(0L, NULL
			, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
			, 0x00000000, 1.0f, 0L);

		const Matrix44 cubeTm = m_cube.GetTransform();
		m_pos = Vector3(cubeTm._41, cubeTm._42, cubeTm._43);
		m_light2 = Vector3(0,1000,0);

		Matrix44 matView;// �� ���
		matView.SetView2( m_light2, m_pos, Vector3(0,1,0));

		Matrix44 matProj;// ���� ���
		matProj.SetProjection( D3DX_PI/2.5f, 1, 0.1f, 10000);

		m_shaderSkin.SetMatrix( "mVP", matView * matProj);
		m_shaderSkin.SetVector( "vLightDir", Vector3(0,-1,0) );
		m_shaderSkin.SetVector( "vEyePos", m_camera.GetEyePos());
		m_shaderSkin.SetMatrix( "mWIT", matIdentity);
		Matrix44 matPos;
		matPos.SetTranslate(m_pos);
		m_shaderSkin.SetMatrix( "mWorld", cubeTm);

		m_shaderSkin.SetRenderPass(1);
		m_model.RenderShadow(m_shaderSkin);


		//-----------------------------------------------------
		// ������Ÿ�� ����
		//-----------------------------------------------------
		graphic::GetDevice()->SetRenderTarget(0, pOldBackBuffer);
		graphic::GetDevice()->SetDepthStencilSurface(pOldZBuffer);
		graphic::GetDevice()->SetViewport(&oldViewport);
		pOldBackBuffer->Release();
		pOldZBuffer->Release();
		graphic::GetDevice()->SetTransform( D3DTS_VIEW, (D3DXMATRIX*)&m_camera.GetViewMatrix() );
		graphic::GetDevice()->SetTransform( D3DTS_PROJECTION, (D3DXMATRIX*)&m_camera.GetProjectionMatrix() );

		m_shaderSkin.SetMatrix( "mVP", m_camera.GetViewProjectionMatrix());
		m_shaderSkin.SetRenderPass(0);
		m_model.SetTM(m_cube.GetTransform());
		m_model.RenderShader(m_shaderSkin);
		m_character.RenderShader(m_shaderSkin);


		//------------------------------------------------------------------------
		// ���� ���.
		//------------------------------------------------------------------------
		D3DXMATRIX mTT;
		mTT = D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f
			, 0.0f,-0.5f, 0.0f, 0.0f
			, 0.0f, 0.0f, 1.0f, 0.0f
			, 0.5f, 0.5f, 0.0f, 1.0f);
		Matrix44 mT = *(Matrix44*)&mTT;

		m_shader.SetMatrix( "mVP", m_camera.GetViewProjectionMatrix());
		m_shader.SetVector( "vLightDir", Vector3(0,-1,0) );
		m_shader.SetVector( "vEyePos", m_camera.GetEyePos());
		m_shader.SetMatrix( "mWIT", matIdentity);
		m_shader.SetMatrix( "mWorld", matIdentity);
		m_shader.SetTexture("ShadowMap", m_pShadowTex);

		Matrix44 m = matView * matProj * mT;
		m_shader.SetMatrix( "mWVPT", m );

		m_shader.SetRenderPass(2);
		m_terrain.RenderShader(m_shader);

		m_cube.Render(matIdentity);



#if 1 // ����׿� �ؽ�ó ���
		{
			graphic::GetDevice()->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
			graphic::GetDevice()->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
			graphic::GetDevice()->SetTextureStageState(1,D3DTSS_COLOROP,    D3DTOP_DISABLE);
			float scale = 128.0f;
			typedef struct {FLOAT p[4]; FLOAT tu, tv;} TVERTEX;

			TVERTEX Vertex[4] = {
				// x  y  z rhw tu tv
				{0, scale, 0, 1, 0, 0,},
				{scale, scale,0, 1, 1, 0,},
				{scale, scale+scale,0, 1, 1, 1,},
				{0, scale+scale,0, 1, 0, 1,},
			};
			graphic::GetDevice()->SetTexture( 0, m_pShadowTex );
			graphic::GetDevice()->SetVertexShader(NULL);
			graphic::GetDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
			graphic::GetDevice()->SetPixelShader(0);
			graphic::GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );
		}
#endif


		//������ ��
		graphic::GetDevice()->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}
}


void cViewer::OnShutdown()
{

}


void cViewer::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_scene)
		m_scene->MessageProc(message, wParam, lParam);

	switch (message)
	{
	case WM_DROPFILES:
		{
			HDROP hdrop = (HDROP)wParam;
			char filePath[ MAX_PATH];
			const UINT size = DragQueryFileA(hdrop, 0, filePath, MAX_PATH);
			if (size == 0) 
				return;// handle error...

			m_filePath = filePath;

			const graphic::RESOURCE_TYPE::TYPE type = graphic::cResourceManager::Get()->GetFileKind(filePath);
			switch (type)
			{
			case graphic::RESOURCE_TYPE::MESH:
				m_model.Create(filePath);
				break;

			case graphic::RESOURCE_TYPE::ANIMATION:
				m_model.SetAnimation(filePath);
				break;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			int fwKeys = GET_KEYSTATE_WPARAM(wParam);
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			dbg::Print( "%d %d", fwKeys, zDelta);

			const float len = m_camera.GetDistance();
			float zoomLen = (len > 100)? 50 : (len/4.f);
			if (fwKeys & 0x4)
				zoomLen = zoomLen/10.f;

			m_camera.Zoom( (zDelta<0)? -zoomLen : zoomLen );
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_F5: // Refresh
			{
				if (m_filePath.empty())
					return;
				m_model.Create(m_filePath);
			}
			break;
		case VK_BACK:
			// ȸ�� ��� �ʱ�ȭ.
			m_rotateTm.SetIdentity();
			m_model.SetTM(m_rotateTm);
			break;
		case VK_TAB:
			{
				static bool flag = false;
				graphic::GetDevice()->SetRenderState(D3DRS_CULLMODE, flag);
				graphic::GetDevice()->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
				flag = !flag;
			}
			break;

		case VK_LEFT: m_boxPos.x -= 10.f; break;
		case VK_RIGHT: m_boxPos.x += 10.f; break;
		case VK_UP: m_boxPos.z += 10.f; break;
		case VK_DOWN: m_boxPos.z -= 10.f; break;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			m_LButtonDown = true;
			m_curPos.x = LOWORD(lParam);
			m_curPos.y = HIWORD(lParam);
		}
		break;

	case WM_LBUTTONUP:
		m_LButtonDown = false;
		break;

	case WM_RBUTTONDOWN:
		{
			m_RButtonDown = true;
			m_curPos.x = LOWORD(lParam);
			m_curPos.y = HIWORD(lParam);
		}
		break;

	case WM_RBUTTONUP:
		m_RButtonDown = false;
		break;

	case WM_MBUTTONDOWN:
		m_MButtonDown = true;
		m_curPos.x = LOWORD(lParam);
		m_curPos.y = HIWORD(lParam);
		break;

	case WM_MBUTTONUP:
		m_MButtonDown = false;
		break;

	case WM_MOUSEMOVE:
		{
			if (wParam & 0x10) // middle button down
			{
				POINT pos = {LOWORD(lParam), HIWORD(lParam)};
				if (m_scene)
					m_scene->SetPos(Vector3(pos.x, pos.y,0));
			}

			if (m_LButtonDown)
			{
				POINT pos = {LOWORD(lParam), HIWORD(lParam)};
				const int x = pos.x - m_curPos.x;
				const int y = pos.y - m_curPos.y;
				m_curPos = pos;

				Quaternion q1(m_camera.GetRight(), -y * 0.01f);
				Quaternion q2(m_camera.GetUpVector(), -x * 0.01f);

				m_rotateTm *= (q2.GetMatrix() * q1.GetMatrix());
			}
			else if (m_RButtonDown)
			{
				POINT pos = {LOWORD(lParam), HIWORD(lParam)};
				const int x = pos.x - m_curPos.x;
				const int y = pos.y - m_curPos.y;
				m_curPos = pos;

				m_camera.Yaw2( x * 0.005f );
				m_camera.Pitch2( y * 0.005f );
			}
			else if (m_MButtonDown)
			{
				const POINT point = {LOWORD(lParam), HIWORD(lParam)};
				const POINT pos = {point.x - m_curPos.x, point.y - m_curPos.y};
				m_curPos = point;

				const float len = m_camera.GetDistance();
				m_camera.MoveRight( -pos.x * len * 0.001f );
				m_camera.MoveUp( pos.y * len * 0.001f );
			}
			else
			{
				POINT pos = {LOWORD(lParam), HIWORD(lParam)};

				Vector3 pickPos;
				Ray ray(pos.x, pos.y, 1024, 768, m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix());
				const float y = m_terrain.GetHeightFromRay(ray.orig, ray.dir, pickPos);

				pickPos.y = y;

				Matrix44 matT;
				matT.SetTranslate(pickPos);
				m_cube.SetTransform(matT);
				
				//m_scene->SetPos(Vector3(pos.x, pos.y,0));
				//if (m_image->IsContain(Vector2(pos.x, pos.y)))
				//{
				//	static int count = 0;
				//	++count;
				//	dbg::Print( "IsContain %d", count);
				//}
			}

		}
		break;
	}
}

