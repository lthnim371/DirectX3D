#pragma once

namespace framework
{
	class cTestScene : public cWindow
	{
	public:
		cTestScene(LPD3DXSPRITE sprite, const int nId = 0, const string& sName = "testScene");
		virtual ~cTestScene();

	//main
		void Button1Click(framework::cEvent &event);
	
	//select
		void Button2Click(framework::cEvent &event);
		void Button3Click(framework::cEvent &event);

	private:
	};
}