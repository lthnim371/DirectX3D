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
			LATTACK, RATTACK,
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
		bool UpdateAttack(const bool bAniState);
		void UpdateJump(const bool bAniState);
		void FindWeapon();  //���� ����bone �̸��� ���� ĳ������ ����bone ã��
		void UpdateWeapon();  //

//		void SetRotation(const float x, const POINT& ptMouse);

	protected:
		void Test();

	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode; // reference

	//�߰�
//		short m_state;
		short m_attackCnt;  //���� Ƚ��(���� ���� Ȯ�� ����)
		short m_jumpCnt;
		bool m_reserveL;  //���콺 ��Ŭ�� ����
		bool m_reserveR;  //���콺 ����Ŭ�� ����
		short m_mode;
//		Vector3 m_prevAniPos;
		float m_prevAniPos;  //�ִ� ����� �̵��� �� ���� (���� vector�� �ٲٱ�� ���)
		vector<cBoneNode*> m_characterWeapon;  //ĳ������ ����bone �ּ� ����
		u_short m_prevLastFrame;
		u_short m_prevEndFrame;
		bool m_jumpAttack;
	};
}
