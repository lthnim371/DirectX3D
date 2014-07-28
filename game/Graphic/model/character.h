#pragma once

namespace graphic
{
	class cCharacter : public cModel
	{
	public:
		enum STATE
		{
			NONE, ROTATION, FORWARD, BACKWARD, LEFTWARD, RIGHTWARD,
			LATTACK, RATTACK
		};
		enum MODE
		{
			NORMAL, MOVE, ATTACK,
		};

		cCharacter(const int id);
		virtual ~cCharacter();

		virtual bool Create(const string &modelName) override;
		void LoadWeapon(const string &fileName);
		virtual bool Move(const float elapseTime) override;
		virtual void Render() override;
		
	//Ãß°¡
		void Action(const int state, const float x = 0.f);

		void SetState(const int state);
		void SetRotation(const float x, const float y);

	protected:
		void Test();

	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode; // reference

	//test
		bool m_animode;
		Vector3 m_position;
		short m_state;
		short m_attackCnt;
		bool m_reserveL;
//		short m_mode;
	};

	inline void cCharacter::SetState(const int state) { m_state = state; }
}
