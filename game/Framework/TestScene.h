#pragma once

namespace framework
{
	class cTestScene : public cWindow
	{
	public:
		cTestScene(LPD3DXSPRITE sprite, const int nId = 0, const string& sName = "testScene");
		virtual ~cTestScene();

	//main
		void Button_main_1Click(framework::cEvent &event);
	
	//character select
		void Button_CS_1Click(framework::cEvent &event);

	//network select
		void Button_NS_1Click(framework::cEvent &event);
		void Button_NS_2Click(framework::cEvent &event);

	private:
	//	bool m_clickLimit;
	};
}