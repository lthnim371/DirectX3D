
#pragma once


#include "../Common/common.h"
#include "../Graphic/graphic.h"
#include "../Framework/framework.h"

using namespace common;

//Ãß°¡
#include "..\Network\Network\network.h"
#include "..\Network\Client\resource.h"
#include "afxcmn.h"
#include <sstream>
#include <string>
#include "afxwin.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define WM_CHATENTER	(WM_USER + 1)
