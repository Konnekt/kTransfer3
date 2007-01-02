/**
  *  kTransfer3 - Helpers
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy$
  *  @lastmodified $Date$
  */
#pragma once

#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "stdafx.h"

namespace Helpers {
  Stamina::String formatedString(const char *format, ...) {
	  va_list ap;
	  va_start(ap, format);

    char buff[6000] = {0};
    int size = VSNPRINTF(buff, sizeof(buff), format, ap);
    buff[sizeof(buff)-1]=0;

    va_end(ap);
    return buff;
  }

  int pluginExists(int net, int type) {
    return Ctrl->ICMessage(IMC_FINDPLUG, net, type);
  }
};

#endif /*__HELPERS_H__*/