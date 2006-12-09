#include "stdafx.h"
#include"Form.h"

using namespace easyWin;

void Form::create(const char *name, int style, int exStyle, int x, int y, int w, int h, HWND parent) {
  assert(_handle == NULL);
  _handle = CreateWindowExA(exStyle, "easyWin::Form", name, style, x, y, w, h, parent, NULL, instance, this);
}