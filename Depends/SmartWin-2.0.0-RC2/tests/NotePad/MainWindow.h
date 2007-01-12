// $Revision: 1.7 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "SmartWin.h"
#include "FindDialog.h"

class MainWindow
			: public SmartWin::WidgetFactory< SmartWin::WidgetWindow, MainWindow >
{
private:
	typedef enum MENU_ITEM
	{
		LOAD = 1, SAVE, CUT, COPY, PASTE, FIND, ABOUT, CHOOSE_FONT
	};
	WidgetTextBox * itsTextField;
	WidgetMenuPtr itsMenu;
	FindDialog * itsFindBox;
	void isResized( const SmartWin::WidgetSizedEventResult & sz );
	void loadFile();
	void saveFile();
	void cut();
	void copy();
	void paste();
	void find();
	void about();
	void chooseFont();
	void menuClicked( WidgetMenuPtr menu, unsigned item );

public:
	void find( const SmartUtil::tstring & txt );
	void closingFind();
	MainWindow();
	~MainWindow();
};
