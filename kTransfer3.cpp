// kTransfer2.cpp : Defines the entry point for the DLL application.
//
#ifdef _MANAGED
#pragma managed(push, off)
#endif

#include "stdafx.h"
#include <windows.h>
#include "Controller.h"

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  return TRUE;
}

int __stdcall IMessageProc(sIMessage_base * msgBase) {
  return Controller::getInstance()->process(msgBase);
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

