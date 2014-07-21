#pragma once


namespace graphic
{

	class cBoneNode : public cNode
								, public ICollisionable
	{
	public:
		cBoneNode(const int id, vector<Matrix44> &palette, const sRawBone &rawMesh);
		virtual ~cBoneNode();

		void SetAnimation( const sRawAni &rawAni, int nAniFrame, bool bLoop=false );
		const Matrix44& GetAccTM() const;
		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &parentTm) override;

		int GetCurrentFrame() const;
		int GetPlayFrame() const;
		void SetCurrentFrame(const int curFrame);

		// ICollisionable Interface
		virtual bool IsTest( int testNum ) override;
		virtual int GetCollisionId() override;
		virtual void UpdateCollisionBox() override;
		virtual cBoundingBox* GetCollisionBox() override;
		virtual void Collision( int testNum, ICollisionable *obj ) override;


		cCube m_boundingCube;
		cBoundingBox m_boundingBox;

	private:
		cTrack *m_track;
		cMesh *m_mesh;
		vector<Matrix44> &m_palette;
		Matrix44 m_accTM;	// ������ TM
		Matrix44 m_offset;	// inverse( m_matWorld )
		int m_aniStart; // ������ ���۽ð� (������)
		int m_aniEnd; // ������ ����ð� (������)
		int m_totalPlayTime; // �� ���ϸ��̼� �� ������

		int m_curPlayFrame; // ���� ���ϸ��̼� ������ (AniEnd�� ������ 0���� �ʱ�ȭ�ȴ�.)
		int m_incPlayFrame; // ���ϸ��̼� ���� �� ������
		float m_curPlayTime; // ���� �ִϸ��̼� �ð� (m_aniEnd �� �����ϸ� 0 �� �ȴ�.)
		float m_incPlayTime; // ���� �ִϸ��̼� �ð� (�� �ð�)

		bool m_isAni; // TRUE�ϰ�츸 ���ϸ��̼��� �ȴ�.
		bool m_isLoop; // ���ϸ��̼� �ݺ� ����
	};

	
	inline const Matrix44& cBoneNode::GetAccTM() const { return m_accTM; }
	inline int cBoneNode::GetCurrentFrame() const { return m_curPlayFrame; }
	inline int cBoneNode::GetPlayFrame() const { return m_incPlayFrame; }
}