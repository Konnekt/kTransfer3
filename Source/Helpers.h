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
  String formatedString(const char *format, ...);

  inline int pluginExists(int net, int type) {
    return Ctrl->ICMessage(IMC_FINDPLUG, net, type);
  }
};

#endif /*__HELPERS_H__*/