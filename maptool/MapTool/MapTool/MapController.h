#pragma once


class cMapController  : public common::cObservable
									,public common::cSingleton<cMapController>
{
public:
	cMapController(void);
	~cMapController(void);

	bool LoadHeightMap(const string &fileName);
	bool LoadHeightMapTexture(const string &fileName);
	bool CreateDefaultTerrain();

	graphic::cTerrainEditor& GetTerrain();
	graphic::cTerrainCursor& GetTerrainCursor();
	const string& GetHeightMapFileName();
	const string& GetTextureFileName();

	void ChangeEditMode(EDIT_MODE::TYPE mode);
	EDIT_MODE::TYPE GetEditMode() const;

//�߰�
	void SetSelectObject(const string& fileName);
	graphic::cModel* const GetCurrObject();
	const string& GetCurrObjFileName();
	void DeleteCurrObject();
	void AddObject(graphic::cModel* const pNewObj);
	vector<graphic::cModel*>& GetObject();

private:
	graphic::cTerrainEditor m_terrain;
	graphic::cTerrainCursor m_cursor;
	string m_heightMapFileName;
	string m_textFileName;
	EDIT_MODE::TYPE m_editMode;
//�߰�
	graphic::cModel* m_currObject;
	vector<graphic::cModel*> m_object;
	string m_currObjFileName;
	vector<string> m_objectFileName;
};


inline graphic::cTerrainEditor& cMapController::GetTerrain() { return m_terrain; }
inline graphic::cTerrainCursor& cMapController::GetTerrainCursor() { return m_cursor; }
inline const string& cMapController::GetHeightMapFileName() { return m_heightMapFileName; }
inline const string& cMapController::GetTextureFileName() { return m_textFileName; }
inline EDIT_MODE::TYPE cMapController::GetEditMode() const { return m_editMode; }

//�߰�
inline graphic::cModel* const cMapController::GetCurrObject() { return m_currObject; }
inline const string& cMapController::GetCurrObjFileName() { return m_currObjFileName; }
inline void cMapController::DeleteCurrObject() { if(m_currObject) SAFE_DELETE( m_currObject ); }
inline vector<graphic::cModel*>& cMapController::GetObject() { return m_object; }
