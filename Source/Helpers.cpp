/**
  *  kTransfer3 - Helpers
  *
  *  @filesource
  *  @copyright    Copyright (c) 2006-2007 Ursus6
  *  @link         svn://konnekt.info/ktransfer3/ kTransfer3 plugin SVN Repo
  *  @version      $Revision$
  *  @modifiedby   $LastChangedBy: ursus6 $
  *  @lastmodified $Date: 2007-01-02 08:25:56 +0100 (Wt, 02 sty 2007) $
  */

#include "stdafx.h"
#include "Helpers.h"

namespace Helpers {
  String formatedString(const char *format, ...) {
	  va_list ap;
	  va_start(ap, format);

    char buff[6000] = {0};
    int size = VSNPRINTF(buff, sizeof(buff), format, ap);
    buff[sizeof(buff)-1]=0;

    va_end(ap);
    return buff;
  }
};

