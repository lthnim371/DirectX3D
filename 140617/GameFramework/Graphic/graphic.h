
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "graphic\DxInit.h"

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

static LPDIRECT3DDEVICE9 GetDevice() { return NULL; }  //static으로 한건 딱 하나만 사용하겠다는 뜻, 만약 안 붙이면 외부에서 호출할때마다 재정의되므로

struct Vertex {};