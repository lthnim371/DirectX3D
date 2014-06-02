//140530 과제 - 펭귄게임 만들기

//※ define OUT에 대해 물어보기

//bcoolition과 장애물 matWorld를 구조체로 사용해야 될듯 싶다

#include <windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <fstream>
#include "../../math/Math.h"
#include "DrawTriangle.h"
#include <tchar.h>

using namespace std;

const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
const int WINPOS_X = 0; //초기 윈도우 시작 위치 X
const int WINPOS_Y = 0; //초기 윈도우 시작 위치 Y

HWND g_hWnd;
vector<Vector3> g_vertices;
vector<Vector3> g_normals;
vector<int> g_indices;
Matrix44 g_matWorld1;
Matrix44 g_matLocal1;
Matrix44 g_matView;
Matrix44 g_matProjection;
Matrix44 g_matViewPort;
Vector3 g_cameraPos(0,200,-300);
Vector3 g_cameraLookat(0,0,100);

//바닥 자료
Matrix44 g_matWorld11;

//캐릭터 자료
Matrix44 g_matLocal2;
Matrix44 g_matWorld2;
vector<Vector3> g_vertices2;
vector<Vector3> g_normals2;
vector<int> g_indices2;
Box characterBox;

//장애물 자료
struct tagObstacle  //장애물 정보 모음
{
	tagObstacle(const Matrix44& Position)
	{
		this->position = Position;
		bCollision = false;
	}
	tagObstacle(const Vector3& Position)
	{
		position.SetTranslate(Position);
		bCollision = false;
	}
	
	Matrix44 position;
	Matrix44 rotation;
	bool bCollision;
};
vector<Vector3> g_vertices3;
vector<Vector3> g_normals3;
vector<int> g_indices3;
Matrix44 g_matLocal3;
Matrix44 g_matWorld3;
Box obstacleBox;
vector<tagObstacle*> obstacleData;

// 콜백 프로시져 함수 프로토 타입
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );
void Init();
void MainLoop(int elapse_time);
void	Render(HWND hWnd);
void	Paint(HWND hWnd, HDC hdc);
bool ReadModelFile( const string &fileName, vector<Vector3> &vertices, vector<int> &indices, 
	vector<Vector3> &normals);

void GetVerticesMinMax( const vector<Vector3> &vertices, OUT Vector3 &vMin, OUT Vector3 &vMax);


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

	g_hWnd = hWnd;

	//윈도우를 정확한 작업영역 크기로 맞춘다
	RECT rcClient = { 0, 0, WINSIZE_X, WINSIZE_Y };
	AdjustWindowRect( &rcClient, WS_OVERLAPPEDWINDOW, FALSE );	//rcClient 크기를 작업 영영으로 할 윈도우 크기를 rcClient 에 대입되어 나온다.

	//윈도우 크기와 윈도우 위치를 바꾸어준다.
	SetWindowPos( hWnd, NULL, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, 
		SWP_NOZORDER | SWP_NOMOVE );

	ShowWindow( hWnd, nCmdShow );

	Init();


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
				MainLoop(elapseT);
			}
		}
	}

	return 0;
}


//
// 윈도우 프로시져 함수 ( 메시지 큐에서 받아온 메시지를 처리한다 )
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
			{
				Matrix44 control;
				control.SetTranslate(Vector3(-3,0,0));
				g_matWorld2 *= control;
			}
			break;
		case VK_RIGHT:
			{
			/*	Matrix44 mat;
				mat.SetRotationY((wParam==VK_LEFT)? 0.1f : -0.1f);
				g_matLocal1 *= mat;*/
				Matrix44 control;
				control.SetTranslate(Vector3(3,0,0));
				g_matWorld2 *= control;
			}
			break;
		}
		break;
	case WM_DESTROY: //윈도우가 파괴된다면..
		PostQuitMessage(0);	//프로그램 종료 요청 ( 메시지 루프를 빠져나가게 된다 )
		break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}


void Init()
{
	ReadModelFile("../../media/plane.dat", g_vertices, g_indices, g_normals);  //바닥 불러오기
	ReadModelFile("../../media/character.dat", g_vertices2, g_indices2, g_normals2);  //캐릭터 불러오기
	ReadModelFile("../../media/obstacle.dat", g_vertices3, g_indices3, g_normals3);  //장애물 불러오기

	//바닥 설정
	g_matWorld1.SetTranslate(Vector3(0,0,0));
	g_matWorld1.SetScale(Vector3(3,3,3));
	g_matWorld11.SetTranslate(Vector3(0,0,700));
	g_matWorld11.SetScale(Vector3(3,3,3));

	//캐릭터 설정
	g_matWorld2.SetTranslate(Vector3(0,0,0));
	Vector3 vMin, vMax;
	GetVerticesMinMax(g_vertices2, vMin, vMax);
	characterBox.SetBox(vMin, vMax);

	//장애물 설정
	tagObstacle* pObstacle1 = new tagObstacle(Vector3(-100,0,1000));
	obstacleData.push_back(pObstacle1);
	tagObstacle* pObstacle2 = new tagObstacle(Vector3(0,0,1350));
	obstacleData.push_back(pObstacle2);
	tagObstacle* pObstacle3 = new tagObstacle(Vector3(100,0,1700));
	obstacleData.push_back(pObstacle3);

	/*g_matWorld3.SetTranslate(Vector3(-100,0,1000));
	obstacleQuantity.push_back(g_matWorld3);
	g_matWorld3.SetTranslate(Vector3(0,0,1350));
	obstacleQuantity.push_back(g_matWorld3);
	g_matWorld3.SetTranslate(Vector3(100,0,1700));
	obstacleQuantity.push_back(g_matWorld3);*/

	vMin = vMax = Vector3();
	GetVerticesMinMax(g_vertices3, vMin, vMax);
	obstacleBox.SetBox(vMin, vMax);

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

	//캐릭터 움직임
	Matrix44 characterMoving;
	characterMoving.SetTranslate(Vector3(0,0,10));
	g_matWorld2 *= characterMoving;
	characterBox.SetWorldTM(g_matWorld2);
	characterBox.Update();

	vector<tagObstacle*>::iterator it;
	for(it = obstacleData.begin(); it != obstacleData.end(); ++it)
	{
		obstacleBox.SetWorldTM( (*it)->position );
		obstacleBox.Update();

		if( (*it)->bCollision == true)
		{
			Matrix44 rotate;

			if( g_matWorld2.GetPosition().x <= (*it)->position.GetPosition().x )
			{
				rotate.SetRotationY(0.5f);
				(*it)->rotation *= rotate;
			}
			else if( g_matWorld2.GetPosition().x >= (*it)->position.GetPosition().x )
			{
				rotate.SetRotationY(-0.5f);
				(*it)->rotation *= rotate;
			}
		}
		else if( (*it)->bCollision == false)
			(*it)->bCollision = characterBox.Collision(obstacleBox);
	}
	
	//카메라 움직임
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

			// 법선벡터의 평균을 구해서 할당한다.
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
	const Matrix44 &tm, const Matrix44 &vpv, Matrix44& matWorld)
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
			else if(i == 30 && j == 1 && g_cameraPos.z > ground[2].z)
			{
				Matrix44 Test;
				Test.SetTranslate(Vector3(0,0,1400));
				
				matWorld = matWorld * Test;
			}
		}  //for
		loopCount += 350.f;
		mapLoop.SetTranslate(Vector3(0,0,loopCount));
	}  //for
}


bool RenderIndices(HDC hdc, const vector<Vector3> &vertices, const vector<int> &indices, 
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

		if(p3.z < g_cameraPos.z)
			return true;

		// culling
		Vector3 n = normals[ indices[ i]];
		n = n.MultiplyNormal(tm);
		if (n.DotProduct(camDir) > 0)
			continue;

		p1 = p1 * vpv;
		p2 = p2 * vpv;
		p3 = p3 * vpv;

		Rasterizer::Color c0(255,0,0,1);
//		Vector3 lightDir(0,-1,0);
		Rasterizer::DrawLine(hdc, c0, p1.x, p1.y, c0, p2.x, p2.y);
		Rasterizer::DrawLine(hdc, c0, p2.x, p2.y, c0, p3.x, p3.y);
		Rasterizer::DrawLine(hdc, c0, p3.x, p3.y, c0, p1.x, p1.y);
//		Rasterizer::Color color = c0 * max(0, n.DotProduct(-lightDir));
//		Rasterizer::DrawTriangle(hdc, color, p1.x, p1.y, color, p2.x, p2.y, color, p3.x, p3.y);
	}

	return false;
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

	//바닥 이어붙이기를 위함
	Matrix44 Test;
	Test.SetTranslate(Vector3(0,0,700));

	RenderVertices(hdcMem, g_vertices, g_indices, g_matWorld1, vpv, g_matWorld1);  //바닥1
	RenderVertices(hdcMem, g_vertices, g_indices, g_matWorld11 * Test, vpv, g_matWorld11);  //바닥2
	
	RenderIndices(hdcMem, g_vertices2, g_indices2, g_normals2, g_matLocal2 * g_matWorld2,  vpv);  //캐릭터
	
	vector<tagObstacle*>::iterator it;
	for(it = obstacleData.begin(); it!=obstacleData.end(); )
	{
		if( (*it)->bCollision)
		{
			LPCTSTR collisionCheck = _T("collide");
			::DrawText(hdcMem, collisionCheck, -1, &rc, DT_LEFT | DT_SINGLELINE);
		} //if

		if( RenderIndices(hdcMem, g_vertices3, g_indices3, g_normals3, (*it)->rotation * (*it)->position,  vpv) )
		{
			delete (*it);
			it = obstacleData.erase(it);

			//캐릭터 위치만큼 더한 위치에서 장애물 생성
			g_matWorld3.SetTranslate(Vector3(0,0,1000));
			g_matWorld3 *= g_matWorld2;
			
			tagObstacle* pObstacle = new tagObstacle(g_matWorld3);
			obstacleData.push_back(pObstacle);
		}
		else
			it++;
	} //for

	BitBlt(hdc, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
}

// vertices의 최대,최소 정점위치를 리턴한다.
void GetVerticesMinMax( const vector<Vector3> &vertices, OUT Vector3 &vMin, OUT Vector3 &vMax)
{
	vMax = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);
	vMin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	for (int i=0; i < (int)vertices.size(); ++i)
	{
		const Vector3 &v = vertices[i];

		if (vMax.x < v.x)
			vMax.x = v.x;
		if (vMax.y < v.y)
			vMax.y = v.y;
		if (vMax.z < v.z)
			vMax.z = v.z;

		if (vMin.x > v.x)
			vMin.x = v.x;
		if (vMin.y > v.y)
			vMin.y = v.y;
		if (vMin.z > v.z)
			vMin.z = v.z;
	}
}