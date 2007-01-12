// $Revision: 1.5 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "FindDialog.h"
#include "resource.h"
#include "MainWindow.h"

bool FindDialog::closing()
{
	itsMain->closingFind();
	return true;
}

void FindDialog::closeClicked( WidgetButtonPtr btn )
{
	itsMain->closingFind();
	close();
}

void FindDialog::findClicked( WidgetButtonPtr btn )
{
	itsMain->find( itsTxt->getText() );
}

FindDialog::FindDialog( MainWindow * main )
		: Widget( main ),
		itsMain( main )
{
	createDialog( IDD_DIALOG1 );
	onClosing( & FindDialog::closing );

	WidgetButtonPtr btnFind = subclassButton( IDOK );
	WidgetButtonPtr btnClose = subclassButton( IDCANCEL );
	btnClose->onClicked( & FindDialog::closeClicked );
	btnFind->onClicked( & FindDialog::findClicked );
	itsTxt = subclassTextBox( IDC_EDIT1 );
	itsTxt->setFocus();
}

FindDialog::~FindDialog()
{}
