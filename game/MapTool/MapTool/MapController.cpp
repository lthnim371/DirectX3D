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

	m_terrain.CreateFromHeightMap(fileName, "empty" );

	return true;
}


// ���� ��ü�� ���� ���� �ؽ��ĸ� �ε��Ѵ�.
bool cMapController::LoadHeightMapTexture(const string &fileName)
{

	return true;
}
