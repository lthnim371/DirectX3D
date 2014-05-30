#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <fstream>
#include "../../math/Math.h"
#include "DrawTriangle.h"

using namespace std;

const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
const int WINPOS_X = 0; //�ʱ� ������ ���� ��ġ X
const int WINPOS_Y = 0; //�ʱ� ������ ���� ��ġ Y

HWND g_hWnd;
vector<Vector3> g_vertices;
vector<Vector3> g_normals;
vector<int> g_indices;
Matrix44 g_matWorld1;
Matrix44 g_matLocal1;
Matrix44 g_matView;
Matrix44 g_matProjection;
Matrix44 g_matViewPort;
Vector3 g_cameraPos(0,200,-400);
Vector3 g_cameraLookat(0,0,0);

//ĳ���� �ڷ�
Matrix44 g_matLocal2;
Matrix44 g_matWorld2;
vector<Vector3> g_vertices2;
vector<Vector3> g_normals2;
vector<int> g_indices2;

// �ݹ� ���ν��� �Լ� ������ Ÿ��
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
void Init();
void MainLoop(int elapse_time);
void	Render(HWND hWnd);
void	Paint(HWND hWnd, HDC hdc);
bool ReadModelFile( const string &fileName, vector<Vector3> &vertices, vector<int> &indices, 
	vector<Vector3> &normals);


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

	g_hWnd = hWnd;

	//�����츦 ��Ȯ�� �۾����� ũ��� �����
	RECT rcClient = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient ũ�⸦ �۾� �������� �� ������ ũ�⸦ rcClient �� ���ԵǾ� ���´�.

	//������ ũ��� ������ ��ġ�� �ٲپ��ش�.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	ShowWindow( hWnd, nCmdShow );

	Init();


	//�޽��� ����ü
	MSG msg;		
	ZeroMemory( &msg, sizeof( MSG ) );

	int oldT = GetTickCount();
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
			const int curT = GetTickCount();
			const int elapseT = curT - oldT;
			if (elapseT > 15)
			{
				oldT = curT;
				MainLoop(elapseT);
			}
		}
	}

	return 0;
}


//
// ������ ���ν��� �Լ� ( �޽��� ť���� �޾ƿ� �޽����� ó���Ѵ� )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_ERASEBKGND:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Paint(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::DestroyWindow(hWnd);
			break;

		case 'D':
		case 'A':
			{
				g_cameraPos.x += (wParam=='D')? 100.f : -100.f;
				Vector3 dir = g_cameraLookat - g_cameraPos;
				dir.Normalize();
				g_matView.SetView(g_cameraPos, dir, Vector3(0,1,0));
			}
			break;

		case 'W':
		case 'S':
			{
				g_cameraPos.z += (wParam=='W')? 100.f : -100.f;
				Vector3 dir = g_cameraLookat - g_cameraPos;
				dir.Normalize();
				g_matView.SetView(g_cameraPos, dir, Vector3(0,1,0));
			}
			break;

		case VK_UP:
		case VK_DOWN:
			{
				Matrix44 mat;
				mat.SetRotationX((wParam==VK_UP)? 0.1f : -0.1f);
				g_matLocal1 *= mat;
			}
			break;

		case VK_LEFT:
		case VK_RIGHT:
			{
				Matrix44 mat;
				mat.SetRotationY((wParam==VK_LEFT)? 0.1f : -0.1f);
				g_matLocal1 *= mat;
			}
			break;
		}
		break;
	case WM_DESTROY: //�����찡 �ı��ȴٸ�..
		PostQuitMessage(0);	//���α׷� ���� ��û ( �޽��� ������ ���������� �ȴ� )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}


void Init()
{
	ReadModelFile("../../media/plane.dat", g_vertices, g_indices, g_normals);  //�ٴ� �ҷ�����
	ReadModelFile("../../media/Character.dat", g_vertices2, g_indices2, g_normals2);  //ĳ���� �ҷ�����

	//�ٴ� ����
	g_matWorld1.SetTranslate(Vector3(0,0,0));
	g_matWorld1.SetScale(Vector3(3,3,3));

	//ĳ���� ����
	g_matWorld2.SetTranslate(Vector3(0,0,0));

	Vector3 dir = g_cameraLookat - g_cameraPos;
	dir.Normalize();
	g_matView.SetView(g_cameraPos, dir, Vector3(0,1,0));
	g_matProjection.SetProjection( MATH_PI / 4.f, 1.0f, 1.0f, 100.0f );

	RECT cr;
	::GetClientRect(g_hWnd, &cr);
	const float width = (float)(cr.right-cr.left);
	const float height = (float)(cr.bottom - cr.top);
	g_matViewPort.SetViewport(width, height);
}


/**
 @brief 
 */
void	MainLoop(int elapse_time)
{
	//Matrix44 mat;
	//mat.SetRotationY(elapse_time*0.0002f);
	//g_matLocal1 *= mat;

//	Matrix44 mapMoving;
//	mapMoving.SetTranslate(Vector3(0,0,-5));
//	g_matWorld1 *= mapMoving;

	//ĳ���� ������
	Matrix44 characterMoving;
	characterMoving.SetTranslate(Vector3(0,0,5));
	g_matWorld2 *= characterMoving;
	
	//ī�޶� ������
	g_cameraPos *= characterMoving;
	g_cameraLookat *= characterMoving;
	Vector3 direction = g_cameraLookat - g_cameraPos;
	direction.Normalize();
	g_matView.SetView(g_cameraPos, direction, Vector3(0,1,0));

	// Render
	Render(g_hWnd);
	::InvalidateRect(g_hWnd, NULL, TRUE);
}


/**
 @brief 
 */
void	Render(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	Paint(hWnd, hdc);
	::ReleaseDC(hWnd, hdc);
}



bool ReadModelFile( const string &fileName, vector<Vector3> &vertices, vector<int> &indices,
	vector<Vector3> &normals)
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string vtx, eq;
	int numVertices;
	fin >> vtx >> eq >> numVertices;

	if (numVertices <= 0)
		return  false;

	vertices.resize(numVertices);

	float num1, num2, num3;
	for (int i = 0; i < numVertices; i++)
	{
		fin >> num1 >> num2 >> num3;
		vertices[i] = Vector3(num1, num2, num3);
	}

	string idx;
	int numIndices;
	fin >> idx >> eq >> numIndices;

	if (numIndices <= 0)
		return false;

	indices.resize(numIndices*3);

	int num4, num5, num6;
	for (int i = 0; i < numIndices*3; i+=3)
	{
		fin >> num4 >> num5 >> num6;
		indices[ i] = num4;
		indices[ i+1] = num5;
		indices[ i+2] = num6;	
	}


	string norm;
	int numNormal;
	fin >> norm >> eq >> numNormal;

	normals.resize(numVertices);

	if (numNormal > 0)
	{
		float num1, num2, num3;
		vector<int> vertCount(numVertices, 0);
		for (int i = 0; i < numNormal; i++)
		{
			fin >> num1 >> num2 >> num3;
			Vector3 n(num1, num2, num3);

			// ���������� ����� ���ؼ� �Ҵ��Ѵ�.
			for (int k=0; k < 3; ++k)
			{
				const int vtxIdx = indices[ i*3 + k];
				normals[ vtxIdx] += n;
				++vertCount[ vtxIdx];
			}
		}

		for (int i=0; i < numVertices; ++i)
		{
			normals[ i] /= (float)vertCount[ i];
			normals[ i].Normalize();
		}
	}


	return true;
}


void RenderVertices(HDC hdc, const vector<Vector3> &vertices, const vector<int> &indices,
	const Matrix44 &tm, const Matrix44 &vpv)
{
	/*for (unsigned int i=0; i < vertices.size(); ++i)
	{
		Vector3 p = vertices[ i] * tm * vpv;

		if (0 == i)
			MoveToEx(hdc, (int)p.x, (int)p.y, NULL);
		else
			LineTo(hdc, (int)p.x, (int)p.y);
	}*/
	
	Vector3 ground[3];
	Matrix44 mapLoop;
	float loopCount = 0.f;

	for(int j=0; j<2; ++j)
	{
		for(unsigned int i=0; i<indices.size(); i+=3)
		{
			ground[0] = vertices[ indices[ i]];
			ground[1] = vertices[ indices[ i+1]];
			ground[2] = vertices[ indices[ i+2]];

			ground[0] *= tm * mapLoop;
			ground[1] *= tm * mapLoop;
			ground[2] *= tm * mapLoop;

			if(g_cameraPos.z + 200 < ground[2].z)
			{
				ground[0] *= vpv;
				ground[1] *= vpv;
				ground[2] *= vpv;
			
				Rasterizer::Color c0(0,0,255,1);
				Rasterizer::DrawLine(hdc, c0, ground[0].x, ground[0].y, c0, ground[1].x, ground[1].y);
				Rasterizer::DrawLine(hdc, c0, ground[1].x, ground[1].y, c0, ground[2].x, ground[2].y);
				Rasterizer::DrawLine(hdc, c0, ground[2].x, ground[2].y, c0, ground[0].x, ground[0].y);
			}
			else if(i == 30 && g_cameraPos.z > ground[2].z)
			{
				Matrix44 Test;
				Test.SetTranslate(Vector3(0,0,1400));
				g_matWorld1 *= Test;
			}
		}
		loopCount += 350.f;
		mapLoop.SetTranslate(Vector3(0,0,loopCount));
	}
}


void RenderIndices(HDC hdc, const vector<Vector3> &vertices, const vector<int> &indices, 
	vector<Vector3> &normals,
	const Matrix44 &tm, const Matrix44 &vpv)
{
	Vector3 camDir = g_cameraLookat - g_cameraPos;
	camDir.Normalize();

	for (unsigned int i=0; i < indices.size(); i+=3)
	{
		Vector3 p1 = vertices[ indices[ i]];
		Vector3 p2 = vertices[ indices[ i+1]];
		Vector3 p3 = vertices[ indices[ i+2]];

		p1 = p1 * tm;
		p2 = p2 * tm;
		p3 = p3 * tm;

		// culling
		Vector3 n = normals[ indices[ i]];
		n = n.MultiplyNormal(tm);
		if (n.DotProduct(camDir) > 0)
			continue;

		p1 = p1 * vpv;
		p2 = p2 * vpv;
		p3 = p3 * vpv;

		Rasterizer::Color c0(0,0,255,1);
		Vector3 lightDir(0,-1,0);
		Rasterizer::Color color = c0 * max(0, n.DotProduct(-lightDir));
		Rasterizer::DrawTriangle(hdc, color, p1.x, p1.y, color, p2.x, p2.y, color, p3.x, p3.y);
	}
}


/**
 @brief 
 */
void Paint(HWND hWnd, HDC hdc)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rc.right-rc.left, rc.bottom-rc.top);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
	HBRUSH hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	FillRect(hdcMem, &rc, hbrBkGnd);
	DeleteObject(hbrBkGnd);

	Matrix44 vpv = g_matView * g_matProjection * g_matViewPort;
	//RenderIndices(hdcMem, g_vertices, g_indices, g_normals, g_matLocal1 * g_matWorld1,  vpv);

	Matrix44 Test;
	Test.SetTranslate(Vector3(0,0,700));

	RenderVertices(hdcMem, g_vertices, g_indices, g_matWorld1, vpv);
	RenderVertices(hdcMem, g_vertices, g_indices, g_matWorld1 * Test, vpv);
	RenderIndices(hdcMem, g_vertices2, g_indices2, g_normals2, g_matLocal2 * g_matWorld2,  vpv);

	BitBlt(hdc, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
}