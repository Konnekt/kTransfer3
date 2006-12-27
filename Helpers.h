#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "stdafx.h"

Stamina::String formatedString(const char *format, ...) {
	va_list ap;
	va_start(ap, format);

  char buff[6000] = {0};
  int size = VSNPRINTF(buff, sizeof(buff), format, ap);
  buff[sizeof(buff)-1]=0;

  va_end(ap);
  return buff;
}

#endif /*__HELPERS_H__*/