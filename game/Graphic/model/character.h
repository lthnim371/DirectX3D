#pragma once

namespace graphic
{
	class cCharacter : public cModel
	{
	public:
		enum STATE
		{
			NONE, NORMAL, ROTATION, LEFTROTATION, RIGHTROTATION,
			FORWARD, BACKWARD, LEFTWARD, RIGHTWARD,	DASH,
			JUMP, FRONTJUMP, BACKJUMP, LEFTJUMP, RIGHTJUMP,
			LATTACK, RATTACK, BEHIT,
		};

		cCharacter(const int id);
		virtual ~cCharacter();

		bool Create(const string &modelName);
		void LoadWeapon(const string &fileName);
		virtual bool Move(const float elapseTime) override;
		virtual void Render() override;
		virtual void RenderShader(cShader &shader);
		
//추가
	public:
		void Update(const short state, const float x = 0, const float y = 0);  //캐릭터 상태 변경
		bool GetCubeCheck() const;
		bool CollisionCheck( cCube& sourCube, const Vector3& sourPos, const Vector3& sourDir = Vector3() );
		int GetHP() const;
		cCube* GetWeaponCube() const;
		cCube* GetCharacterCube() const;
		void SetAttackSuccess();
		int GetMode() const;
		void UpdateBeHit(const bool bAniState, const Vector3& sourPos, const float fAniPosGap);
		float GetAniPosGap() const;
		cCamera* GetCamera();

	protected:
		bool UpdateAttack(const bool bAniState);
		void UpdateJump(const bool bAniState, const float elapseTime);
		void FindWeapon();  //현재 무기bone 이름과 같은 캐릭터의 무기bone 찾기
		void UpdateWeapon();  //캐릭터 무기 bone위치를 실시간 공유 받음
		void GetWeaponBoundingBox();

	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode; // reference

	//추가
//		short m_state;
		short m_attackCnt;  //공격 횟수(공격 상태 확인 가능)
		bool m_reserveL;  //마우스 왼클릭 예약
		bool m_reserveR;  //마우스 오른클릭 예약
		short m_mode;
//		Vector3 m_prevAniPos;
		float m_prevAniPos;  //애니 적용시 이동한 값 보관 (추후 vector로 바꾸기로 고려)
		float m_aniPosGap;
		vector<cBoneNode*> m_characterWeapon;  //캐릭터의 무기bone 주소 보관
//		u_short m_prevLastFrame;
//		u_short m_prevEndFrame;
		short m_jumpCnt;
		bool m_currJumpAttack;
		bool m_prevJumpAttack;
		float m_jumpSpeed;
		cCube* m_weaponCube;
		u_short m_cubeStartFrame;
		u_short m_cubeMaximumFrame;
		bool m_cubeCheck;
//		Vector3 m_cubePos;
//		short m_cubeKeepCnt;
		cCube* m_characterCube;
		short m_weaponCubeNumber;
		bool m_targetAttackCheck;
		int m_hp;
		int m_sp;

		cCamera* m_camera;
		ID3DXFont* m_font;
	};

	inline bool cCharacter::GetCubeCheck() const { return m_cubeCheck; }
	inline int cCharacter::GetHP() const { return m_hp; }
	inline cCube* cCharacter::GetWeaponCube() const { return m_weaponCube; }
	inline cCube* cCharacter::GetCharacterCube() const { return m_characterCube; }
	inline void cCharacter::SetAttackSuccess() { m_cubeCheck = false; m_targetAttackCheck = true; }
	inline int cCharacter::GetMode() const { return m_mode; }
	inline float cCharacter::GetAniPosGap() const { return m_aniPosGap; }
	inline cCamera* cCharacter::GetCamera() { return m_camera; }
}
