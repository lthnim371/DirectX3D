#pragma once

namespace graphic
{
	class cCharacter : public cModel
	{
	public:
		enum STATE
		{
			NONE, NORMAL, ROTATION, LEFTROTATION, RIGHTROTATION,
			FORWARD, BACKWARD, LEFTWARD, RIGHTWARD,
			DASH, GUARD, GUARD_BE_HIT,
			JUMP, FRONTJUMP, BACKJUMP, LEFTJUMP, RIGHTJUMP,
			LATTACK, RATTACK, BEHIT,
		};

		cCharacter(const int id);
		virtual ~cCharacter();

		bool Create(const string &modelName);
		void LoadWeapon(const string &fileName);//, const string &fileName2);
		virtual bool Move(const float elapseTime) override;
		virtual void Render() override;
		virtual void RenderShader(cShader &shader) override;
		virtual void RenderShadow(cShader &shader) override;
		
//�߰�
	public:
		void Update(const float elapseTime, const short state, const float x = 0, const float y = 0);  //ĳ���� ���� ����
		bool GetCubeCheck() const;
		bool CollisionCheck1( cCube& sourCube, const Vector3& sourPos, const Vector3& sourDir = Vector3() );
		bool CollisionCheck2( cCube& sourCube, const Vector3& sourPos, const Vector3& sourDir = Vector3() );
		float GetHP() const;
		float GetSP() const;
		cCube* GetWeaponCube() const;
		cCube* GetCharacterCube() const;
		void SetAttackSuccess();
		bool GetAttackSuccess() const;
		int GetMode() const;
		void UpdateBeHit(const bool bAniState, const Vector3& sourPos, const float fAniPosGap);
		float GetAniPosGap() const;
		cCamera* GetCamera();
		void MoveControl(const bool bCtl, const bool bOnlyJump = false);  //�ִϸ��̼� �̵��� ���� ���� ����

	//debug
		void SetMode( const int stage );
		void SetHp( const float hp );
		void SetDrawCube(const bool bCubeDraw);

	protected:
		bool UpdateAttack(const bool bAniState);
		void UpdateJump(const bool bAniState, const float elapseTime);
		void FindWeapon();  //���� ����bone �̸��� ���� ĳ������ ����bone ã��
		void UpdateWeapon();  //ĳ���� ���� bone��ġ�� �ǽð� ���� ����
		void GetWeaponBoundingBox();

	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode; // reference
	
	//�߰�
//		short m_state;
		short m_attackCnt;  //���� Ƚ��(���� ���� Ȯ�� ����)
		bool m_reserveL;  //���콺 ��Ŭ�� ����
		bool m_reserveR;  //���콺 ����Ŭ�� ����
		short m_mode;
//		Vector3 m_prevAniPos;
		float m_prevAniPos;  //�ִ� ����� �̵��� �� ���� (���� vector�� �ٲٱ�� ���)
		float m_aniPosGap;
		vector<cBoneNode*> m_characterWeapon;  //ĳ������ ����bone �ּ� ����
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
		float m_hp;
		float m_sp;
		bool m_moveControl;
		float m_tick;

		cCamera* m_camera;
//		ID3DXFont* m_font;
	};

	inline bool cCharacter::GetCubeCheck() const { return m_cubeCheck; }
	inline float cCharacter::GetHP() const { return m_hp; }
	inline float cCharacter::GetSP() const { return m_sp; }
	inline cCube* cCharacter::GetWeaponCube() const { return m_weaponCube; }
	inline cCube* cCharacter::GetCharacterCube() const { return m_characterCube; }
	inline void cCharacter::SetAttackSuccess() { m_cubeCheck = false; m_targetAttackCheck = true; }
	inline bool cCharacter::GetAttackSuccess() const { return m_targetAttackCheck; }
	inline int cCharacter::GetMode() const { return m_mode; }
	inline float cCharacter::GetAniPosGap() const { return m_aniPosGap; }
	inline cCamera* cCharacter::GetCamera() { return m_camera; }
//	inline void cCharacter::MoveControl(const bool bCtl) { m_moveControl = bCtl; m_bone->MoveControl(bCtl); }

//debug
	inline void cCharacter::SetMode( const int stage ) { m_mode = stage; }
	inline void cCharacter::SetHp( const float hp ) { m_hp = hp; }
}
