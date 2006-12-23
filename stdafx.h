// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef __STDAFX_H__
#define __STDAFX_H__
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:

#include <windows.h>

#include <Stamina/Object.h>
#include <Stamina/Time64.h>
#include <Stamina/Object.h>
#include <Stamina/ObjectImpl.h>
#include <Stamina/ListWnd/ListWnd.h>
#include <Stamina/ListWnd/ListView.h>
#include <Stamina/ListWnd/Entry.h>
#include <Stamina/Helpers.h>
#include <Stamina/Exception.h>

//#include <stdlib.h>
//#include <winnls.h>
//#include <process.h>
//#include <string>
//#include <math.h>
#include <vector>
#include <map>
#include <list>
#include <deque>

#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "konnekt/plug_export.h"
#include "konnekt/ui.h"
#include "konnekt/plug_func.h"
#include "konnekt/plug.h"
#include "konnekt/core_ctrl.h"
#include "konnekt/kNotify.h"

#include "PlugController.h"

#include "Transfer.h"
#include "Item.h"
#include "Directory.h"
#include "File.h"
#include "Items.h"
#include "Option.h"
#include "Queue.h"
#include "Group.h"
#include "State.h"

using namespace boost;
using namespace Konnekt;
using namespace Stamina;

#endif /*__STDAFX_H__*/


// TODO: reference additional headers your program requires here
