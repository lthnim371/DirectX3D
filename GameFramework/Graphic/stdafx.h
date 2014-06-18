//stdafx.h : 미리 컴파일된 헤더(프리 컴파일 헤더)
//미리 컴파일된 헤더를 사용하게 되면 cpp파일에 자동으로 맨 위에 입력되어 있다.
//그런데 인클루드 순서상 꼭 맨 위에여야 하는 이유는 먼저 인클루드 된 헤더파일에 있는 것들을(함수 등)을 밑에서 인클루드하는 헤더에서 사용할 수 있음으로
//그래서 모든 곳에서 사용되어지는 디파인이나 인클루드 같은 것을 stdafx.h에 입력하고 stdafx.h 인클루드는 항상 제일 첫번째여야 한다
//소속된 프로젝트 내에서 전역적으로 사용되는것들일 때


// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#include "graphic.h"
#include <vector>