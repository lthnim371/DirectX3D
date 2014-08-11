//game application

#pragma once


class cGameApp : public framework::cGameMain
{
public:
	cGameApp();
	virtual ~cGameApp();

	virtual bool OnInit() override;
	virtual void OnUpdate(const float elapseT) override;
	virtual void OnRender(const float elapseT) override;
	virtual void OnShutdown() override;
	virtual void MessageProc( UINT message, WPARAM wParam, LPARAM lParam) override;

//�߰�
	virtual void OnInput(const float elapseT) override;  //�Է�Ű�� �޴� loop

private:
	graphic::cVertexBuffer m_vtxBuff;
	graphic::cIndexBuffer m_idxBuff;
	int m_VtxSize;
	int m_FaceSize;
	graphic::cMaterial m_mtrl;
	graphic::cLight m_light;
/*
	POINT m_currMouse;
	POINT m_prevMouse;
	bool m_bMouse;
*/
//	graphic::cCharacter* character1;
//	graphic::cCharacter* character2;

//	framework::cStage* m_stage;
};
