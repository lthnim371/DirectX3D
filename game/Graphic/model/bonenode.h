#pragma once


namespace graphic
{

	class cBoneNode : public cNode
	{
	public:
		cBoneNode(const int id, vector<Matrix44> &palette, const sRawBone &rawMesh);
		virtual ~cBoneNode();

		void SetAnimation( const sRawAni &rawAni, int nAniFrame, bool bLoop=false );
		const Matrix44& GetAccTM() const;
		const Matrix44& GetOffset() const;
		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &parentTm) override;
		virtual void RenderShader(const Matrix44 &parentTm) override;
		virtual void RenderShader(cShader &shader, const Matrix44 &parentTm) override;

		int GetCurrentFrame() const;
		int GetPlayFrame() const;
		void SetCurrentFrame(const int curFrame);
		void UpdateAccTM();

	//추가
		bool GetAniState() const;
		Vector3 GetAniTM() const;
		void SetAccTM(const Matrix44& accTM);
		int GetEndFrame() const;
		void SetMoveControl(const bool bCtl);
		bool GetMoveControl() const;

	private:
		cTrack *m_track;
		cMesh *m_mesh;
		vector<Matrix44> &m_palette;
		Matrix44 m_accTM;	// 누적된 TM
		Matrix44 m_offset;	// inverse( m_matWorld )
		int m_aniStart; // 프래임 시작시간 (프레임)
		int m_aniEnd; // 프래임 종료시간 (프레임)
		int m_totalPlayTime; // 총 에니메이션 될 프레임

		int m_curPlayFrame; // 현재 에니메이션 프래임 (AniEnd를 지나면 0으로 초기화된다.)
		int m_incPlayFrame; // 에니메이션 중인 총 프레임
		float m_curPlayTime; // 현재 애니메이션 시간 (m_aniEnd 에 도달하면 0 이 된다.)
		float m_incPlayTime; // 현재 애니메이션 시간 (총 시간)

		bool m_isAni; // TRUE일경우만 에니메이션이 된다.
		bool m_isLoop; // 에니메이션 반복 여부

	//추가
		bool m_moveControl;
	};

	
	inline const Matrix44& cBoneNode::GetAccTM() const { return m_accTM; }
	inline const Matrix44& cBoneNode::GetOffset() const { return m_offset; }
	inline int cBoneNode::GetCurrentFrame() const { return m_curPlayFrame; }
	inline int cBoneNode::GetPlayFrame() const { return m_incPlayFrame; }

//추가
	inline bool cBoneNode::GetAniState() const { return m_isAni; }
	inline Vector3 cBoneNode::GetAniTM() const { return m_aniTM.GetPosition(); }
	inline void cBoneNode::SetAccTM(const Matrix44& accTM) { m_accTM = accTM; }
	inline int cBoneNode::GetEndFrame() const { return m_aniEnd; }
	inline void cBoneNode::SetMoveControl(const bool bCtl) { m_moveControl = bCtl; }
	inline bool cBoneNode::GetMoveControl() const { return m_moveControl; }
}
