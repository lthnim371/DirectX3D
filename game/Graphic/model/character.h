#pragma once

namespace graphic
{
	class cCharacter : public cModel
	{
	public:
		enum STATE
		{
			NORMAL, ROTATION, FORWARD, BACKWARD, LEFTWARD, RIGHTWARD,
			DASH, JUMP, FRONTJUMP, BACKJUMP, LEFTJUMP, RIGHTJUMP,
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
		bool CollisionCheck( cCube& sourCube, Matrix44& sourTM );
		int GetHP() const;
		cCube* GetWeaponCube() const;
		cCube* GetCharacterCube() const;
		void SetAttackSuccess();

	protected:
		bool UpdateAttack(const bool bAniState);
		void UpdateJump(const bool bAniState);
		void UpdateBeHit(const bool bAniState);
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
		Vector3 m_cubePos;
//		short m_cubeKeepCnt;
		int m_hp;
		int m_sp;
		cCube* m_characterCube;
		short m_weaponCubeNumber;
		bool m_targetAttackCheck;
	//	cImage* m_img;
	};

	inline bool cCharacter::GetCubeCheck() const { return m_cubeCheck; }
	inline int cCharacter::GetHP() const { return m_hp; }
	inline cCube* cCharacter::GetWeaponCube() const { return m_weaponCube; }
	inline cCube* cCharacter::GetCharacterCube() const { return m_characterCube; }
	inline void cCharacter::SetAttackSuccess() { m_cubeCheck = false; m_targetAttackCheck = true; }
}
