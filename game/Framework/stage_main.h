#pragma once

namespace framework
{
	class cStage_Main : public cStage  //ó�� ���� ��������
	{
	public:
		cStage_Main();
		~cStage_Main();

	//	virtual void Init() override;
	//	virtual void Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
		virtual void Init(const int nId) override;  //����ȭ�� �� ��ư �̹��� ���� �� ���
		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;  //����ȭ�� �ִϸ��̼�
		virtual void Release() override;
		
		bool MessageProc( UINT message, WPARAM wParam, LPARAM lParam);  //��ư Ŭ�� �ν�

	private:
		LPD3DXSPRITE m_sprite;
		cTestScene* m_scene;  //����ȭ�� ��ü
		cButton* m_button1;

		u_short usCount;  //����ȭ�� �ִϸ��̼��� ���� count
	};
}