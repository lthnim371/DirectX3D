#include "StdAfx.h"
#include "MapController.h"


cMapController::cMapController(void)
{
}


cMapController::~cMapController(void)
{
}


// ���̸� ������ �о ������ �����Ѵ�.
bool cMapController::LoadHeightMap(const string &fileName)
{
	m_heightMapFileName = fileName;
	return m_terrain.CreateFromHeightMap(fileName, "empty" );
}


// ���� ��ü�� ���� ���� �ؽ��ĸ� �ε��Ѵ�.
bool cMapController::LoadHeightMapTexture(const string &fileName)
{
	m_textFileName = fileName;
	return m_terrain.CreateTerrainTexture(fileName);
}
