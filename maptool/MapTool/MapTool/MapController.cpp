#include "StdAfx.h"
#include "MapController.h"


cMapController::cMapController(void) :
	m_editMode(EDIT_MODE::MODE_HEIGHTMAP)
//�߰�
	, m_currObject(NULL)
{
}


cMapController::~cMapController(void)
{
}


// ���̸� ������ �о ������ �����Ѵ�.
bool cMapController::LoadHeightMap(const string &fileName)
{
	m_heightMapFileName = fileName;
	const bool result = m_terrain.CreateFromHeightMap(fileName, "empty" );

	NotifyObserver();
	return result;
}


// ���� ��ü�� ���� ���� �ؽ��ĸ� �ε��Ѵ�.
bool cMapController::LoadHeightMapTexture(const string &fileName)
{
	m_textFileName = fileName;
	const bool result = m_terrain.CreateTerrainTexture(fileName);

	NotifyObserver();
	return result;
}


// ���� ����.
bool cMapController::CreateDefaultTerrain()
{
	m_terrain.CreateTerrain(64, 64, 50.f, 8.f);
	m_terrain.CreateTerrainTexture( "../media/terrain/grass014.jpg");

	NotifyObserver();
	return true;
}


// �� ���� ��� ����.
void cMapController::ChangeEditMode(EDIT_MODE::TYPE mode)
{
	m_editMode = mode;
}

//�߰�
void cMapController::SetSelectObject(const string& fileName)
{
	m_currObjFileName = fileName;

	if(m_currObject)
		SAFE_DELETE( m_currObject );
	m_currObject = new graphic::cModel(500);
	m_currObject->Create( fileName, graphic::MODEL_TYPE::RIGID );
}

void cMapController::AddObject(graphic::cModel* const pNewObj)
{
	m_object.push_back( pNewObj );
	m_objectFileName.push_back( m_currObjFileName );
}