// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once
#pragma comment(lib, "gdiplus")
#pragma comment (lib, "winmm.lib")    //음악

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#include <Ole2.h>
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <gdiplus.h>
#include <mmsystem.h>
#include <iostream>
#include "Singleton.h"
using namespace Gdiplus;
using namespace std;

#define RAD2DEG			180/PI
#define PI				3.1415926535

#define MAX_LOADSTRING 100
#define IDI_MY202311189CDYVODKAFLAPPYBIRD
#define IDC_MY202311189CDYVODKAFLAPPYBIRD