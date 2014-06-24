﻿//140623(3-24일차)과제 : fps 출력 및 쿼터니언 응용

#include <windows.h>
#include <string>
#include <fstream>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../math/Math.h"
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#include <vector>
#include <map>

#include "../../wxMemMonitorLib/wxMemMonitor.h"
MEMORYMONITOR_INNER_PROCESS();

using namespace std;

LPDIRECT3DDEVICE9	g_pDevice = NULL;
const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
const int WINPOS_X = 0; //초기 윈도우 시작 위치 X
const int WINPOS_Y = 0; //초기 윈도우 시작 위치 Y


DECLARE_TYPE_NAME(sGlobal)
struct sGlobal : public memmonitor::Monitor<sGlobal, TYPE_NAME(sGlobal)>
{
	ID3DXFont *font;  //화면에 출력하는 용도
	ID3DXMesh*mesh3DText;  //내부적으로 버텍스, 인덱스 이용함
	ID3DXSprite *textSprite;   // Sprite for batching draw text calls

	// 회전
	POINT curPos;
	bool lButtonDown;
	Matrix44 localTm;

	enum RENDER_MODE 
	{ 
		RENDER_MESH, 
		RENDER_FACE_NORMAL, 
		RENDER_VERTEX_NORMAL 
	};
	RENDER_MODE mode;
};
sGlobal *global;


// 버텍스 구조체
struct Vertex
{
	Vertex() {}
	Vertex(float x0, float y0, float z0) : p(Vector3(x0, y0, z0)), n(Vector3(0,0,0)) {}
	Vector3 p;
	Vector3 n;
	float u, v;
	static const DWORD FVF;
};
//버텍스 구조체 포맷.
const DWORD Vertex::FVF  = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;



// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
bool InitDirectX(HWND hWnd);
bool InitVertexBuffer();
void Render(int timeDelta);
HRESULT CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice,
	LPD3DXMESH* ppMesh, char* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic );



int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Sample";
	wchar_t windowName[32] = L"Sample";

	//윈도우 클레스 정보 생성
	//내가 이러한 윈도를 만들겠다 라는 정보
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	WndClass.cbWndExtra = 0;			//윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);		//윈도우 배경색상
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//윈도우의 커서모양 결정
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );		//윈도우아이콘모양 결정
	WndClass.hInstance = hInstance;				//프로그램인스턴스핸들 
	WndClass.lpfnWndProc = (WNDPROC)WndProc;			//윈도우 프로시져 함수 포인터
	WndClass.lpszMenuName = NULL;						//메뉴이름 없으면 NULL
	WndClass.lpszClassName = className;				//지금 작성하고 있는 윈도우 클레스의 이름
	WndClass.style	 = CS_HREDRAW | CS_VREDRAW;	//윈도우 그리기 방식 설정 ( 사이즈가 변경될때 화면갱신 CS_HREDRAW | CS_VREDRAW )

	//위에서 작성한 윈도우 클레스정보 등록
	RegisterClass( &WndClass );

	//윈도우 생성
	//생성된 윈도우 핸들을 전역변수 g_hWnd 가 받는다.
	HWND hWnd = CreateWindow(
		className,				//생성되는 윈도우의 클래스이름
		windowName,				//윈도우 타이틀바에 출력되는 이름
		WS_OVERLAPPEDWINDOW,	//윈도우 스타일 WS_OVERLAPPEDWINDOW
		WINPOS_X,				//윈도우 시작 위치 X 
		WINPOS_Y,				//윈도우 시작 위치 Y
		WINSIZE_X,				//윈도우 가로 크기 ( 작업영역의 크기가 아님 )
		WINSIZE_Y,				//윈도우 세로 크기 ( 작업영역의 크기가 아님 )
		GetDesktopWindow(),		//부모 윈도우 핸들 ( 프로그램에서 최상위 윈도우면 NULL 또는 GetDesktopWindow() )
		NULL,					//메뉴 ID ( 자신의 컨트롤 객체의 윈도우인경우 컨트롤 ID 가 된	
		hInstance,				//이 윈도우가 물릴 프로그램 인스턴스 핸들
		NULL					//추가 정보 NULL ( 신경끄자 )
		);

	//윈도우를 정확한 작업영역 크기로 맞춘다
	RECT rcClient = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient 크기를 작업 영영으로 할 윈도우 크기를 rcClient 에 대입되어 나온다.

	//윈도우 크기와 윈도우 위치를 바꾸어준다.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	if (!InitDirectX(hWnd))
	{
		return 0;
	}

	if (!memmonitor::Init(memmonitor::INNER_PROCESS,hInstance,"config_memmonitor.json" ))
	{
		MessageBoxA(NULL, memmonitor::GetLastError().c_str(), "ERROR", MB_OK);
	}

	global = new sGlobal;
	global->lButtonDown = false;
	global->mode = sGlobal::RENDER_MESH;

	InitVertexBuffer();

	ShowWindow( hWnd, nCmdShow );

	//메시지 구조체
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

//	int oldT = GetTickCount();
	int oldT = ::timeGetTime();
	while (msg.message != WM_QUIT)
	{
		//PeekMessage 는 메시지 큐에 메시지가 없어도 프로그램이 멈추기 않고 진행이 된다.
		//이때 메시지큐에 메시지가 없으면 false 가 리턴되고 메시지가 있으면 true 가 리턴이된다.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage( &msg ); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
			DispatchMessage( &msg );  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
		}
		else
		{
//			const int curT = GetTickCount();
			const int curT = ::timeGetTime();
			const int elapseT = curT - oldT;
		//	if (elapseT > 1000)
		//	{	
				oldT = curT;
				Render(elapseT);
		//	}
		}
	}

	if (g_pDevice)
		g_pDevice->Release();
	if (global->font)
		global->font->Release();
	if (global->textSprite)
		global->textSprite->Release();
	if (global->mesh3DText)
		global->mesh3DText->Release();
	delete global;
	memmonitor::Cleanup();
	return 0;
}


bool InitDirectX(HWND hWnd)
{
	LPDIRECT3D9 d3d9;
	d3d9 = Direct3DCreate9( D3D_SDK_VERSION );

	// 하드웨어 정보를 가지고 와서 자신의 정점 프로세스 타입을 정하자
	D3DCAPS9 caps;

	//Direct3D9 객체 통해 비디오 카드의 하드웨어 정보를 가지고 온다.
	d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,			//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,				//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		&caps						//디바이스 정보를 받아올 D3DCAPS9 포인터
		);

	//정점계산 처리방식을 지정할 플레그 값
	int vertexProcessing = 0;

	//정점 위치와 광원 계산시 하드웨어 사용이 가능한가
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


	//3. D3DPRESENT_PARAMETERS 구조체 정보를 생성
	//내가 이러한 Device 를 만들겟다라는 정보

	D3DPRESENT_PARAMETERS d3dpp;		
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//뎁스 버퍼와 스테실 버퍼 크기 뎁스 24bit 스텐실 버퍼 8 비트
	d3dpp.BackBufferCount = 1;						//백버퍼 갯수 그냥 1개
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;			//백버퍼 색상 포맷 알파8비트,레드8비트,그린8비트,블루8비트
	d3dpp.BackBufferHeight = WINSIZE_Y;				//백버퍼 픽셀 크기
	d3dpp.BackBufferWidth = WINSIZE_X;				//백버퍼 픽셀 크기
	d3dpp.EnableAutoDepthStencil = true;						//자동 깊이버퍼 사용 여부 ( 그냥 true )
	d3dpp.Flags = 0;						//추기 플래그 ( 일단 0 )
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;	//화면 주사율 ( 그냥 D3DPRESENT_RATE_DEFAULT 모니터 주사율과 동일시 )
	d3dpp.hDeviceWindow = hWnd;					//Device 가 출력될 윈도우 핸들
	d3dpp.MultiSampleQuality = 0;						//멀티 샘플링 질
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		//멀티 샘플링 타입 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	//화면 전송 간격 ( 그냥 D3DPRESENT_INTERVAL_ONE 모니터 주사율과 동일시 )
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//화면 스왑 체인 방식
	d3dpp.Windowed = true;						//윈도우 모드냐? ( 이게 false 면 풀스크린 된다! )

	//4. Device 생성
	if (FAILED(d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,					//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,						//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		hWnd,								//디바이스를 사용할 윈도우 핸들
		vertexProcessing,					//정점 처리 방식에 대한 플레그
		&d3dpp,								//앞에서 정의한 D3DPRESENT_PARAMETERS 구조체 포인터
		&g_pDevice							//얻어올 디바이스 더블포인터
		)))
	{
		//디바이스 생성 실패
		d3d9->Release(); // Deivce 를 만들기 위해 생성된 Direct3D9 객체를 해제
		d3d9 = NULL;

		MessageBoxA( hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK );
		return false;
	}

	//디바이스 생성 성공
	d3d9->Release(); // Deivce 를 만들었으니 넌 더이상 필요없다 ( 사라져라! )
	d3d9 = NULL;
	return true;
}


bool InitVertexBuffer()
{
	HRESULT hr = D3DXCreateFont( g_pDevice, // D3D device
		0,               // Height
		0,                     // Width
		FW_BOLD,               // Weight
		1,                     // MipLevels, 0 = autogen mipmaps
		FALSE,                 // Italic
		DEFAULT_CHARSET,       // 그냥 디폴트
		OUT_DEFAULT_PRECIS,    // 정밀도
		DEFAULT_QUALITY,       // 그냥 디폴트
		DEFAULT_PITCH | FF_DONTCARE, // 디폴트
		L"굴림",              // pFaceName
		&global->font );              // ppFont
	if (FAILED(hr))
		return false;

	if (FAILED(hr = D3DXCreateSprite(g_pDevice, &global->textSprite)))
		return false;

	CreateD3DXTextMesh(g_pDevice, &global->mesh3DText, "굴림", 0, FALSE, FALSE);



	Matrix44 V;
	Vector3 dir = Vector3(0,0,0)-Vector3(0,0,-500);
	dir.Normalize();
	V.SetView(Vector3(0,0,-500), dir, Vector3(0,1,0));
	g_pDevice->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&V);

	Matrix44 proj;
	proj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	g_pDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;

	g_pDevice->LightEnable(0, true);

	//g_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	//g_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true) ;

	return true;
}


//
// 윈도우 프로시져 함수 ( 메시지 큐에서 받아온 메시지를 처리한다 )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::DestroyWindow(hWnd);
			break;

		case VK_TAB:
			{
				static bool flag = false;
				g_pDevice->SetRenderState(D3DRS_CULLMODE, flag? D3DCULL_CCW : D3DCULL_NONE);
				g_pDevice->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
				flag = !flag;
			}
			break;

		case '1':
			global->mode = sGlobal::RENDER_MESH;
			break;

		case '2':
			global->mode = sGlobal::RENDER_FACE_NORMAL;
			break;

		case '3':
			global->mode = sGlobal::RENDER_VERTEX_NORMAL;
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			global->lButtonDown = true;
			global->curPos.x = LOWORD(lParam);
			global->curPos.y = HIWORD(lParam);
		}
		break;

	case WM_LBUTTONUP:
		global->lButtonDown = false;
		break;

	case WM_MOUSEMOVE:
		if (global->lButtonDown)
		{
			POINT pos = {LOWORD(lParam), HIWORD(lParam)};
			const int x = pos.x - global->curPos.x;
			const int y = pos.y - global->curPos.y;
			global->curPos = pos;

			Matrix44 mat1;
			mat1.SetRotationY( -x * 0.01f );
			Matrix44 mat2;
			mat2.SetRotationX( -y * 0.01f );

			global->localTm *= (mat1 * mat2);
		}
		break;

	case WM_DESTROY: //윈도우가 파괴된다면..
		PostQuitMessage(0);	//프로그램 종료 요청 ( 메시지 루프를 빠져나가게 된다 )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

string format(const char* fmt, ...)  //스트링으로 변환해주는 함수
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );
	return textString;
}

//랜더
void Render(int timeDelta)
{
	//화면 청소
	if (SUCCEEDED(g_pDevice->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(150, 150, 150),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		g_pDevice->BeginScene();

		//fps출력
		static WORD frameCnt = 0;
		static DWORD ntimeDelay = 0;
		static float fps = 0.f;
				
		frameCnt++;
		ntimeDelay += timeDelta;
		if( ntimeDelay >= 1000 )
		{	
			fps = (float)frameCnt;
			frameCnt = 0;
			ntimeDelay -= ntimeDelay;
		}		
		string outputFps = format( "%0.3f", fps );
		RECT rc;
		SetRect( &rc, 150, 100, 0, 0 );  //다이렉트 생성할때의 화면 크기의 좌표로 적용됨
		global->font->DrawTextA( NULL,   //A : 아스키코드
//			"global->font->DrawText", 
			outputFps.c_str(),
			-1, 
			&rc, 
			DT_NOCLIP,  //다 출력하겠다라는 의미(화면 크기 연연하지 않겠다인듯??)
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) );
				

		//쿼터니언 응용	
		/*
		Quaternion quat;
		quat.SetRotationArc(Vector3(0,1,0),Vector3(0,0,1));  //첫번째 인자방향 기준으로 두번째 인자 방향으로
		Matrix44 qt = quat.GetMatrix();
		*/
		static float ftheta = 0.f;
		ftheta += 0.0005f;
		const float fradian = ftheta / MATH_PI;
		if( ftheta >= 360.f )
			ftheta = 0.f;
		Matrix44 mat;
		mat.SetTranslate(Vector3(5, sin(fradian) * 10.f, -490));  //sin(fradian) : 각도에 따른 +-변화  //10.f 곱하기는 범위
//		Matrix44 m = global->localTm * mat;
//		g_pDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&m);  //마우스 회전 잠금
		g_pDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&mat);
		global->mesh3DText->DrawSubset( 0 );
				
		Vector3 mainPos(-10, 0, -490);
		Vector3 targetLook = mat.GetPosition() - mainPos;  //대상물체와 메인물체와의 거리 판단
		targetLook.Normalize();
		Quaternion quat;
//		mainPos.Normalize();
//		quat.SetRotationArc(mainPos, targetLook);   //첫번째 인자를 물체의 위치로 하니 회전하는 방향이 생각과는 다르게 표현된다...역시 고정된 방향이어야 되나보다...
		quat.SetRotationArc(Vector3(1,0,0), targetLook);  //양의 x축방향으로 대상물체를 바라본다
		Matrix44 qm = quat.GetMatrix();
		mat.SetTranslate(mainPos);
		qm *= mat;
		g_pDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&qm);
		global->mesh3DText->DrawSubset( 0 );
		
		//랜더링 끝
		g_pDevice->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		g_pDevice->Present( NULL, NULL, NULL, NULL );
	}
}


HRESULT CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice,
	LPD3DXMESH* ppMesh,
	char* pstrFont, DWORD dwSize,
	BOOL bBold, BOOL bItalic )
{
	HRESULT hr;
	LPD3DXMESH pMeshNew = NULL;
	HDC hdc = CreateCompatibleDC( NULL );
	if( hdc == NULL )
		return E_OUTOFMEMORY;
	INT nHeight = -MulDiv( dwSize, GetDeviceCaps( hdc, LOGPIXELSY ), 72 );
	HFONT hFont;
	HFONT hFontOld;

	hFont = CreateFontA( nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		pstrFont );

	hFontOld = ( HFONT )SelectObject( hdc, hFont );

	hr = D3DXCreateText( pd3dDevice, hdc, L"This is calling D3DXCreateText",
		0.001f, 0.4f, &pMeshNew, NULL, NULL );

	SelectObject( hdc, hFontOld );
	DeleteObject( hFont );
	DeleteDC( hdc );

	if( SUCCEEDED( hr ) )
		*ppMesh = pMeshNew;

	return hr;
}
