// $Revision: 1.16 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "MainWindow.h"
#include "resource.h"

#include <fstream>

void MainWindow::isResized( const SmartWin::WidgetSizedEventResult & sz )
{
	using namespace SmartWin;
	SmartWin::Rectangle rz( getClientAreaSize() );
	itsTextField->setBounds( rz );
}

void MainWindow::loadFile()
{
	using namespace SmartWin;
	using namespace std;
	WidgetLoadFile dlg = createLoadFile();
	dlg.addFilter( _T( "Text files (*.txt)" ), _T( "*.txt" ) );
	dlg.addFilter( _T( "All files (*.*)" ), _T( "*.*" ) );
	SmartUtil::tstring filePath = dlg.showDialog();
	if ( filePath.size() != 0 )
	{
		basic_ifstream< TCHAR > file( SmartUtil::AsciiGuaranteed::doConvert( filePath.c_str(), SmartUtil::ConversionCodepage::ANSI ).c_str() );
		if ( !file.good() )
			return;
		vector< TCHAR > fileContent;
		TCHAR ch;
		while ( true )
		{
			ch = file.get();
			if ( !file.good() )
				break;
			if ( '\n' == ch )
			{
				fileContent.push_back( '\r' );
				fileContent.push_back( '\n' );
			}
			else
				fileContent.push_back( ch );
		}
		itsTextField->setText( SmartUtil::tstring( fileContent.begin(), fileContent.end() ) );
	}
}

void MainWindow::saveFile()
{
	using namespace SmartWin;
	using namespace std;
	WidgetSaveFile dlg = createSaveFile();
	dlg.addFilter( _T( "Text Files (*.txt)" ), _T( "*.txt" ) );
	dlg.addFilter( _T( "All Files (*.*)" ), _T( "*.*" ) );
	SmartUtil::tstring filePath = dlg.showDialog();
	if ( filePath.size() != 0 )
	{
		basic_ofstream< TCHAR > file( SmartUtil::AsciiGuaranteed::doConvert( filePath.c_str(), SmartUtil::ConversionCodepage::ANSI ).c_str() );
		if ( file.good() )
		{
			file << itsTextField->getText();
		}
	}
}

void MainWindow::cut()
{
	SmartUtil::tstring selection = itsTextField->getSelection();
	SmartWin::ClipBoardString::instance().setClipBoardData( selection, this );
	itsTextField->replaceSelection( _T( "" ), true );
}

void MainWindow::copy()
{
	SmartUtil::tstring selection = itsTextField->getSelection();
	SmartWin::ClipBoardString::instance().setClipBoardData( selection, this );
}

void MainWindow::paste()
{
	if ( SmartWin::ClipBoardString::instance().isClipBoardFormatAvailable( SmartWin::ClipBoardBase::TEXT ) )
		itsTextField->replaceSelection( SmartWin::ClipBoardString::instance().getClipBoardData( this ) );
}

void MainWindow::find()
{
	if ( 0 == itsFindBox )
	{
		itsFindBox = new FindDialog( this );
	}
	else
	{
		itsFindBox->setFocus();
	}
}

void MainWindow::find( const SmartUtil::tstring & txt )
{
	long position = itsTextField->findText( txt, itsTextField->getCaretPos() );
	if ( position == - 1 )
		return;
	itsTextField->setSelection( position, static_cast< long >( txt.size() ) + position );
	itsTextField->setFocus();
	itsTextField->showCaret();
}

void MainWindow::closingFind()
{
	itsFindBox = 0;
}

void MainWindow::menuClicked( WidgetMenuPtr menu, unsigned item )
{
	switch ( item )
	{
		case LOAD :
			{
				loadFile();
			}
			break;
		case SAVE :
			{
				saveFile();
			}
			break;
		case CUT :
			{
				cut();
			}
			break;
		case COPY :
			{
				copy();
			}
			break;
		case PASTE :
			{
				paste();
			}
			break;
		case FIND :
			{
				find();
			}
			break;
		case ABOUT :
			{
				about();
			}
			break;
		case CHOOSE_FONT :
			{
				chooseFont();
			}
			break;
	}
}

MainWindow::MainWindow()
		: itsFindBox( 0 )
{
	createWindow();
	setText( _T( "SmartWin - NotePad" ) );
	onSized( & MainWindow::isResized );

	WidgetTextBox::Seed textBoxCS;

	textBoxCS.style = ES_MULTILINE | ES_WANTRETURN | WS_VISIBLE;
	textBoxCS.exStyle = WS_EX_CLIENTEDGE;
	textBoxCS.font = sw::createFont( _T( "Courier New" ), 16, 0, 1, ANSI_CHARSET, false, false, false, 0, OUT_DEFAULT_PRECIS );
	itsTextField = createTextBox( textBoxCS );
	itsTextField->setScrollBarHorizontally();
	itsTextField->setScrollBarVertically();

	itsMenu = createMenu();
	WidgetMenuPtr file = itsMenu->appendPopup( _T( "&File" ) );
	file->appendItem( LOAD, _T( "&Load File" ), & MainWindow::menuClicked );
	file->appendItem( SAVE, _T( "&Save File" ), & MainWindow::menuClicked );

	WidgetMenuPtr edit = itsMenu->appendPopup( _T( "&Edit" ) );
	edit->appendItem( CUT, _T( "&Cut" ), & MainWindow::menuClicked );
	edit->appendItem( COPY, _T( "C&opy" ), & MainWindow::menuClicked );
	edit->appendItem( PASTE, _T( "&Paste" ), & MainWindow::menuClicked );
	edit->appendItem( FIND, _T( "&Find" ), & MainWindow::menuClicked );
	edit->appendItem( CHOOSE_FONT, _T( "Choose Fon&t" ), & MainWindow::menuClicked );

	WidgetMenuPtr help = itsMenu->appendPopup( _T( "&Help" ) );
	help->appendItem( ABOUT, _T( "&About Notepad..." ), & MainWindow::menuClicked );

	itsMenu->attach( this );

	setBounds( 100, 100, 800, 600 );
	itsTextField->setFocus();
}

MainWindow::~MainWindow()
{}

// A WidgetModalDialog dialog window
class AboutBox
			: public SmartWin::WidgetFactory< SmartWin::WidgetModalDialog, AboutBox, SmartWin::MessageMapPolicyModalDialogWidget >
{
private:
	unsigned itsButtonPressedNumberOfTimes;

public:
	AboutBox( SmartWin::Widget * parent )
			: SmartWin::WidgetFactory< SmartWin::WidgetModalDialog, AboutBox, SmartWin::MessageMapPolicyModalDialogWidget >( parent ),
			Widget( parent ), itsButtonPressedNumberOfTimes( 0 )
	{
		// Note!
		// This is about the ONLY event handler in SmartWin which HAVE to be defined BEFORE
		// the actual Widget is displayed through "createXXX" (createDialog)
		// the reason is that the "createDialog" function NEVER returns BEFORE the Widget is DESTROYED and CLOSED
		// therefore if we want to at all be able to have "custom functionality" in our Widget we must add
		// that custom functionality BEFORE the "createDialog" is being called!!
		onInitDialog( & AboutBox::init );
	}

	unsigned getNumberOfButtonPresses()
	{
		return itsButtonPressedNumberOfTimes;
	}

	void clicked( WidgetButtonPtr btn )
	{
		++itsButtonPressedNumberOfTimes;
	}

	bool init()
	{
		WidgetButtonPtr btn = subclassButton( IDC_BUTTON1 );
		btn->onClicked( & AboutBox::clicked );
		return true;
	}

	~AboutBox()
	{}
}
;

void MainWindow::chooseFont()
{
	WidgetChooseFont widget = createChooseFont();
	SmartWin::FontPtr font = widget.showDialog();
	if ( font.get() != 0 )
	{
		itsTextField->setFont( font );
	}
}

void MainWindow::about()
{
	// Note!
	// This is the ONLY place in SmartWin you need to do your OWN house cleaning...
	// The reason to this is that you're likely to have some return values that you're interested in retreiving from
	// the ModalDialog and since the ModalDialog doesn't return before the Widget is closed we CAN'T have the
	// Modal Dialog destroy itself like other Widgets do.
	// The easiest way to therefore assure that you're not loosing memory is just allocating the dialog on the HEAP memory or Stack
	AboutBox about( this );
	about.createDialog ( IDD_ABOUTDIALOG );
	std::basic_stringstream< TCHAR > stream;
	stream << "Button pressed: " << about.getNumberOfButtonPresses() << " times!!";
	createMessageBox().show( stream.str() );
}
