/**
  *  kTransfer3
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  */

#include "stdafx.h"
#include "Controller.h"

using namespace kTransfer3;

int SmartWinMain(Application& app) {
	return 0;
}

HANDLE handle_mainwnd_thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainLoop, NULL, 0, NULL);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  if (ul_reason_for_call == DLL_PROCESS_DETACH) {
    TerminateThread(handle_mainwnd_thread, 0);
    delete main_wnd;
  }
  return true;
}

int __stdcall IMessageProc(sIMessage_base * msgBase) {
  return Controller::getInstance()->process(msgBase);
}