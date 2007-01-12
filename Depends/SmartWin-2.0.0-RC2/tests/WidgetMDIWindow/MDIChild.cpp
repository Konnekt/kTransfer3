// $Revision: 1.8 $

#include "MDIChild.h"
#include "WidgetMain.h"

MDIChild::MDIChild( WidgetMain * parent, SmartWin::WidgetMDIParent< WidgetMain, SmartWin::MessageMapPolicyNormalWidget > * mdiParent )
	: itsParent( parent ),
	Widget( mdiParent )
{
	createMDIChild();
	onClosing( & MDIChild::closeMdi );
	onSized( & MDIChild::sized );

	WidgetButtonPtr btn = createButton();
	btn->setBounds( 10, 10, 80, 20 );
	btn->setText( _T( "Push" ) );
	btn->onClicked( & MDIChild::btnClicked );
	txt = createTextBox();
	txt->setBounds( 10, 40, 160, 100 );
	WidgetChildWindowPtr w = createWidgetChildWindow();
	this->setBounds( 30, 30, 150, 150 );
}

bool MDIChild::closeMdi()
{
	itsParent->setText( txt->getText() );
	itsParent->mdiClosed( this );
	return true;
}

void MDIChild::btnClicked( WidgetButtonPtr btn )
{
	createMessageBox().show( getText(), btn->getText() );
}

void MDIChild::sized( const SmartWin::WidgetSizedEventResult & newSize )
{
	SmartWin::Rectangle rect( newSize.newSize );
	rect = rect.shrink( 10, 30 );
	txt->setBounds( rect );
}
