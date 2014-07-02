//140701 과제 : 트리에 응용되는 재귀함수에 대해 이해하기

// TreeRender.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
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

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
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

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TREERENDER));

	// 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
//  설명:
//
//    Windows 95에서 추가된 'RegisterClassEx' 함수보다 먼저
//    해당 코드가 Win32 시스템과 호환되도록
//    하려는 경우에만 이 함수를 사용합니다. 이 함수를 호출해야
//    해당 응용 프로그램에 연결된
//    '올바른 형식의' 작은 아이콘을 가져올 수 있습니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
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
		// 메뉴 선택을 구문 분석합니다.
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


// 정보 대화 상자의 메시지 처리기입니다.
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

//원본을 인자로 받아와 만약 노드가 없다면 원본을 반환하고 노드가 있다면 더해진 값을 반환
//어느 한쪽에서 호출할 때는 똑같은 인자를 두번 넘기는 꼴이 되므로 약간의 비효율적인 면이 있기도 하다...
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


//위와 비슷한 원리로 반환되는 지역변수에 노드가 없다면 넘겨준 인자값 그대로 넣어주고 아니라면 더해진 값을 넣어준다
//설사 노드가 없더라도 반환되는 변수에 값을 누적시키는게 가능하기에 데이터가 많아질수록 범위가 늘어나므로 임의의 수치로 조절해줘야 된다...
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
