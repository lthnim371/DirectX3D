#pragma once
//#include "..\basecontrol.h"

namespace graphic
{
	class cCharacter : public cModel
	{
	public:
		enum STATE
		{
			NONE, ROTATION, FORWARD, BACKWARD, LEFTWARD, RIGHTWARD, 
		};

		cCharacter(const int id);
		virtual ~cCharacter();

		virtual bool Create(const string &modelName) override;
		void LoadWeapon(const string &fileName);
		virtual bool Move(const float elapseTime) override;
		virtual void Render() override;
		
	//�߰�
		void Action(const int state, const float x = 0.f);

		void SetState(const int state);
		void SetRotation(const float x, const float y);

	protected:
		void Test();

	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode; // reference

		bool m_animode;
		Vector3 m_position;
		int m_state;
	};

	inline void cCharacter::SetState(const int state) { m_state = state; };
}
