// $Revision: 1.8 $
#include "DialogWidget.h"
#include "resource.h"

void btnClicked( DialogWidget * parent, SmartWin::WidgetButton< DialogWidget, SmartWin::MessageMapPolicyDialogWidget > * btn )
{
	if ( btn->getText() == _T( "YES!!" ) )
		parent->close();
	else if ( btn->getText() == _T( "NOOO!" ) )
		parent->createMessageBox().show( _T( "Not that button please..." ) );
}

void ::DialogWidget::buttonPushed( WidgetButtonPtr button )
{
	createMessageBox().show( button->getText() );
}

::DialogWidget::DialogWidget( Widget * parent )
	: SmartWin::WidgetFactory< SmartWin::WidgetDialog, ::DialogWidget, SmartWin::MessageMapPolicyDialogWidget >( parent ),
	Widget( parent )
{
	createDialog( IDD_DIALOG1 );
	setText( _T( "SmartWin dialog..." ) );
	onClosing( & DialogWidget::closing );
	// Subclassing buttons
	itsOk = subclassButton( IDMYOK );
	itsOk->setText( _T( "YES!!" ) );
	itsOk->onClicked( btnClicked );
	itsCancel = subclassButton( IDMYCANCEL );
	itsCancel->setText( _T( "NOOO!" ) );
	itsCancel->onClicked( btnClicked );
	itsBtnTest = subclassButton( IDC_BUTTON1 );
	itsBtnTest->onClicked( & DialogWidget::buttonPushed );

	// Subclassing Edit Control
	itsTxt = subclassTextBox( IDC_EDIT1 );
	itsTxt->setText( _T( "Write your poem right here..." ) );

	// Adding a splitter
	itsSplitter = createSplitterCool();
	itsSplitter->setWidth( 8 );
	itsSplitter->onMoved( & DialogWidget::sizingSplitter );
	SmartWin::Rectangle splRc( getClientAreaSize() );
	splRc.pos.x = splRc.size.x / 2;
	splRc.size.x = 8;
	itsSplitter->setBounds( splRc );
	::SetWindowPos( itsSplitter->handle(), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
}

::DialogWidget::~DialogWidget()
{
}

// This one shows the "hard" way of resizing Widgets, look at e.g. AddressBook to see an "easier" way to do it
// Also to be "optimal" we should rather add two WidgetChildWindows, one to the left of the splitter and the other
// to the right and then add controls within THESE Widgets instead...
void ::DialogWidget::sizingSplitter( WidgetSplitterCoolPtr splitter, const SmartWin::Point & pt )
{
	// Resizing button
	SmartWin::Rectangle spltSize = itsSplitter->getBounds();
	SmartWin::Rectangle wndSize = getBounds();

	// Making some space
	SmartWin::Rectangle leftRect = wndSize;
	leftRect.size.y -= 50;
	leftRect.pos.x = 10;
	leftRect.pos.y = 10;

	// Ensuring LEFT of Splitter
	leftRect.size.x = spltSize.pos.x - wndSize.pos.x - 30;

	itsBtnTest->setBounds( leftRect );

	// Resizing txt area
	SmartWin::Rectangle rightRect = wndSize;
	rightRect.pos.x = spltSize.pos.x - wndSize.pos.x + 10;
	rightRect.pos.y = 80; // Down of the buttons
	rightRect.size.x = wndSize.size.x - rightRect.pos.x - 25;
	rightRect.size.y = wndSize.size.y - rightRect.pos.y - 45;

	itsTxt->setBounds( rightRect );
}

bool ::DialogWidget::closing()
{
	WidgetMessageBox box = createMessageBox();
	if ( box.show( _T( "Are you sure you wish to close this nice fine WidgetDialog?!?" ),
		_T( "Confirm!" ),
		WidgetMessageBox::BOX_OKCANCEL ) == WidgetMessageBox::RETBOX_OK )
	{
		box.show( _T( "Well, you can always start it again from the Menu (Help/Help Stuff/Show Help)" ) );
		return true;
	}
	return false;
}
