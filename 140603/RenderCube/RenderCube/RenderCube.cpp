﻿
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>  //사용할려면 속성에서 추가해줘야한다
#include "../math/Math.h"
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )  //사용할려면 속성에서 추가해줘야한다

LPDIRECT3DDEVICE9	g_pDevice = NULL;
const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
const int WINPOS_X = 0; //초기 윈도우 시작 위치 X
const int WINPOS_Y = 0; //초기 윈도우 시작 위치 Y

//1
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // 버텍스 버퍼
LPDIRECT3DINDEXBUFFER9 g_pIB = NULL; // 인덱스 버퍼
//1

//2
// 버텍스 구조체
struct Vertex
{
	Vertex() {}
	Vertex(float x0, float y0, float z0) : x(x0), y(y0), z(z0) {}
	float x, y, z;
	static const DWORD FVF;
};
//버텍스 구조체 포맷.
const DWORD Vertex::FVF  = D3DFVF_XYZ;  //순수 xyz만 사용하겠다.
//2




// 콜백 프로시져 함수 프로토 타입 
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
bool InitDirectX(HWND hWnd);
bool InitVertexBuffer();
void Render(int timeDelta);

int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Render Cube";
	wchar_t windowName[32] = L"Render Cube";

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

	InitVertexBuffer();

	ShowWindow( hWnd, nCmdShow );

	//메시지 구조체
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

	int oldT = GetTickCount();
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
			const int curT = GetTickCount();
			const int elapseT = curT - oldT;
			if (elapseT > 15)
			{
				oldT = curT;
				Render(elapseT);
			}
		}
	}

	//3
	if (g_pVB)
		g_pVB->Release();
	if (g_pIB)
		g_pIB->Release();
	//3

	if (g_pDevice)
		g_pDevice->Release();
	return 0;
}


//
// 윈도우 프로시져 함수 ( 메시지 큐에서 받아온 메시지를 처리한다 )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
		else if (wParam == VK_TAB)
		{
			static bool flag = true;
			g_pDevice->SetRenderState(D3DRS_CULLMODE, flag? D3DCULL_CCW : D3DCULL_NONE);
			g_pDevice->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
			flag = !flag;
		}
		break;
	case WM_DESTROY: //윈도우가 파괴된다면..
		PostQuitMessage(0);	//프로그램 종료 요청 ( 메시지 루프를 빠져나가게 된다 )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
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
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//화면 전송 간격 ( 그냥 D3DPRESENT_INTERVAL_ONE 모니터 주사율과 동일시 )
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


//랜더
void Render(int timeDelta)
{
	//화면 청소
	if (SUCCEEDED(g_pDevice->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(255, 255, 255),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		g_pDevice->BeginScene();

		//9
//		D3DXMATRIX Rx, Ry;
//		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f) ; // x축으로 45도 회전시킨다.
		
		//우리가 만든걸로 해보기
		Matrix44 Rx, Ry;
		Rx.SetRotationX(4.0f);
		static float y = 0;
		Ry.SetRotationY(y);
//		D3DXMatrixRotationY(&Ry, y); 
		y += timeDelta / 1000.f; // 각 프례임당 y 회전을 중가시킨다.
		if (y >= 6.28f) // 각도가 2*PI 에 이르면 0으로 초기화한다.
			y = 0;

//		D3DXMATRIX r;
		Matrix44 r;
		r = Rx * Ry;  //x축 먼저 회전하고 y축 회전시킴 ※곱하는 순서 중요
		g_pDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&r);
		
		g_pDevice->SetStreamSource( 0, g_pVB, 0, sizeof(Vertex) );  //버텍스 버퍼 등록
		g_pDevice->SetIndices(g_pIB);  //인덱스 적용
		g_pDevice->SetFVF( Vertex::FVF );  //
		g_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
			0, 0,
			8,  //버텍스 갯수
			0,
			12);  //트라이앵글 갯수
		//9

		Matrix44 t;
		t.SetTranslate(Vector3(1,1,0));
		g_pDevice->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&t);
		g_pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
			0, 0,
			8,  //버텍스 갯수
			0,
			12);  //트라이앵글 갯수
		//9

		//랜더링 끝
		g_pDevice->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		g_pDevice->Present( NULL, NULL, NULL, NULL );
	}
}


bool InitVertexBuffer()
{
	//4
	// 버텍스 버퍼 생성.
	if (FAILED(g_pDevice->CreateVertexBuffer( 8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY, Vertex::FVF,
		D3DPOOL_MANAGED, &g_pVB, NULL)))
	{
		return false;
	}


	// 버텍스 버퍼 초기화.
	Vertex* vertices;
	if (FAILED(g_pVB->Lock( 0, sizeof(Vertex), (void**)&vertices, 0)))
		return false;

	vertices[ 0] = Vertex(-1, -1, -1);
	vertices[ 1] = Vertex(-1, 1, -1);
	vertices[ 2] = Vertex(1, 1, -1);
	vertices[ 3] = Vertex(1, -1, -1);
	vertices[ 4] = Vertex(-1, -1, 1);
	vertices[ 5] = Vertex(-1, 1, 1);
	vertices[ 6] = Vertex(1, 1, 1);
	vertices[ 7] = Vertex(1, -1, 1);
	g_pVB->Unlock();
	//4

	//5
		if (FAILED(g_pDevice->CreateIndexBuffer(36*sizeof(WORD),  //정육면체 -> 면=6 -> 트라이앵글 = 12 -> 12 * 3 = 36
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIB, NULL)))
	{
		return false;
	}

	WORD *indices = NULL;
	g_pIB->Lock(0, 0, (void**)&indices, 0);

	//시계방향으로 설정
	int index = 0;
	// front
	indices[ index++] = 0; indices[ index++] = 1; indices[ index++] = 2;
	indices[ index++] = 0; indices[ index++] = 2; indices[ index++] = 3;

	// back
	indices[ index++] = 4; indices[ index++] = 6; indices[ index++] = 5;
	indices[ index++] = 4; indices[ index++] = 7; indices[ index++] = 6;

	// left
	indices[ index++] = 4; indices[ index++] = 5; indices[ index++] = 1;
	indices[ index++] = 4; indices[ index++] = 1; indices[ index++] = 0;

	// right
	indices[ index++] = 3; indices[ index++] = 2; indices[ index++] = 6;
	indices[ index++] = 3; indices[ index++] = 6; indices[ index++] = 7;

	// top
	indices[ index++] = 1; indices[ index++] = 5; indices[ index++] = 6;
	indices[ index++] = 1; indices[ index++] = 6; indices[ index++] = 2;

	// bottom
	indices[ index++] = 4; indices[ index++] = 0; indices[ index++] = 3;
	indices[ index++] = 4; indices[ index++] = 3; indices[ index++] = 7;
	g_pIB->Unlock();
	//5

	//6
	//우리가 만든걸로 해보기
//	D3DXVECTOR3 position(0,0,-5);
//	D3DXVECTOR3 target(0,0,0);
//	D3DXVECTOR3 up(0,1,0);
//	D3DXMATRIX V;
//	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	Vector3 position(0,0,-5);
	Vector3 look(0,0,0);
	Vector3 up(0,1,0);
	Matrix44 camera;
	camera.SetView(position, (look - position).Normal(), up);
	g_pDevice->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&camera);
	//6

	//7
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj ,D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	g_pDevice->SetTransform(D3DTS_PROJECTION, &proj) ;
	//7

	//8
	g_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE );  //컬링모드. 옵션 중 CW는 시계방향, CCW는 반시계방향. 디폴트는 CCW
	g_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);  //디폴트는 솔리드
	//8



	return true;
}
