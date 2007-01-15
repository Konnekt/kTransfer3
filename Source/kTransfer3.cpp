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

DWORD self;
HANDLE Controller::mainwnd_thread = CreateThread(0, 10000, (LPTHREAD_START_ROUTINE)MainLoop, NULL, 0, &self);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  return true;
}

int __stdcall IMessageProc(sIMessage_base * msgBase) {
  return Controller::getInstance()->process(msgBase);
}