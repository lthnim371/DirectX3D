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
		virtual void RenderShader(cShader &shader);
		
	//�߰�
		void Update(const short state, const float x = 0, const float y = 0);  //ĳ���� ���� ����
		bool Attack(bool bAniState);

//		void SetRotation(const float x, const POINT& ptMouse);

	protected:
		void Test();

	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode; // reference

	//�߰�
//		bool m_animode;
//		short m_state;
		short m_attackCnt;  //���� Ƚ��(���� ���� Ȯ�� ����)
	//	short m_countL;  //���� Ƚ��(���� ���� Ȯ�� ����)
	//	short m_countR;
		bool m_reserveL;  //���콺 ��Ŭ�� ����
		bool m_reserveR;  //���콺 ����Ŭ�� ����
		short m_mode;
//		Vector3 m_prevAniPos;
		float m_prevAniPos;  //�ִ� ����� �̵��� �� ����
	};
}
