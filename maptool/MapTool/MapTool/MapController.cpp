#include "StdAfx.h"
#include "MapController.h"

using namespace graphic;

cMapController::cMapController(void) :
	m_editMode(EDIT_MODE::MODE_HEIGHTMAP)
//추가
	, m_currObject(NULL)
{
	m_camera.SetCamera(Vector3(100,100,-500), Vector3(0,0,0), Vector3(0,1,0));
	m_camera.SetProjection( D3DX_PI / 4.f, (float)VIEW_WIDTH / (float) VIEW_HEIGHT, 1.f, 10000.0f);

}


cMapController::~cMapController(void)
{
}

// 지형 파일 열기.
bool cMapController::LoadTerrainFile(const string &fileName)
{
	graphic::sRawTerrain rawTerrain;
	graphic::importer::ReadRawTerrainFile(fileName, rawTerrain);


	return true;
}

// 높이맵 파일을 읽어서 지형에 적용한다.
bool cMapController::LoadHeightMap(const string &fileName)
{
	m_heightMapFileName = fileName;
	const bool result = m_terrain.CreateFromHeightMap(fileName, "empty" );

	NotifyObserver();
	return result;
}


// 지형 전체를 덮어 씌울 텍스쳐를 로딩한다.
bool cMapController::LoadHeightMapTexture(const string &fileName)
{
	m_textFileName = fileName;
	const bool result = m_terrain.CreateTerrainTexture(fileName);

	NotifyObserver();
	return result;
}

// 지형 정보를 파일에 저장한다.
bool cMapController::SaveTerrainFile(const string &fileName)
{
	m_terrain.GetRigidModels() = m_object;

	graphic::sRawTerrain rawTerrain;
	m_terrain.GetRawTerrain(rawTerrain);
	graphic::exporter::WriteRawTerrainFile(fileName, rawTerrain);

	return true;
}

// 지형 생성.
bool cMapController::CreateDefaultTerrain()
{
	m_terrain.CreateTerrain(128, 128, 100.f, 8.f);
	m_terrain.CreateTerrainTexture( "../../media/texture/map/Grassbland01_T.tga");

	NotifyObserver();
	return true;
}


// 툴 편집 모드 설정.
void cMapController::ChangeEditMode(EDIT_MODE::TYPE mode)
{
	m_editMode = mode;
}


// 스플래팅 텍스쳐 모드에서, 마우스로 브러슁을 할 때 호출한다.
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


// 브러쉬 업데이트.
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


// HeightFactor 가 업데이트 될 때 호출 한다.
void cMapController::UpdateHeightFactor(const float heightFactor)
{
	m_terrain.SetHeightFactor(heightFactor);
}


// 지형위에 모델이 추가되거나 제거 될 때 호출한다.
void cMapController::UpdatePlaceModel()
{
	NotifyObserver( NOTIFY_TYPE::NOTIFY_ADD_PLACE_MODEL );
}

//추가
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