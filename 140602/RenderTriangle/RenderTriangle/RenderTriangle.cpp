//�� �ϰ��� ����ҷ��� �ߴµ� ����� ����� �ȵȴٸ� FVF�� �ǽ��Ͽ���


#include <windows.h>
#include <d3d9.h>

const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
const int WINPOS_X = 0; //�ʱ� ������ ���� ��ġ X
const int WINPOS_Y = 0; //�ʱ� ������ ���� ��ġ Y

//�ʱ�ȭ1
#pragma comment( lib, "d3d9.lib" )
//���̷�Ʈ�� ���ϴ°���(��������) 
LPDIRECT3DDEVICE9	g_pDevice = NULL;
//�� 9 : ���ڴ� ���̷�Ʈ ����
//g_pDevice : ���̷�Ʈ ��ġ ����̹�
//1

//���ؽ�1
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // ���ؽ� ����  //9 : ���̷�Ʈ ����
//1

//���ؽ�2
// ���ؽ� ����ü
struct Vertex  //���ؽ����ۿ� �븻���� ���� ���� ����ִ�. �ص����� �κ� ����
{
	//����ü ���� �߿��ϴ�
	FLOAT x, y, z, rhw; // The transformed position for the vertex
	DWORD color;        // The vertex color
	static const DWORD FVF;
};
//���ؽ� ����ü ����.
const DWORD Vertex::FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;  //(����)����ü ������ ���� ������ �´� ������ ��ɵ� ���󰡾��Ѵ�.
//2

// �ݹ� ���ν��� �Լ� ������ Ÿ�� 
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

//������1
void Render();
//1

//�ʱ�ȭ3
bool InitDirectX(HWND hWnd);  //���̷�Ʈ �ʱ�ȭ �Լ� ����
//3

//���ؽ�4
bool InitVertexBuffer();
//4

int APIENTRY WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	wchar_t className[32] = L"Sample";
	wchar_t windowName[32] = L"Sample";

	//������ Ŭ���� ���� ����
	//���� �̷��� ������ ����ڴ� ��� ����
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.cbWndExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);		//������ ������
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//�������� Ŀ����� ����
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );		//����������ܸ�� ����
	WndClass.hInstance = hInstance;				//���α׷��ν��Ͻ��ڵ� 
	WndClass.lpfnWndProc = (WNDPROC)WndProc;			//������ ���ν��� �Լ� ������
	WndClass.lpszMenuName = NULL;						//�޴��̸� ������ NULL
	WndClass.lpszClassName = className;				//���� �ۼ��ϰ� �ִ� ������ Ŭ������ �̸�
	WndClass.style	 = CS_HREDRAW | CS_VREDRAW;	//������ �׸��� ��� ���� ( ����� ����ɶ� ȭ�鰻�� CS_HREDRAW | CS_VREDRAW )

	//������ �ۼ��� ������ Ŭ�������� ���
	RegisterClass( &WndClass );

	//������ ����
	//������ ������ �ڵ��� �������� g_hWnd �� �޴´�.
	HWND hWnd = CreateWindow(
		className,				//�����Ǵ� �������� Ŭ�����̸�
		windowName,				//������ Ÿ��Ʋ�ٿ� ��µǴ� �̸�
		WS_OVERLAPPEDWINDOW,	//������ ��Ÿ�� WS_OVERLAPPEDWINDOW
		WINPOS_X,				//������ ���� ��ġ X 
		WINPOS_Y,				//������ ���� ��ġ Y
		WINSIZE_X,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		WINSIZE_Y,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		GetDesktopWindow(),		//�θ� ������ �ڵ� ( ���α׷����� �ֻ��� ������� NULL �Ǵ� GetDesktopWindow() )
		NULL,					//�޴� ID ( �ڽ��� ��Ʈ�� ��ü�� �������ΰ�� ��Ʈ�� ID �� ��	
		hInstance,				//�� �����찡 ���� ���α׷� �ν��Ͻ� �ڵ�
		NULL					//�߰� ���� NULL ( �Ű���� )
		);

	//�����츦 ��Ȯ�� �۾����� ũ��� �����
	RECT rcClient = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient ũ�⸦ �۾� �������� �� ������ ũ�⸦ rcClient �� ���ԵǾ� ���´�.

	//������ ũ��� ������ ��ġ�� �ٲپ��ش�.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	//4
	if (!InitDirectX(hWnd))  //���̷�Ʈ �ʱ�ȭ �Լ� ȣ��
	{
		return 0;
	}
	//

	//���ؽ�5
	InitVertexBuffer();
	//5
			
	ShowWindow( hWnd, nCmdShow );

	//�޽��� ����ü
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

	while (msg.message != WM_QUIT)
	{
		//PeekMessage �� �޽��� ť�� �޽����� ��� ���α׷��� ���߱� �ʰ� ������ �ȴ�.
		//�̶� �޽���ť�� �޽����� ������ false �� ���ϵǰ� �޽����� ������ true �� �����̵ȴ�.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage( &msg ); //���� Ű���� �� ���ڸ� �����Ͽ� WM_CHAR �޽����� �߻���Ų��.
			DispatchMessage( &msg );  //�޾ƿ� �޽��� ������ ������ ���ν��� �Լ��� �����Ų��.
		}
		else
		{
			//������2
			Render();
			//2
		}
	}

	// 2
	if (g_pDevice)
		g_pDevice->Release();  //��???�Լ��� �������Լ��� ���ؼ� �����ؾ���
	//

	return 0;
}


//
// ������ ���ν��� �Լ� ( �޽��� ť���� �޾ƿ� �޽����� ó���Ѵ� )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		break;
	case WM_DESTROY: //�����찡 �ı��ȴٸ�..
		PostQuitMessage(0);	//���α׷� ���� ��û ( �޽��� ������ ���������� �ȴ� )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//5
bool InitDirectX(HWND hWnd)  //���̷�Ʈ �ʱ�ȭ �����ִ� �Լ�
{
	//6
	LPDIRECT3D9 d3d9;
	d3d9 = Direct3DCreate9( D3D_SDK_VERSION );  //D3D_SDK_VERSION : sdk ���� ������

	// �ϵ���� ������ ������ �ͼ� �ڽ��� ���� ���μ��� Ÿ���� ������
	D3DCAPS9 caps;

	//Direct3D9 ��ü ���� ���� ī���� �ϵ���� ������ ������ �´�.
	d3d9->GetDeviceCaps(  //GetDeviceCaps : ��ġ �Ű�ü �������� �Լ�
		D3DADAPTER_DEFAULT,			//�� ���÷��� �׷��� ī�� �׳� D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,				//����̽�Ÿ�� ���� �׳� D3DDEVTYPE_HAL
		&caps						//����̽� ������ �޾ƿ� D3DCAPS9 ������  //�� �Լ��� ȣ���ؼ� ���� ä����� �������ش�
		);

	//������� ó������� ������ �÷��� ��
	int vertexProcessing = 0;

	//���� ��ġ�� ���� ���� �ϵ���� ����� �����Ѱ�
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	//vertexProcessing : ���̷�Ʈ ����̽� �����ϴµ� ���
	//6

	
	//7 - ��å ����
	//3. D3DPRESENT_PARAMETERS ����ü ������ ����
	//���� �̷��� Device �� ����ٴٶ�� ����

	D3DPRESENT_PARAMETERS d3dpp;		
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;			//���� ���ۿ� ���׽� ���� ũ�� ���� 24bit ���ٽ� ���� 8 ��Ʈ
	d3dpp.BackBufferCount = 1;						//����� ���� �׳� 1��
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;			//����� ���� ���� ����8��Ʈ,����8��Ʈ,�׸�8��Ʈ,���8��Ʈ
	d3dpp.BackBufferHeight = WINSIZE_Y;				//����� �ȼ� ũ��
	d3dpp.BackBufferWidth = WINSIZE_X;				//����� �ȼ� ũ��
	d3dpp.EnableAutoDepthStencil = true;						//�ڵ� ���̹��� ��� ���� ( �׳� true )
	d3dpp.Flags = 0;						//�߱� �÷��� ( �ϴ� 0 )
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;	//ȭ�� �ֻ��� ( �׳� D3DPRESENT_RATE_DEFAULT ����� �ֻ����� ���Ͻ� )
	d3dpp.hDeviceWindow = hWnd;					//Device �� ��µ� ������ �ڵ�
	d3dpp.MultiSampleQuality = 0;						//��Ƽ ���ø� ��
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		//��Ƽ ���ø� Ÿ�� 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//ȭ�� ���� ���� ( �׳� D3DPRESENT_INTERVAL_ONE ����� �ֻ����� ���Ͻ� )
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//ȭ�� ���� ü�� ���
	d3dpp.Windowed = true;						//������ ����? ( �̰� false �� Ǯ��ũ�� �ȴ�! )

	//4. Device ����
	if (FAILED(d3d9->CreateDevice(  //� �Լ��� ȣ���ϸ� ���� ä����(g_pDevice��)
		D3DADAPTER_DEFAULT,					//�� ���÷��� �׷��� ī�� �׳� D3DADAPTER_DEFAULT
		D3DDEVTYPE_HAL,						//����̽�Ÿ�� ���� �׳� D3DDEVTYPE_HAL
		hWnd,								//����̽��� ����� ������ �ڵ�
		vertexProcessing,					//���� ó�� ��Ŀ� ���� �÷���  //����Ʈ����, �ϵ���� �� ������� ������ �Ұ��� �Ǵ�
		&d3dpp,								//�տ��� ������ D3DPRESENT_PARAMETERS ����ü ������
		&g_pDevice							//���� ����̽� ����������  //�������ش�(�������Ҵ����)
		)))
	{
		//����̽� ���� ����
		d3d9->Release(); // Deivce �� ����� ���� ������ Direct3D9 ��ü�� ����
		d3d9 = NULL;  //d3d9�� ������ g_pDevice�� ����� ���� �ͻ�

		MessageBoxA( hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK );
		return false;
	}

	//���ؽ�3
	if (g_pVB)  //������� ������������Ѵ�
		g_pVB->Release();
	//3

	//����̽� ���� ����
	d3d9->Release(); // Deivce �� ��������� �� ���̻� �ʿ���� ( �������! )
	d3d9 = NULL;
	//7
	

	return true;
}
//5

//������3
//����
void Render()
{
	//ȭ�� û�� //(�׻� ȣ���Ѵٰ� ����ȴ�)
	if (SUCCEEDED(g_pDevice->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����(�ĸ�), D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(0, 0, 0),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )  //�ĸ���� ä�� ����  //D3DCOLOR_XRGB : DWORD������ �Ǿ� �־� 
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1. 0 �� ī�޶󿡼� ���ϰ����, 1 �� ī�޶󿡼� ���� �� )  //���̹��� �ʱ�ȭ
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
		g_pDevice->BeginScene();

		//���ؽ�7
		g_pDevice->SetStreamSource( 0,  //�׳� 0
			g_pVB,  //���ؽ����� ���(ȭ�鿡 �׸��Ŵٶ��)
			0,  //������ ��½�ų ��ġ?
			sizeof( Vertex ) );  //����ü ũ��
		g_pDevice->SetFVF( Vertex::FVF );
		g_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0,   //D3DPT_TRIANGLELIST : �� ������ Ʈ���̾ޱ۷� �׸��ڴٴ� ��
			2 );  //Ʈ���̾ޱ��� ���� 2����� 2. ��, ���ؽ� ������ �������
		//7
		
		//������ ��
		g_pDevice->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		g_pDevice->Present( NULL, NULL, NULL, NULL );  //�������, �ĸ���� ���ҽ�Ŵ  //���� : � ��Ʈ���� �׸��ٴٴ� �� ����
	}
}
//3

//���ؽ�6
bool InitVertexBuffer()
{
	// ���ؽ� �ʱ�ȭ.
	const Vertex Vertices[] =
	{
		//�ð�������� �׸��ÿ�
		//ù��° Ʈ���̾ޱ�
		{ 50.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color  //rhw : ��ũ�� ��ǥ �״�� ���ڴ�(ī�޶�, ��??�� �Ⱦ��ڴٶ��)
		{ 850.0f, 650.0f, 0.5f, 1.0f, 0xff00ff00, }, //0xff00ff00 : ff - ����, �� �ڴ� RGB
		{  50.0f, 650.0f, 0.5f, 1.0f, 0xff00ffff, }, //D3DCOLOR_XRGB ��ũ�η� ����ϸ� �ɵ�
		//�ι�° Ʈ���̾ޱ�
		{ 50.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, },
		{ 850.0f, 50.0f, 0.5f, 1.0f, 0xff00ff00, },
		{ 850.0f, 650.0f, 0.5f, 1.0f, 0xff00ffff, },
	};

	// ���ؽ� ���� ����.
	if (FAILED(g_pDevice->CreateVertexBuffer( 6 * sizeof(Vertex),  //����ũ��  //����ü�� 3���ۿ� ��� 3 ���Ѱ���
		0, 
		Vertex::FVF,  //���ؽ����� ��ɷ� �Ұ���
		D3DPOOL_MANAGED,  //�޸�Ǯ
		&g_pVB, NULL)))
	{
		return false;
	}

	// ���ؽ� ���� �ʱ�ȭ.
	void *vertices;  //void* : memcpy�� ���� �������� �ҷ��� ����Ѱ���
	//Lock : cpu���� gpu���� �޸𸮸� �����÷����ϸ� �� �̻� �� �������� ���°�?????. �޸� �����Ϳ� �����Ϳ� �����ؼ� ��������  //���� �ؾ� �����͸� ������ �� �վ
	if (FAILED(g_pVB->Lock( 0, 0,  //��� ������
		( void** )&vertices, 0)))
		return false;
	memcpy( vertices, Vertices, sizeof( Vertices ) );  //�׷��� �� ���� �����ؿ���
	g_pVB->Unlock();  //�� ������ ���

	//�׷��� ������ ������ ����� �����
	//Vertex *vertices;	
	//if (FAILED(g_pVB->Lock( 0, 0,  //��� ������
	//	( void** )&vertices, 0)))
	//	return false;
	//vertices[0] = Vertices[0];
	//vertices[1] = Vertices[1];
	//vertices[2] = Vertices[2];
	//g_pVB->Unlock();  //�� ������ ���

	return true;
}
//6



