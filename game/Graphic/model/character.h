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
		
	//�߰�
		void Update(const short state, const float x = 0.f);  //ĳ���� ���� ����
		

		void SetRotation(const float x, const float y);

	protected:
		void Test();

	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode; // reference

	//�߰�
		bool m_animode;
//		short m_state;
		short m_attackCnt;  //���� Ƚ��(���� ���� Ȯ�� ����)
		bool m_reserveL;  //���콺 ��Ŭ�� ����
//		short m_mode;
//		Vector3 m_prevAniPos;
		float m_prevAniPos;
	};
}
