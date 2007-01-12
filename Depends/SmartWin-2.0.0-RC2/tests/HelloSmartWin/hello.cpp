// $Revision: 1.25 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
// Standard C++ of 1998 includes
#include "SmartWin.h"     // Fundamental include
#include <iostream>
#include <fstream>
using namespace SmartWin; // Lets you skip the prefix.

// hello.cpp
// Show the SmartWin++ features with lots of comments for beginners.
//
//

enum
{ ITEMS = 3
}; // The numer of items in lists
enum widg_page
{ SINGLE_WIDGETS = 0, MULTI_WIDGETS = 1
}; // Which of 2 tabbed pages

// HelloWinClass defines a window with native controls based on the Win32 API.
//
// For our purposes, use this pattern for simple windows:
// class YourClass
//     : public WidgetFactory< WidgetWindow, YourClass >
//
//
class HelloWinClass
			: public WidgetFactory< WidgetWindow, HelloWinClass >

			// Detailed explanation of the above C++ class template declaration syntax:
			//   HelloWinClass is derived from WidgetFactory.
			//   WidgetFactory has 3 template parameters: Base, Parent, and MessageMapPolicy
			//
			//  template< template<class, class> class Base,
			//            class Parent,
			//            class MessageMapPolicy = MessageMapPolicyNormalWidget>
			//  class WidgetFactory
			//
			// So HelloWinClass has template instance values of
			//   WidgetWindow, HelloWinClass, and MessageMapPolicyNormalWidget for
			//           Base,        Parent, and MessageMapPolicy
			//
			// Hold on, where did MessageMapPolicyNormalWidget come from ?
			// If you look at WidgetFactory.h you will see MessageMapPolicyNormalWidget is the default.
			// for MessageMapPolicy.
			//
			// This means that HelloWinClass can use the public part of WidgetFactory
			// and that the particular instance of WidgetFactory can refer to a Parent class
			// at compile time, and actually mean HelloWinClass at runtime.
{
private:
	// Hello will have two menus; a conventional top of window one,
	// and a popup that is invoked by right-clicking on the window area.
	WidgetMenuPtr itsMainMenu, itsPopupMenu;

	// Below are the definitions for the Widgets used in this example.
	// They look like simple pointers, but they are actually templated typedefs in WidgetFactory.
	// For our purposes, just declare WidgetXxxx *itsXxxx, initialize with itsXxxx= createXxxx()
	// and be happy that SmartWin handles the rest, including deleting them at the right time.
	//
	// By convention, we prefix all member variables with "its".
	//
	WidgetButtonPtr itsButton;
	WidgetCheckBoxPtr itsCheckBox;
	WidgetComboBoxPtr itsComboBox;
	WidgetDataGridPtr itsList;
	WidgetGroupBoxPtr itsRadioGroupBox;
	WidgetRadioButtonPtr itsRadios[ ITEMS ];
	WidgetTextBoxPtr itsTextBoxSource;
	WidgetSliderPtr itsHorizSlider, itsVertSlider;
	WidgetSpinnerPtr itsSpinner;
	WidgetStaticPtr itsPromptA, itsPromptB;
	WidgetTabSheetPtr itsTabSheet;
	WidgetTextBoxPtr itsTextA, itsTextB;
	WidgetTreeViewPtr itsTree;

	// The Widgets are organized on two child windows
	// which correspond to the two tabs of the WidgetTabSheet
	//
	WidgetChildWindowPtr itsMulti, itsSingle;

	// We have a little graphics state in the class.
	double itsGraphicsPart; // The fraction of the window to devote to graphics.
	COLORREF itsColor; // COLORREF is a Win32 type which is a 24 bit RGB value.
	Point itsEllipseSize; // A Point has x and y, which we interpret as sizes here.

public:

	HelloWinClass()
	{}

	// This is the response function from a menu option which was created as:
	// itsPopupMenu->appendItem( m++, _T("&Choose color"), &HelloWinClass::menuChooseColor );
	//
	// menu is the WidgetMenu created above but discarded.
	// The item has the value of m++.
	//
	// The result of this function is to change itsColor if the user selects a new color.
	// Then the paint function is redone to draw the graphics with the new color.
	//
	void menuChooseColor( WidgetMenuPtr menu, unsigned item )
	{
		WidgetChooseColor dlg = createChooseColor(); // Built in dialog.
		WidgetChooseColor::ColorParams params = dlg.showDialog( false ); // Initial color
		if ( params.userPressedOk() )
		{
			itsColor = params.getColor();

			UpdateCanvas uc( this );
			paint( uc );
		}
	}

	// Lets the user choose the font for the controls.
	// Then the window is redone.
	// How to set the Font for the Menus ??
	//
	void menuChooseFont( WidgetMenuPtr menu, unsigned item )
	{
#ifndef WINCE
		WidgetChooseFont widget = createChooseFont(); // Built in dialog.
		FontPtr font = widget.showDialog();
		if ( font.get() != 0 )
		{
			FontPtr choosenFont( font );
			itsTabSheet->setFont( choosenFont );

			itsButton->setFont( choosenFont );
			itsTextBoxSource->setFont( choosenFont );
			itsCheckBox->setFont( choosenFont );
			itsPromptA->setFont( choosenFont ); itsTextA->setFont( choosenFont );
			itsPromptB->setFont( choosenFont ); itsTextB->setFont( choosenFont );

			itsTree->setFont( choosenFont );
			itsComboBox->setFont( choosenFont );
			itsList->setFont( choosenFont );
			itsRadioGroupBox->setFont( choosenFont );
			for ( int i = 0; i < ITEMS; i++ ) itsRadios[ i ] ->setFont( choosenFont );

			updateWidget(); // Redo everything with the new font.
			layout(); // to show how the new Font effects the layout.
		}
#endif
	}

	void menuCloseEvent( WidgetMenuPtr menu, unsigned item )
	{
		close(); // Exit the window.  All Widgets are automatically removed.
	}

	// A mouse event response function
	// mouse.pos uses window relative coordinates.
	// The trackPopupMenu takes a screen position,
	// so we add mouse.pos to the window position.
	//
	void PopupMenuViaRightClick( const MouseEventResult & mouse )
	{
		SmartWin::Rectangle possize = getBounds();
		int x = possize.pos.x + mouse.pos.x;
		int y = possize.pos.y + mouse.pos.y;

		itsPopupMenu->trackPopupMenu( this, x, y );
	}

	void copyTextToClipboard( WidgetMenuPtr menu, unsigned item )
	{
		SmartUtil::tstring selection = itsTextBoxSource->getSelection();
		ClipBoardString::instance().setClipBoardData( selection, this );
	}

	// The fundamental button click response function.
	// The passed parameter is the button that was clicked, in case you want
	// the same response function for multiple buttons.
	//
	void buttonClicked( WidgetButtonPtr button )
	{
		WidgetMessageBox mb = createMessageBox();
		switch ( mb.show( _T( "Do you like SmartWin++ ?" ),
						  _T( "Survey" ), WidgetMessageBox::BOX_YESNOCANCEL ) )
		{
			case WidgetMessageBox::RETBOX_YES :
				mb.show( _T( "Then, tell us how to be even better !" ) );
				break;

			case WidgetMessageBox::RETBOX_NO :
				mb.show( _T( "Then, tell us how to improve !" ) );
				break;

			case WidgetMessageBox::RETBOX_CANCEL :
				break;
		}
	}

	void checkclicked( WidgetCheckBoxPtr chk )
	{
		if ( chk->getChecked() )
		{
			chk->setText( _T( "Please uncheck me" ) );
		}
		else
		{
			chk->setText( _T( "Check me" ) );
		}
		layout(); // to reflect the different text value in the checkbox.
	}

	void scrollHoriz( WidgetSliderPtr slider )
	{
		itsEllipseSize.x = slider->getPosition();
		UpdateCanvas uc( this );
		paint( uc );
	}

	void scrollVert( WidgetSliderPtr slider )
	{
		itsEllipseSize.y = slider->getPosition();
		UpdateCanvas uc( this );
		paint( uc );
	}

	void spinVert( WidgetSpinnerPtr spinner )
	{
		itsEllipseSize.y = spinner->getValue() * 10;
		UpdateCanvas uc( this );
		paint( uc );
	}

	void radioClicked( WidgetRadioButtonPtr btn )
	{
		for ( int i = 0; i < ITEMS; i++ )
		{
			if ( btn == itsRadios[ i ] )
			{
				itsComboBox->setSelectedIndex( i );
			}
		}
	}

	void selectionChanged( WidgetComboBoxPtr comboBox )
	{
		createMessageBox().show( _T( "New selection: " ) + comboBox->getSelectedValue() );
	}

	void treeNodeSelChanged( WidgetTreeViewPtr tree )
	{
		SmartUtil::tstring itemText = tree->getSelectedItemText();
		int dex = tree->getSelectedIndex();
		if ( - 1 == dex )
			createMessageBox().show( itemText );
		else
		{
			itsComboBox->setSelectedIndex( dex );
		}
	}

	// Read in the ASCII file a line at a time.
	//
	void FetchFile( SmartUtil::tstring filePath, SmartUtil::tstring & filedata )
	{
		std::basic_ifstream< TCHAR > file( filePath.c_str() );
		if ( !file.good() )
			return;

		const int bsize = 65536;
		TCHAR buff[ bsize ];
		while ( file.getline( buff, bsize ) )
		{
			filedata += buff;
			filedata += _T( "\r\n" );
		}
	}

	void createSingleItems()
	{
		// The inputs and outputs of the dialog
		WidgetStatic::Seed staticCS;
		WidgetTextBox::Seed textBoxCS;

		staticCS.style |= ES_RIGHT; // Make the prompts right justified.
		textBoxCS.style |= ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL;

		staticCS.caption = _T( "First prompt: " );
		itsPromptA = itsSingle->createStatic( staticCS );
		textBoxCS.caption = _T( "Bugs Bunny" );
		itsTextA = itsSingle->createTextBox( textBoxCS );

		staticCS.caption = _T( "Second prompt: " );
		itsPromptB = itsSingle->createStatic( staticCS );
		textBoxCS.caption.clear();
		itsTextB = itsSingle->createTextBox( textBoxCS );

		itsButton = itsSingle->createButton();
		itsButton->setText( _T( "Display a message" ) );
		itsButton->onClicked( & HelloWinClass::buttonClicked );

		itsCheckBox = itsSingle->createCheckBox();
		itsCheckBox->setText( _T( "&Check me" ) );
		itsCheckBox->onClicked( & HelloWinClass::checkclicked );

		itsTextBoxSource = itsSingle->createTextBox( textBoxCS );
		SmartUtil::tstring filedata;
		FetchFile( _T( "hello.cpp" ), filedata );
		itsTextBoxSource->setText( filedata );
		onRightMouseDown( & HelloWinClass::PopupMenuViaRightClick );

		itsSpinner = itsSingle->createSpinner();
		itsSpinner->setRange( 0, 10 );
		itsSpinner->setValue( 10 );
		itsSpinner->onScrollVert( & HelloWinClass::spinVert );

		itsHorizSlider = itsSingle->createSlider();
		itsHorizSlider->setAutoTicks( true );
		itsHorizSlider->setHorizontal( true );
		itsHorizSlider->setShowTicksBoth( true );
		itsHorizSlider->setRange( 0, 100 );
		itsHorizSlider->setPosition( 100 );
		itsHorizSlider->setTickFrequency( 10 );
		itsHorizSlider->onScrollHorz( & HelloWinClass::scrollHoriz );

		itsVertSlider = itsSingle->createSlider();
		itsVertSlider->setAutoTicks( true );
		itsVertSlider->setHorizontal( false );
		itsVertSlider->setShowTicksBoth( true );
		itsVertSlider->setRange( 0, 100 );
		itsVertSlider->setPosition( 100 );
		itsVertSlider->setTickFrequency( 10 );
		itsVertSlider->onScrollVert( & HelloWinClass::scrollVert );
	}

	void createMultiItems()
	{
		TCHAR * cols[] =
		{ _T( "Alpha" ), _T( "Beta" ), _T( "Charlie" )
		};
		TCHAR * labels[] =
		{ _T( "Zero" ), _T( "One" ), _T( "Two" )
		};
		int labN = sizeof( labels ) / sizeof( char * );
		int colN = sizeof( cols ) / sizeof( char * );

		itsRadioGroupBox = itsMulti->createGroupBox();
		itsRadioGroupBox->setText( _T( "Radio buttons" ) );
		for ( int i = 0; i < labN; i++ )
		{
			itsRadios[ i ] = createRadioButton( itsRadioGroupBox );
			itsRadios[ i ] ->setText( labels[ i ] );
			itsRadios[ i ] ->onClicked( & HelloWinClass::radioClicked );
		}

		// Combo Boxes
		itsComboBox = itsMulti->createComboBox();
		for ( int i = 0; i < labN; i++ )
		{
			itsComboBox->addValue( labels[ i ] );
		}
		itsComboBox->setSelectedIndex( 2 );
		itsComboBox->onSelectionChanged( & HelloWinClass::selectionChanged );

		// List or grid Widget
		itsList = itsMulti->createDataGrid();
		std::vector< SmartUtil::tstring > columns;
		for ( int i = 0; i < colN; i++ ) columns.push_back( cols[ i ] );
		itsList->createColumns( columns );

		std::vector< SmartUtil::tstring > values;
		for ( int i = 0; i < labN; i++ ) values.push_back( labels[ i ] );
		itsList->insertRow( values );
		itsList->insertRow( values );

		// Creating tree view
		itsTree = itsMulti->createTreeView();
		itsTree->setHasButtons( true );
		itsTree->setHasLines( true );
		itsTree->setLinesAtRoot( true );
		itsTree->setTrackSelect( true );
		itsTree->setEditLabels();
		itsTree->onSelectionChanged( & HelloWinClass::treeNodeSelChanged );

		// Inserting items into tree view
		TreeViewNode root = itsTree->insertNode( _T( "Root" ), TreeViewNode(), static_cast< unsigned int >( - 1 ) );
		for ( int i = 0; i < colN; i++ ) itsTree->insertNode( labels[ i ], root, i );
	}

	void initAndCreate()
	{
		createWindow();
		setText( _T( "Hello SmartWin" ) ); // Title

		SmartWin::Rectangle desktop( getDesktopSize() );
		desktop.pos.y += 25;
		desktop.size.y -= 25;
		setBounds( desktop.top( 0.95 ) ); // or maximize();

		itsGraphicsPart = 0.25;
		itsColor = 0x808000; // Defaulting to yellow...
		itsEllipseSize = Point( 100, 100 );

		// Creating main menu
		itsMainMenu = createMenu();
		WidgetMenuPtr file = itsMainMenu->appendPopup( _T( "&File" ) );
		int m = 1;
		file->appendItem( m++, _T( "Choose &font" ), & HelloWinClass::menuChooseFont );
		file->appendItem( m++, _T( "&Copy text selection" ), & HelloWinClass::copyTextToClipboard );

		// Creating a right click pop-up menu
		itsPopupMenu = file->appendPopup( _T( "&Graphics" ) );
		itsPopupMenu->appendItem( m++, _T( "&Choose color" ), & HelloWinClass::menuChooseColor );

		file->appendItem( m++, _T( "&Exit" ), & HelloWinClass::menuCloseEvent );
#ifndef WINCE
		itsMainMenu->attach( this ); // Attaching main menu to main window
#endif

		itsTabSheet = createTabSheet();
		itsTabSheet->setHotTrack();
		itsTabSheet->setButtonStyle( false );
		itsTabSheet->setTabsAtBottom( false );
		itsTabSheet->addPage( _T( "Simple" ), SINGLE_WIDGETS );
		itsTabSheet->addPage( _T( "Complex" ), MULTI_WIDGETS );
		itsTabSheet->onSelectionChanged( & HelloWinClass::tabSelectionChanged );
		itsTabSheet->setSelectedIndex( SINGLE_WIDGETS );

		itsMulti = createWidgetChildWindow();
		createMultiItems();
		itsSingle = createWidgetChildWindow();
		createSingleItems();

		onPainting( & HelloWinClass::painting );

		layout(); // for the first time, and only AFTER all Widgets are created.
		onSized( & HelloWinClass::isResized );

		// Since tab sheet will hide underlaying controls we need to position it at the BOTTOM of the Z-order window chain
		::SetWindowPos( itsTabSheet->handle(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		updateWidget();
	}

	void isResized( const WidgetSizedEventResult & sz )
	{
		layout(); // to reflect the change in window size by the user.
		updateWidget(); // Tell painting part that it needs to redraw.
	}

	void tabSelectionChanged( WidgetTabSheetPtr sheet )
	{
		layout(); // according to the new tab selection.
	}

	// Common routine to do layout the first time, and after each resize event.
	// We divide the available space into:   Tabbed Area | Graphics Area
	//
	void layout()
	{
		SmartWin::Rectangle r( getClientAreaSize() );

		r = r.left( 1.0 - itsGraphicsPart ); // The non-graphics side.
		itsTabSheet->setBounds( r );

		Point tabLabelSize = itsTabSheet->getTextSize( _T( "Complex" ) );
		tabLabelSize.x = 0; // Only concerned with the label's height
		tabLabelSize.y += 10;
		r = r.upperLeftAdjust( tabLabelSize ); // Tab labels
		r = r.shrinkWidth( 5 ); // , and have a x border

		itsSingle->setBounds( r );
		itsMulti->setBounds( r );

		// Coordinates for Widgets inside child windows are zero based.
		r = SmartWin::Rectangle( r.size );

		enum widg_page page = ( widg_page ) itsTabSheet->getSelectedIndex();
		layout_single( SINGLE_WIDGETS == page, r );
		layout_multi( MULTI_WIDGETS == page, r );
	}

	// The scheme is:
	//
	//  itsPromptA     itsTextA  \
	//  itsPromptB     itsTextB  /  Top 30%
	//
	//  button slider checkbox   >  Bottom 65%
	//
	// We achieve the layout using a combination of
	// Rectangle functions and the Place class.
	//
	void layout_single( bool isVisible, SmartWin::Rectangle rect )
	{
		if ( isVisible )
		{
			SmartWin::Rectangle highrect( rect.top( 0.3 ) );
			SmartWin::Rectangle lowrect( rect.bottom( 0.65 ) );

			Place p;
			p.setBoundsBorders( highrect, 4, 4 ); // X, Y border between Widgets

			int rows = 2, cols = 2; // For sizing purposes, we divide into a grid.

			// The first row.
			itsPromptA->setSizeAsGridPerPlace( p, rows, cols );
			itsTextA->setSizeAsGridPerPlace( p, rows, cols );

			// The second row.
			// The Place class notices that we have reached the end of a row and starts the next one.
			itsPromptB->setSizeAsGridPerPlace( p, rows, cols );
			itsTextB->setSizeAsGridPerPlace( p, rows, cols );

			// The idea above was to use all the space available
			// and expand the Widgets to fit into the space.

			// For the Widgets below, we will use fixed sizes,
			// and use the Place class to position them
			// one after another according to thier sizes.
			//
			// Place::positionToRight( Rectangle r ) works like this:
			//  It sets r.pos to the current position that traverses the Rectangle
			//  set in Place::setBoundsBorders().  Then it adjusts the current
			//  position taking into account the r.size and the borders.
			//  If it needs to, it will move to a new "line" of Widgets.
			//
			p.setBoundsBorders( lowrect, 2, 4 ); // X, Y border between Widgets
			SmartWin::Rectangle pos_size;

			// Fixed sizes.
			pos_size.size = Point( 30, 50 );
			p.positionToRight( pos_size ); // Sets the pos_size.pos, and adjusts for pos_size.size.
			itsSpinner->setBounds( pos_size ); // The spinner's size will be 30 by 50 pixels

			pos_size.size = Point( 40, 50 );
			p.positionToRight( pos_size );
			itsVertSlider->setBounds( pos_size );

			pos_size.size = Point( 70, 50 );
			p.positionToRight( pos_size );
			itsHorizSlider->setBounds( pos_size );

			// These two will have enough size to fit their text in the current font.
			itsCheckBox->setSizePerTextPerPlace( p, itsCheckBox->getText(), 40, 4 );
			itsButton->setSizePerTextPerPlace( p, itsButton->getText(), 40, 4 );

			p.newRow();
			p.sizeForRemainingSpace( pos_size );
			itsTextBoxSource->setBounds( pos_size );
		}

		itsSingle->setVisible( isVisible );
	}

	void layout_multi( bool isVisible, SmartWin::Rectangle rect )
	{
		if ( isVisible )
		{
			Place p;
			p.setBoundsBorders( rect, 6, 8 ); // X, Y border between Widgets

			int rows = 2, cols = 3; // For sizing purposes, we divide into a grid.
			itsComboBox->setSizeAsGridPerPlace( p, rows, cols );
			itsTree->setSizeAsGridPerPlace( p, rows, cols );

			SmartWin::Rectangle radiorect;
			p.sizeOfCell( rows, cols, radiorect.size );
			p.positionToRight( radiorect );
			itsRadioGroupBox->setBounds( radiorect );
			Point groupsize = itsRadioGroupBox->getTextSize( itsRadioGroupBox->getText() );

			int radiorows = ITEMS;
			int rowdex;
			SmartWin::Rectangle radio1inside( radiorect.size ); // Start at 0,0 again.
			radio1inside = radio1inside.upperLeftAdjust( Point( 0, groupsize.y ) ); // Allow room for Group label
			radio1inside = radio1inside.shrink( 4, 4 );
			rowdex = 0;
			for ( int rowdex = 0; rowdex < ITEMS; rowdex++ )
			{
				itsRadios[ rowdex ] ->setSizeAsCol( radio1inside, radiorows, rowdex );
			}

			// List Grid takes the rest ...
			SmartWin::Rectangle ListGridRect;
			p.sizeForRemainingSpace( ListGridRect );
			itsList->setBounds( ListGridRect );
		}

		itsMulti->setVisible( isVisible );
	}

	void painting( Canvas & pc )
	{
		paint( pc );
	}

	void paint( Canvas & c )
	{
		Pen pen( c, itsColor );
		Brush brush( c, itsColor );
		TextPen textpen( c, itsColor );
		COLORREF background;

		SmartWin::Rectangle rc( getClientAreaSize() ); // Establish 0,0 based SmartWin::Rectangle
		SmartWin::Rectangle area( rc.right( itsGraphicsPart ) ); // Use the Graphics side.
		area.shrink( 5, 3 ); // Allocate a border in pixels
		background = c.getSysColor( COLOR_BTNFACE ); //     c.getBkColor();

		int r = 0, rows = 5; // A column of graphics elements

		SmartWin::Rectangle texArea( area.row( r++, rows ) );
		c.setBkColor( background );
		c.extTextOut( _T( "Hello Graphical World" ), texArea.pos.x, texArea.pos.y );

		SmartWin::Rectangle ellipseArea( area.row( r++, rows ) );
		{
			Pen backpen( c, background );
			Brush backbrush( c, background );
			c.rectangle( ellipseArea ); // Always erase the ellipse area first.
		}

		ellipseArea.size.x = ( ellipseArea.size.x * itsEllipseSize.x ) / 100;
		ellipseArea.size.y = ( ellipseArea.size.y * itsEllipseSize.y ) / 100;
		c.ellipse( ellipseArea );

		c.line( area.row( r++, rows ) );
		{
			TextPen purplepen( c, 0x800080 );
			c.drawText( _T( "Hello Purple World" ), area.row( r++, rows ), DT_SINGLELINE | DT_CENTER | DT_VCENTER );
		} // purplepen is now out of scope, so we revert to old color.

		c.rectangle( area.row( r++, rows ) );
	}
};

// The main creates an instance of the class and then starts the SmartWin application.
//
int SmartWinMain( Application & app )
{
	HelloWinClass * testHello = new HelloWinClass;
	testHello->initAndCreate();
	return app.run();
}
