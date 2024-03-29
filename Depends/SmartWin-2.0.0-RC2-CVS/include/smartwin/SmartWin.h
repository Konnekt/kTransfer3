// $Revision: 1.15 $
/*
  Copyright (c) 2005, Thomas Hansen
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

	  * Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.
	  * Redistributions in binary form must reproduce the above copyright notice, 
		this list of conditions and the following disclaimer in the documentation 
		and/or other materials provided with the distribution.
	  * Neither the name of the SmartWin++ nor the names of its contributors 
		may be used to endorse or promote products derived from this software 
		without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef SmartWin_h
#define SmartWin_h

#ifdef _MSC_VER
// We don't want the stupid "pointer trunctation" to 64 bit architecture warning.
// The warnings aren't justified anyway since they are basically a bug in 7.1
// release... E.g. the SetWindowLongPtr is defined as SetWindowLong in 32 bits mode
// but will in 64 bits mode be defined as the 64 bits equivalent version, therefore
// it will give you a 64 bit compile warning when this file is compiled with
// warning level 4 (MSVC)
#pragma warning( disable : 4244 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4311 )

// We also need to tell the compiler that it needs to link pointer to members as
// virtual multiple inheritance pointers. We generally want as little as possible
// of Project Settings therefore we set this directly in the code instead of
// forcing the library user to set lots of different settings before managing to
// compile his project.
#pragma pointers_to_members( full_generality, virtual_inheritance )
#endif

#include "WindowsHeaders.h"
#include "boost.h"
#include "SmartUtil.h"
#include "Application.h"
#include "WidgetFactory.h"
#include "widgets/WidgetWindowBase.h"
#include "widgets/WidgetWindow.h"
#include "widgets/WidgetDialog.h"
#include "widgets/WidgetModalDialog.h"
#include "widgets/WidgetMDIChild.h"
#include "BasicTypes.h"
#include "Anchors.h"
#include "Place.h"
#include "CallbackFuncPrototypes.h"
#include "Command.h"
#include "widgets/WidgetMessageBox.h"
#include "widgets/WidgetMDIParent.h"
#include "widgets/WidgetGroupBox.h"
#include "widgets/WidgetDateTimePicker.h"
#include "widgets/WidgetRadioButton.h"
#include "widgets/WidgetStatusBar.h"
#include "widgets/WidgetTextBox.h"
#include "widgets/WidgetButton.h"
#include "widgets/WidgetLoadFile.h"
#include "widgets/WidgetSaveFile.h"
#include "widgets/WidgetChooseFolder.h"
#include "widgets/WidgetDataGrid.h"
#include "widgets/WidgetTreeView.h"
#include "widgets/WidgetStatic.h"
#include "widgets/WidgetComboBox.h"
#include "widgets/WidgetMenu.h"
#include "widgets/WidgetMenuExtended.h"
#include "widgets/WidgetToolbar.h"
#include "widgets/WidgetSplitter.h"
#include "widgets/WidgetSlider.h"
#include "widgets/WidgetSpinner.h"
#include "widgets/WidgetChooseFont.h"
#include "widgets/WidgetChooseColor.h"
#include "widgets/WidgetProgressBar.h"
#include "ClipBoard.h"
#include "Threads.h"
#include "LibraryLoader.h"
#include "Bitmap.h"
#include "Cursor.h"
#include "Icon.h"
#include "ImageList.h"
#include "Resource.h"

namespace sw = SmartWin;

// 2005.05.03 was there 18427 lines of code in SmartWin

#endif
