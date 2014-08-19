#pragma once


namespace graphic
{

	class cTerrain
	{
	public:
		cTerrain();
		virtual ~cTerrain();

		bool CreateFromHeightMap( const string &heightMapFileName, 
			const string &textureFileName, const float heightFactor=3.f, const float textureUVFactor=1.f );
		bool CreateTerrainTexture( const string &textureFileName );
		bool CreateTerrain( const int rowCellCount=64, const int colCellCount=64, const float cellSize=50.f,
			const float textureUVFactor=1.f );

		float GetHeight(const float x, const float z);
		float GetHeightFromRay( const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out );
		bool Pick(const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out);
		cModel* PickModel(const Vector3 &orig, const Vector3 &dir);

		bool AddRigidModel(const cModel &model);
		cModel* FindRigidModel(const int id);
		bool RemoveRigidModel(cModel *model, const bool destruct=true);
		bool RemoveRigidModel(const int id, const bool destruct=true);
		vector<cModel*>& GetRigidModels();

		virtual void Render();
		virtual void RenderShader(cShader &shader);
	//�߰�
		void RenderShader(cShader &shader, cCamera* pCam);
		void RenderShadowRigidModels(cShader &shader, const Vector3& light, const Matrix44& proj);
		void RenderShaderRigidModels(cShader &shader);
		void SetDrawCube(const bool bCubeDraw);

		int GetRowCellCount() const;
		int GetColCellCount() const;
		float GetCellSize() const;
		float GetTerrainWidth() const;
		float GetTerrainHeight() const;
		const string& GetTextureName();
		float GetTextureUVFactor() const;
		float GetHeightFactor() const;

		virtual void Clear();


	protected:
		float GetHeightMapEntry( int row, int col );
		bool UpdateHeightMap( const string &heightMapFileName, 
			const string &textureFileName, const float heightFactor );
		void RenderRigidModels();
//		void RenderShaderRigidModels(cShader &shader);
	//�߰�
		void RenderShaderRigidModels(cShader &shader, cCamera* pCam);


	protected:
		int m_rowCellCount;
		int m_colCellCount;
		float m_cellSize;
		float m_heightFactor;
		float m_textureUVFactor;
		string m_heightMapFileName;
		cGrid2 m_grid;

		cShader *m_modelShader;	// reference
		vector<cModel*> m_rigids;
	};


	inline int cTerrain::GetRowCellCount() const { return m_rowCellCount; }
	inline int cTerrain::GetColCellCount() const { return m_colCellCount; }
	inline float cTerrain::GetCellSize() const { return m_cellSize; }
	inline float cTerrain::GetTerrainWidth() const { return m_colCellCount * m_cellSize; }
	inline float cTerrain::GetTerrainHeight() const { return m_rowCellCount * m_cellSize; }
	inline float cTerrain::GetTextureUVFactor() const { return m_textureUVFactor; }
	inline float cTerrain::GetHeightFactor() const { return m_heightFactor; }
	inline vector<cModel*>& cTerrain::GetRigidModels() { return m_rigids; }
}
