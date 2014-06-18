

#include "global.h"
#include "../Framework/framework.h"

class cGameApp : public framework::cGameMain
{
public:
	cGameApp();
	virtual ~cGameApp();
		
protected:

};

INIT_FRAMEWORK(cGameApp)

cGameApp::cGameApp()
{
	m_windowName = L"GameApp";
	const RECT r = {0, 0, 800, 600};
	m_windowRect = r;
}

cGameApp::~cGameApp()
{

}