#pragma once


class cMapController  : public common::cObservable2
									,public common::cSingleton<cMapController>
{
public:
	cMapController(void);
	~cMapController(void);

	bool LoadTerrainFile(const string &fileName);
	bool LoadHeightMap(const string &fileName);
	bool LoadHeightMapTexture(const string &fileName);
	bool SaveTerrainFile(const string &fileName);
	bool CreateDefaultTerrain();

	graphic::cCamera& GetCamera();
	graphic::cTerrainEditor& GetTerrain();
	graphic::cTerrainCursor& GetTerrainCursor();
	const string& GetHeightMapFileName();
	const string& GetTextureFileName();

	void Brush(CPoint point);
	void UpdateBrush();
	void UpdateHeightFactor(const float heightFactor);
	void UpdatePlaceModel();

	void ChangeEditMode(EDIT_MODE::TYPE mode);
	EDIT_MODE::TYPE GetEditMode() const;

//추가
	void SetSelectObject(const string& fileName);
	graphic::cModel* const GetCurrObject();
	const string& GetCurrObjFileName();
	void DeleteCurrObject();
	void AddObject(graphic::cModel* const pNewObj);
	vector<graphic::cModel*>& GetObject();

private:
	graphic::cCamera m_camera;
	graphic::cTerrainEditor m_terrain;
	graphic::cTerrainCursor m_cursor;
	string m_heightMapFileName;
	string m_textFileName;
	EDIT_MODE::TYPE m_editMode;

//추가
	graphic::cModel* m_currObject;
	vector<graphic::cModel*> m_object;
	string m_currObjFileName;
	vector<string> m_objectFileName;
};


inline graphic::cCamera& cMapController::GetCamera() { return m_camera; }
inline graphic::cTerrainEditor& cMapController::GetTerrain() { return m_terrain; }
inline graphic::cTerrainCursor& cMapController::GetTerrainCursor() { return m_cursor; }
inline const string& cMapController::GetHeightMapFileName() { return m_heightMapFileName; }
inline const string& cMapController::GetTextureFileName() { return m_textFileName; }
inline EDIT_MODE::TYPE cMapController::GetEditMode() const { return m_editMode; }
//추가
inline graphic::cModel* const cMapController::GetCurrObject() { return m_currObject; }
inline const string& cMapController::GetCurrObjFileName() { return m_currObjFileName; }
inline void cMapController::DeleteCurrObject() { if(m_currObject) SAFE_DELETE( m_currObject ); }
inline vector<graphic::cModel*>& cMapController::GetObject() { return m_object; }