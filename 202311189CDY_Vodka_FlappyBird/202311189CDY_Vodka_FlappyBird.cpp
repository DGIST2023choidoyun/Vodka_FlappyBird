// 202311189CDY_Vodka_FlappyBird.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "202311189CDY_Vodka_FlappyBird.h"
#include "WindowClass.h"
#include "Singleton.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int size[2] = { 900, 540 };


    if (!WindowClass::Instance()->Initialize(hInstance, size))
        return FALSE;

    return WindowClass::Instance()->Run();
}