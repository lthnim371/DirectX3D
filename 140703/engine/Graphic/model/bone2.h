#pragma once


namespace graphic
{
	class cBoneNode;

	class cBoneMgr
	{
	public:
		cBoneMgr(const int id, const sRawMeshGroup &rawMeshes);
		virtual ~cBoneMgr();

		void SetAnimation( const sRawAniGroup &rawAnies, int nAniFrame );
		bool Move(const float elapseTime);
		void Render(const Matrix44 &parentTm);
		void Clear();	
		vector<Matrix44>& GetPalette();

		cBoneNode* FindBone(const int id);


	protected:
		void SetAnimationRec( cBoneNode *node, const sRawAniGroup &rawAni, int nAniFrame );


	private:
		cBoneNode *m_root;
		int m_id;
		vector<Matrix44> m_palette;
	};


	inline vector<Matrix44>& cBoneMgr::GetPalette() { return m_palette; }
}
