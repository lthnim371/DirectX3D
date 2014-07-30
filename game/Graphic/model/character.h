#pragma once

namespace graphic
{
	class cCharacter : public cModel
	{
	public:
		enum STATE
		{
			NORMAL, ROTATION, FORWARD, BACKWARD, LEFTWARD, RIGHTWARD,
			DASH, LATTACK, RATTACK
		};

		cCharacter(const int id);
		virtual ~cCharacter();

		virtual bool Create(const string &modelName) override;
		void LoadWeapon(const string &fileName);
		virtual bool Move(const float elapseTime) override;
		virtual void Render() override;
		
	//추가
		void Update(const short state, const float x = 0, const float y = 0);  //캐릭터 상태 변경
		bool Attack(bool bAniState);

//		void SetRotation(const float x, const POINT& ptMouse);

	protected:
		void Test();

	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode; // reference

	//추가
//		bool m_animode;
//		short m_state;
		short m_attackCnt;  //공격 횟수(공격 상태 확인 가능)
	//	short m_countL;  //공격 횟수(공격 상태 확인 가능)
	//	short m_countR;
		bool m_reserveL;  //마우스 왼클릭 예약
		bool m_reserveR;  //마우스 오른클릭 예약
		short m_mode;
//		Vector3 m_prevAniPos;
		float m_prevAniPos;  //애니 적용시 이동한 값 보관
	};
}
