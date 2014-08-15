#include "StdAfx.h"
#include "MapController.h"

using namespace graphic;

cMapController::cMapController(void) :
	m_editMode(EDIT_MODE::MODE_HEIGHTMAP)
//�߰�
	, m_currObject(NULL)
{
	m_camera.SetCamera(Vector3(100,100,-500), Vector3(0,0,0), Vector3(0,1,0));
	m_camera.SetProjection( D3DX_PI / 4.f, (float)VIEW_WIDTH / (float) VIEW_HEIGHT, 1.f, 10000.0f);

}


cMapController::~cMapController(void)
{
}

// ���� ���� ����.
bool cMapController::LoadTerrainFile(const string &fileName)
{
	graphic::sRawTerrain rawTerrain;
	graphic::importer::ReadRawTerrainFile(fileName, rawTerrain);


	return true;
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

// ���� ������ ���Ͽ� �����Ѵ�.
bool cMapController::SaveTerrainFile(const string &fileName)
{
	m_terrain.GetRigidModels() = m_object;

	graphic::sRawTerrain rawTerrain;
	m_terrain.GetRawTerrain(rawTerrain);
	graphic::exporter::WriteRawTerrainFile(fileName, rawTerrain);

	return true;
}

// ���� ����.
bool cMapController::CreateDefaultTerrain()
{
	m_terrain.CreateTerrain(64, 64, 50.f, 8.f);
	m_terrain.CreateTerrainTexture( "../../media/terrain/��1.jpg");

	NotifyObserver();
	return true;
}


// �� ���� ��� ����.
void cMapController::ChangeEditMode(EDIT_MODE::TYPE mode)
{
	m_editMode = mode;
}


// ���÷��� �ؽ��� ��忡��, ���콺�� �귯���� �� �� ȣ���Ѵ�.
void cMapController::Brush(CPoint point)
{
	const Ray ray(point.x, point.y, VIEW_WIDTH, VIEW_HEIGHT, 
		m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix() );

	Vector3 pickPos;
	m_terrain.Pick( ray.orig, ray.dir, pickPos );
	m_cursor.UpdateCursor( m_terrain, pickPos );

	const int oldLayerCount = m_terrain.GetSplatLayerCount();
	m_terrain.Brush( m_cursor );

	if (m_terrain.GetSplatLayerCount() != oldLayerCount)
	{
		NotifyObserver(NOTIFY_TYPE::NOTIFY_ADD_LAYER);
	}
}


// �귯�� ������Ʈ.
void cMapController::UpdateBrush()
{
	CPoint pos(VIEW_WIDTH/2, VIEW_HEIGHT/2);
	Ray ray(pos.x, pos.y, VIEW_WIDTH, VIEW_HEIGHT, 
		m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix() );

	Vector3 pickPos;
	m_terrain.Pick( ray.orig, ray.dir, pickPos);
	m_cursor.UpdateCursor(m_terrain, pickPos );

	NotifyObserver( NOTIFY_TYPE::NOTIFY_CHANGE_CURSOR );
}


// HeightFactor �� ������Ʈ �� �� ȣ�� �Ѵ�.
void cMapController::UpdateHeightFactor(const float heightFactor)
{
	m_terrain.SetHeightFactor(heightFactor);
}


// �������� ���� �߰��ǰų� ���� �� �� ȣ���Ѵ�.
void cMapController::UpdatePlaceModel()
{
	NotifyObserver( NOTIFY_TYPE::NOTIFY_ADD_PLACE_MODEL );
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