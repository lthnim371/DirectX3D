#pragma once


namespace graphic
{
	class cBoneMgr;
	class cBoundingBox;

	DECLARE_TYPE_NAME_SCOPE(graphic, cModel)
	class cModel : public ICollisionable
						, public memmonitor::Monitor<cModel, TYPE_NAME(cModel)>
	{
	public:
		cModel(const int id);
		virtual ~cModel();

		virtual bool Create(const string &modelName);
		void SetAnimation( const string &aniFileName);
		virtual bool Move(const float elapseTime);
		virtual void Render();
		void Clear();

		int GetId() const;
		void SetTM(const Matrix44 &tm);
		void MultiplyTM(const Matrix44 &tm);
		const Matrix44& GetTM() const;
		cBoneMgr* GetBoneMgr();
		cMesh* FindMesh(const string &meshName);

		// ICollisionable Interface
		virtual bool IsTest( int testNum ) override;
		virtual int GetCollisionId() override;
		virtual void UpdateCollisionBox() override;
		virtual cBoundingBox* GetCollisionBox() override;
		virtual void Collision( int testNum, ICollisionable *obj ) override;

		// debug �� �Լ�.
		void SetRenderMesh(const bool isRenderMesh);
		void SetRenderBone(const bool isRenderBone);
		void SetRenderBoundingBox(const bool isRenderBoundingBox);


	protected:
		int m_id;
		MODEL_TYPE::TYPE m_type;
		vector<cMesh*> m_meshes;
		cBoneMgr *m_bone;
		Matrix44 m_matTM;
		cBoundingBox m_boundingBox; // only rigid mesh model

		// debug ��.
		bool m_isRenderMesh; // default = true
		bool m_isRenderBone; // default = false
		bool m_isRenderBoundingBox; // default = true
	};


	inline int cModel::GetId() const { return m_id; }
	inline void cModel::SetTM(const Matrix44 &tm) { m_matTM = tm; }
	inline void cModel::MultiplyTM(const Matrix44 &tm) { m_matTM *= tm; }
	inline const Matrix44& cModel::GetTM() const { return m_matTM; }
	inline cBoneMgr* cModel::GetBoneMgr() { return m_bone; }
}
