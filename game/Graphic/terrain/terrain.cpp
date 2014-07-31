
#include "stdafx.h"
#include "terrain.h"
#include <objidl.h>
#include <gdiplus.h> 
#pragma comment( lib, "gdiplus.lib" ) 
using namespace std;
using namespace Gdiplus;
using namespace graphic;

// global constants
enum {
	CELL_COL_COUNT = 64,
	CELL_ROW_COUNT = 64,
	VERTEX_COL_COUNT = CELL_COL_COUNT+1,
	VERTEX_ROW_COUNT = CELL_ROW_COUNT+1,
	CELL_SIZE = 50,
	WIDTH = CELL_COL_COUNT * CELL_SIZE,
	HEIGHT = CELL_ROW_COUNT * CELL_SIZE,
};


ULONG_PTR g_gdiplusToken;
GdiplusStartupInput g_gdiplusStartupInput; 

cTerrain::cTerrain()
{
	static bool onlyOne = false;
	if (!onlyOne)
	{	
		Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL); 
		onlyOne = true;
	}
}

cTerrain::~cTerrain()
{
	static bool onlyOne = false;
	if (!onlyOne)
	{
		Gdiplus::GdiplusShutdown(g_gdiplusToken);
		onlyOne = true;
	}
}


bool cTerrain::CreateFromHeightMap( const string &heightMapFileName, 
	const string &textureFileName, const float heightFactor ) // heightFactor=3.f
{
	const wstring wfileName = common::str2wstr(heightMapFileName);
	Bitmap bmp(wfileName.c_str());
	
	m_grid.Create(CELL_ROW_COUNT, CELL_COL_COUNT, CELL_SIZE, 1.f);

	const float incX = (float)(bmp.GetWidth()-1) / (float)CELL_COL_COUNT;
	const float incY = (float)(bmp.GetHeight()-1) /(float) CELL_ROW_COUNT;

	sVertexNormTex *pv = (sVertexNormTex*)m_grid.GetVertexBuffer().Lock();

	for (int i=0; i < VERTEX_COL_COUNT; ++i)
	{
		for (int k=0; k < VERTEX_ROW_COUNT; ++k)
		{
			sVertexNormTex *vtx = pv + (k*VERTEX_COL_COUNT) + i;

			Color color;
			bmp.GetPixel(i*incX, k*incY, &color);
			const float h = ((color.GetR() + color.GetG() + color.GetB()) / 3.f) 
				* heightFactor;
			vtx->p.y = h;
		}
	}

	m_grid.GetVertexBuffer().Unlock();

	m_grid.CalculateNormals();
	m_grid.GetTexture().Create( textureFileName );
	return true;
}


void cTerrain::Render()
{
	m_grid.Render();
}


void cTerrain::RenderShader(cShader &shader)
{
	Vector3 fog(1.f, 10000.f, 0);  // near, far
	shader.SetVector( "vFog", fog);

	m_grid.RenderShader(shader);
}


float Lerp(float p1, float p2, float alpha)
{
	return p1 * (1.f - alpha) + p2 * alpha;
}


// x/z��鿡�� ���� ��ǥ x,z ��ġ�� �ش��ϴ� ���� �� y�� �����Ѵ�.
float cTerrain::GetHeight(const float x, const float z)
{
	float newX = x + (WIDTH / 2.0f);
	float newZ = HEIGHT - (z + (HEIGHT / 2.0f));

	newX /= CELL_SIZE;
	newZ /= CELL_SIZE;

	const float col = ::floorf( newX );
	const float row = ::floorf( newZ );

	//  A   B
	//  *---*
	//  | / |
	//  *---*
	//  C   D
	const float A = GetHeightMapEntry( (int)row, (int)col );
	const float B = GetHeightMapEntry( (int)row, (int)col+1 );
	const float C = GetHeightMapEntry( (int)row+1, (int)col );
	const float D = GetHeightMapEntry( (int)row+1, (int)col+1 );

	const float dx = newX - col;
	const float dz = newZ - row;

	float height = 0.0f;
	if( dz < 1.0f - dx )  // upper triangle ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C
		height = A + Lerp(0.0f, uy, dx) + Lerp(0.0f, vy, dz);
	}
	else // lower triangle DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
		height = D + Lerp(0.0f, uy, 1.0f - dx) + Lerp(0.0f, vy, 1.0f - dz);
	}

	return height;
}


// ���� 2���� �迭�� ���� ��, row, col �ε����� ���� ���� �����Ѵ�.
float cTerrain::GetHeightMapEntry( int row, int col )
{
	const int vtxSize = (VERTEX_COL_COUNT) * (VERTEX_ROW_COUNT);

	if( 0 > row || 0 > col )
		return 0.f;
	if( vtxSize <= (row * VERTEX_ROW_COUNT + col) ) 
		return 0.f;

	sVertexNormTex *pv = (sVertexNormTex*)m_grid.GetVertexBuffer().Lock();
	const float h = pv[ row * VERTEX_ROW_COUNT + col].p.y;
	m_grid.GetVertexBuffer().Unlock();
	return h;
}


// ���� ���� orig, dir �� �̿��ؼ�, �浹�� ���� y ��ǥ�� �����Ѵ�.
// ��ŷ ��ġ�� out�� �����ؼ� �����Ѵ�.
float cTerrain::GetHeightFromRay( const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out )
{
	if (m_grid.Pick(orig, dir, out))
	{
		return GetHeight(out.x, out.z);
	}
	return 0.f;
}


// ��ŷ ó��.
bool cTerrain::Pick(const int x, const int y, const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out)
{
	return m_grid.Pick(orig, dir, out);
}
