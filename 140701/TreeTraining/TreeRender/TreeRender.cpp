//140701 ���� : Ʈ���� ����Ǵ� ����Լ��� ���� �����ϱ�

// TreeRender.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "TreeRender.h"
#include <vector>
#include <string>
using namespace std;

struct sNode
{
	string name;
	sNode* left;
	sNode* right;

	sNode() {left=NULL, right=NULL;}
	sNode(const string &n) : name(n) {left=NULL, right=NULL;}
	sNode(const string &n, sNode *_l, sNode *_r) :
	name(n), left(_l), right(_r) {}
};
sNode *g_root;


#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
//void				print(HDC hdc, sNode *node, int x, int y);
int print(HDC hdc, sNode *node, int x, int y, int original);
int print(HDC hdc, sNode *node, int x, int y);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TREERENDER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	g_root = new sNode( "1",
		new sNode("2", 
			new sNode("4", 
				new sNode("8",
					new sNode("16", NULL, NULL),
					new sNode("17", NULL, NULL) ),//8
				new sNode("9",
					new sNode("18", NULL, NULL),
					new sNode("19", NULL, NULL)	)//9
					),//4,
			new sNode("5",
				new sNode("10",
					new sNode("20", NULL, NULL),
					new sNode("21", NULL, NULL) ),//10
				new sNode("11",
					new sNode("22", NULL, NULL),
					new sNode("23", NULL, NULL) )//11
		)//5
		),//2,
		new sNode("3", 
			new sNode("6",
				new sNode("12",
					new sNode("24", NULL, NULL),
					new sNode("25", NULL, NULL) ),//12
				new sNode("13",
					new sNode("26", NULL, NULL),
					new sNode("27", NULL, NULL) )//13
					),//6
			new sNode("7",
				new sNode("14",
					new sNode("28", NULL, NULL),
					new sNode("29", NULL, NULL) ),//14
				new sNode("15",
					new sNode("30", NULL, NULL),
					new sNode("31", NULL, NULL) )//15
					)//7
		)//3
	);//1

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TREERENDER));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
//  ����:
//
//    Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
//    �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
//    �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
//    �ش� ���� ���α׷��� �����
//    '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TREERENDER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TREERENDER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		print(hdc, g_root, 100, 100, 100);
	//	print(hdc, g_root, 50, 100);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//void print(HDC hdc, sNode *node, int x, int y)

//������ ���ڷ� �޾ƿ� ���� ��尡 ���ٸ� ������ ��ȯ�ϰ� ��尡 �ִٸ� ������ ���� ��ȯ
//��� ���ʿ��� ȣ���� ���� �Ȱ��� ���ڸ� �ι� �ѱ�� ���� �ǹǷ� �ణ�� ��ȿ������ ���� �ֱ⵵ �ϴ�...
int print(HDC hdc, sNode *node, int x, int y, int original)
{
	if (!node)
		return original;

	Ellipse(hdc, x, y, x+100, y+100);
	TextOutA(hdc, x+50, y+50, node->name.c_str(), node->name.length() );
	
	x = print(hdc, node->left, x, y + 100, x);
	x = print(hdc, node->right, x + 100, y + 100, x);

	return x;

	/*print(hdc, node->left, x, y + 100 );
	print(hdc, node->right, x + 200, y + 100 );*/
}


//���� ����� ������ ��ȯ�Ǵ� ���������� ��尡 ���ٸ� �Ѱ��� ���ڰ� �״�� �־��ְ� �ƴ϶�� ������ ���� �־��ش�
//���� ��尡 ������ ��ȯ�Ǵ� ������ ���� ������Ű�°� �����ϱ⿡ �����Ͱ� ���������� ������ �þ�Ƿ� ������ ��ġ�� ��������� �ȴ�...
int print(HDC hdc, sNode *node, int x, int y)
{	
	if (!node)
		return x;

	Ellipse(hdc, x, y, x+100, y+100);
	TextOutA(hdc, x+50, y+50, node->name.c_str(), node->name.length() );
	
	int result = 0;
	result = print(hdc, node->left, x, y + 100);
	result += 50;
	result = print(hdc, node->right, result, y + 100);

	return result;
}
