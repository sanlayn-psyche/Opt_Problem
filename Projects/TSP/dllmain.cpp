// dllmain.cpp : 定义 DLL 应用程序的入口点。
#pragma once

#include "TSPSolve.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

EXTERN_C __declspec(dllexport) bool tsp_solve(void *root, bool usercut);


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


bool tsp_solve(void *root, bool usercut)
{

    TSPSolve* tsp = &TSPSolve::Instance();
    return tsp->m_solve(*(std::string*)root, usercut);
}
