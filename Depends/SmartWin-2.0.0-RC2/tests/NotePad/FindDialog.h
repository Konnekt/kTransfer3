// $Revision: 1.6 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include "SmartWin.h"

class MainWindow;

class FindDialog
			: public SmartWin::WidgetFactory< SmartWin::WidgetDialog, FindDialog, SmartWin::MessageMapPolicyDialogWidget >
{
private:
	WidgetTextBoxPtr itsTxt;
	MainWindow * itsMain;
	bool closing();
	void closeClicked( WidgetButtonPtr btn );
	void findClicked( WidgetButtonPtr btn );

public:
	FindDialog( MainWindow * main );
	~FindDialog();
};

#endif
