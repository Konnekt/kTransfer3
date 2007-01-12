// $Revision: 1.8 $
#ifndef MainWindow_H
#define MainWindow_H

// This one should point to where SmartWin is installed, this one expects to find SmarWin
// on same catalog as the catalog to the current project is
#include "SmartWin.h"
#include "DialogWidget.h"

// Main class, this is our "driving" window, or "main" window.
class MainWindow
	: public SmartWin::WidgetFactory< SmartWin::WidgetWindow, MainWindow >
{
private:
	void staticTextClicked( WidgetStaticPtr staticCtrl );
	void staticTextGenTre( WidgetStaticPtr staticCtrl );
	void statusBarDblClicked( WidgetStatusBarPtr staticCtrl );
	void sized( const SmartWin::WidgetSizedEventResult & sz );
	void btnClicked( WidgetButtonPtr );
	void chkClicked( WidgetCheckBoxPtr );
	void btnClickedNext( WidgetButtonPtr );
	void treeViewClicked( WidgetTreeViewPtr );
	void menuClicked( WidgetMenuPtr item, unsigned int id );
	static void toolbarClicked( MainWindow * parent, WidgetToolbarPtr item, unsigned int id );
	bool listEdited( WidgetDataGridPtr list, unsigned col, unsigned row, SmartUtil::tstring & newVal );
	void comboBoxSelChanged( WidgetComboBoxPtr box );
	bool closing();
	void updateStatusBar();
	void moved( const SmartWin::Point & pt );

	WidgetStatusBarPtr itsStatusBar;
	WidgetButtonPtr itsButton;
	WidgetStaticPtr itsLblExp;
	WidgetStaticPtr itsLblGenTree;
	WidgetRichTextBoxPtr itsTxtField;
	WidgetMenuPtr itsMenu;
	WidgetMenuPtr itsMenu2;
	WidgetMenuPtr itsMenuSys;
	WidgetTreeViewPtr itsTree;
	WidgetToolbarPtr itsToolBar;
	WidgetCheckBoxPtr itsCheckBox;
	WidgetDataGridPtr itsList;
	WidgetComboBoxPtr itsBox;
	DialogWidget * itsDialogWidget;

	const static unsigned int MENU_FILE_OPEN = 1, MENU_FILE_SAVE = 2;
public:
	void init();
	MainWindow();

	// Note!
	// All derived Widget must (of course) have virtual DTORs
	virtual ~MainWindow();
};

#endif
