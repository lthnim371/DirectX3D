
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "graphic\DxInit.h"

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

static LPDIRECT3DDEVICE9 GetDevice() { return NULL; }  //static���� �Ѱ� �� �ϳ��� ����ϰڴٴ� ��, ���� �� ���̸� �ܺο��� ȣ���Ҷ����� �����ǵǹǷ�