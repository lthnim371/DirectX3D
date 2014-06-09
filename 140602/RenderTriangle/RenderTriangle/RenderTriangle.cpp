//다 하고나서 출력할려고 했는데 출력이 제대로 안된다면 FVF를 의심하여라


#include <windows.h>
#include <d3d9.h>

const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
const int WINPOS_X = 0; //초기 윈도우 시작 위치 X
const int WINPOS_Y = 0; //초기 윈도우 시작 위치 Y

//초기화1
#pragma comment( lib, "d3d9.lib" )
//다이렉트를 말하는거임(포인터임) 
LPDIRECT3DDEVICE9	g_pDevice = NULL;
//※ 9 : 숫자는 다이렉트 버전
//g_pDevice : 다이렉트 장치 드라이버
//1

//버텍스1
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // 버텍스 버퍼  //9 : 다이렉트 버전
//1

//버텍스2
// 버텍스 구조체
struct Vertex  //버텍스버퍼와 노말벡터 등이 같이 들어있다. ※디파인 부분 참고
{
	//구조체 순서 중요하다
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
	static const DWORD FVF;
};
//버텍스 구조체 포맷.
const DWORD Vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;  //(위의)구조체 순서에 따라 각가에 맞는 디파인 명령도 따라가야한다.
//2

// 콜백 프로시져 함수 프로토 타입 
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

//랜더링1
void Render();
//1

//초기화3
bool InitDirectX(HWND hWnd);  //다이렉트 초기화 함수 선언
//3

//버텍스4
bool InitVertexBuffer();
//4

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

	//초기화4
	if (!InitDirectX(hWnd))  //다이렉트 초기화 함수 호출
	{
		return 0;
	}
	//

	//버텍스5
	InitVertexBuffer();
	//5
			
	ShowWindow( hWnd, nCmdShow );

	//메시지 구조체
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

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
			//랜더링2
			Render();
			//2
		}
	}

	//초기화2
	if (g_pDevice)
		g_pDevice->Release();  //폼???함수라서 릴리즈함수를 통해서 해제해야함
	//2

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
			PostQuitMessage(0);
		break;
	case WM_DESTROY: //윈도우가 파괴된다면..
		PostQuitMessage(0);	//프로그램 종료 요청 ( 메시지 루프를 빠져나가게 된다 )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//초기화5
bool InitDirectX(HWND hWnd)  //다이렉트 초기화 시켜주는 함수
{
	//초기화6
	LPDIRECT3D9 d3d9;
	d3d9 = Direct3DCreate9( D3D_SDK_VERSION );  //D3D_SDK_VERSION : sdk 버전 디파인

	// 하드웨어 정보를 가지고 와서 자신의 정점 프로세스 타입을 정하자
	D3DCAPS9 caps;

	//Direct3D9 객체 통해 비디오 카드의 하드웨어 정보를 가지고 온다.
	d3d9->GetDeviceCaps(  //GetDeviceCaps : 장치 매개체 가져오는 함수
		D3DADAPTER_DEFAULT,			//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,				//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		&caps						//디바이스 정보를 받아올 D3DCAPS9 포인터  //이 함수를 호출해서 값을 채우고나서 리턴해준다
		);

	//정점계산 처리방식을 지정할 플레그 값
	int vertexProcessing = 0;

	//정점 위치와 광원 계산시 하드웨어 사용이 가능한가
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	//vertexProcessing : 다이렉트 디바이스 생성하는데 사용
	//6

	
	//초기화7 - ※책 참고
	//3. D3DPRESENT_PARAMETERS 구조체 정보를 생성
	//내가 이러한 Device 를 만들겟다라는 정보

	D3DPRESENT_PARAMETERS d3dpp;		
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;			//뎁스 버퍼와 스테실 버퍼 크기 뎁스 24bit 스텐실 버퍼 8 비트
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
	if (FAILED(d3d9->CreateDevice(  //어떤 함수를 호출하면 값을 채워서(g_pDevice에)
		D3DADAPTER_DEFAULT,					//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,						//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		hWnd,								//디바이스를 사용할 윈도우 핸들
		vertexProcessing,					//정점 처리 방식에 대한 플레그  //소프트웨어, 하드웨어 중 어떤것으로 랜더링 할건지 판단
		&d3dpp,								//앞에서 정의한 D3DPRESENT_PARAMETERS 구조체 포인터
		&g_pDevice							//얻어올 디바이스 더블포인터  //리턴해준다(포인터할당받음)
		)))
	{
		//디바이스 생성 실패  <- ????뭐징???? else 말하는건가???
		d3d9->Release(); // Deivce 를 만들기 위해 생성된 Direct3D9 객체를 해제
		d3d9 = NULL;  //d3d9의 역할은 g_pDevice를 만들기 위한 것뿐

		MessageBoxA( hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK );
		return false;
	}

	//버텍스3
	if (g_pVB)  //만들고나면 해제시켜줘야한다
		g_pVB->Release();
	//3

	//디바이스 생성 성공
	d3d9->Release(); // Deivce 를 만들었으니 넌 더이상 필요없다 ( 사라져라! )
	d3d9 = NULL;
	//7
	

	return true;
}
//5

//랜더링3
//랜더
void Render()
{
	//화면 청소 //(항상 호출한다고 보면된다)
	if (SUCCEEDED(g_pDevice->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼(후면), D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(0, 0, 0),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )  //후면버퍼 채울 색상  //D3DCOLOR_XRGB : DWORD값으로 되어 있어 
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1. 0 이 카메라에서 제일가까운, 1 이 카메라에서 제일 먼 )  //깊이버퍼 초기화
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		g_pDevice->BeginScene();

		//버텍스7
		g_pDevice->SetStreamSource( 0,  //그냥 0
			g_pVB,  //버텍스버퍼 등록(화면에 그릴거다라는)
			0,  //실제로 출력시킬 위치?
			sizeof( Vertex ) );  //구조체 크기
		g_pDevice->SetFVF( Vertex::FVF );
		g_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0,   //D3DPT_TRIANGLELIST : 점 세개를 트라이앵글로 그리겠다는 뜻
			2 );  //트라이앵글이 만약 2개라면 2. 즉, 버텍스 갯수와 상관없음
		//7
		
		//랜더링 끝
		g_pDevice->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		g_pDevice->Present( NULL, NULL, NULL, NULL );  //전면버퍼, 후면버퍼 스왑시킴  //인자 : 어떤 렉트에만 그리겟다는 뜻 같음
	}
}
//3

//버텍스6
bool InitVertexBuffer()
{
	// 버텍스 초기화.
	const Vertex Vertices[] =
	{
		//시계방향으로 그리시오
		//첫번째 트라이앵글
		{ 50.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color  //rhw : 스크린 좌표 그대로 쓰겠다(카메라, 툼??을 안쓰겠다라는)
		{ 850.0f, 650.0f, 0.5f, 1.0f, 0xff00ff00, }, //0xff00ff00 : ff - 알파, 그 뒤는 RGB
		{  50.0f, 650.0f, 0.5f, 1.0f, 0xff00ffff, }, //D3DCOLOR_XRGB 매크로로 사용하면 될듯
		//두번째 트라이앵글
		{ 50.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, },
		{ 850.0f, 50.0f, 0.5f, 1.0f, 0xff00ff00, },
		{ 850.0f, 650.0f, 0.5f, 1.0f, 0xff00ffff, },
	};

	// 버텍스 버퍼 생성.
	if (FAILED(g_pDevice->CreateVertexBuffer( 6 * sizeof(Vertex),  //버퍼크기  //구조체가 3개밖에 없어서 3 곱한거임
		0, 
		Vertex::FVF,  //버텍스포맷 어떤걸로 할건지
		D3DPOOL_MANAGED,  //메모리풀
		&g_pVB, NULL)))
	{
		return false;
	}

	// 버텍스 버퍼 초기화.
	void *vertices;  //void* : memcpy로 쉽게 가져오게 할려고 사용한거임
	//Lock : cpu에서 gpu에서 메모리를 가져올려고하면 더 이상 못 가져가게 막는것?????. 메모리 포인터에 데이터에 접근해서 가져오기  //락을 해야 포인터를 가져올 수 잇어서
	if (FAILED(g_pVB->Lock( 0, 0,  //어디서 어디까지
		( void** )&vertices, 0)))
		return false;
	memcpy( vertices, Vertices, sizeof( Vertices ) );  //그래서 그 값을 복사해오기
	g_pVB->Unlock();  //다 끝나면 언락

	//그러나 실제로 앞으로 사용할 방법은
	//Vertex *vertices;	
	//if (FAILED(g_pVB->Lock( 0, 0,  //어디서 어디까지
	//	( void** )&vertices, 0)))
	//	return false;
	//vertices[0] = Vertices[0];
	//vertices[1] = Vertices[1];
	//vertices[2] = Vertices[2];
	//g_pVB->Unlock();  //다 끝나면 언락

	return true;
}
//6



