// $Revision: 1.19 $
// WidgetRadioButtonGroup example program
// Unit test tries to instantiate all different forms, and use all functions
// Besides demonstrating the use of RadioButtons, it also shows
// various fonts and layout techniques.
//
#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

// The "factory" version
// Using class based layout techniques.
//
// This class responds to window resizing by adjusting the positions of the radio
// groups accordingly
class RadioButGroupLayout
	: public WidgetFactory< WidgetWindow, RadioButGroupLayout >
{
public:
	RadioButGroupLayout()
	{}

	bool checked21;
	WidgetTextBoxPtr txt, descr;
	WidgetGroupBoxPtr group, group2;
	WidgetRadioButtonPtr radio1, radio2, radio3;
	WidgetRadioButtonPtr radio21, radio22, radio23;

	void radioClicked( WidgetRadioButtonPtr btn )
	{
		// Since one of those buttons are ownerdrawned we need to do this...
		// BS_OWNERDRAW RadioButtons don't maintain state at all
		if ( btn == radio21 )
		{
			checked21 = true;
			radio22->setChecked( false );
			radio23->setChecked( false );
			radio21->updateWidget();
		}
		else if ( btn == radio22 )
		{
			checked21 = false;
			radio22->setChecked( true );
			radio23->setChecked( false );
			radio21->updateWidget();
		}
		else if ( btn == radio23 )
		{
			checked21 = false;
			radio22->setChecked( false );
			radio23->setChecked( true );
			radio21->updateWidget();
		}
		if ( btn->getText() == _T( "Radio 1 - 1" ) )
			group->setEnabled( false );
		else
			group->setEnabled( true );
		txt->setText( btn->getText() );
	}

	void radioDblClicked( WidgetRadioButtonPtr btn )
	{
		txt->setText( btn->getText() + _T( " double clicked" ) );
	}

	void isResized( const SmartWin::WidgetSizedEventResult & sz )
	{
		layout( sz.newSize );
	}

	void layout( SmartWin::Point newsize )
	{
		//      [radio1]    [radio2]
		//         [  text box  ]
		//
		SmartWin::Rectangle win( newsize );
		win = win.shrink( 0.90 );

		// Northwest corner  - Shrink X and Y sizes by 50%.
		SmartWin::Rectangle radio1group( win, 0, 0, 0.5, 0.5 );
		group->setBounds( radio1group.cropRight( 5 ) );

		// Northeast corner - Move X position 50% over, and Shrink sizes by 50%.
		SmartWin::Rectangle radio2group( win, 0.5, 0, 0.5, 0.5 );
		group2->setBounds( radio2group.cropLeft( 5 ) );

		// Read as: Create textboxrect with 10% in from the left edge,
		// and 75% in from the top edge.  Its sizes are 80% x and 25% y
		// of the old rectangle.
		SmartWin::Rectangle textboxrect( win, 0.1, 0.85, 0.80, 0.15 );
		txt->setBounds( textboxrect.cropTop( 5 ) );

		SmartWin::Rectangle descrpRect( win, 0.1, 0.5, 0.80, 0.35 );
		descr->setBounds( descrpRect.cropBottom( 5 ).cropTop( 10 ) );

		const int rows = 3;
		int rowdex;
		//HC: was the intended behavior to get the coodinates respect to the group instead of the window?
		SmartWin::Rectangle radio1inside( sw::Rectangle(group->getSize())/*.size*/ );   // Start at 0,0 again.
		radio1inside = radio1inside.shrink( 0.8 ); // Make the inside rectangle 20% smaller.
		rowdex = 0;
		radio1->setSizeAsCol( radio1inside, rows, rowdex++, 0 );
		radio2->setSizeAsCol( radio1inside, rows, rowdex++, 0 );
		radio3->setSizeAsCol( radio1inside, rows, rowdex++, 0 );

		SmartWin::Rectangle radio2inside( sw::Rectangle( group->getSize() ) ); // Start at 0,0 again.
		radio2inside = radio2inside.shrink( 10, 20 ); // Borders of 5 and 10 on each side.
		rowdex = 0;
		radio21->setSizeAsCol( radio2inside, rows, rowdex++, 0 );
		radio22->setSizeAsCol( radio2inside, rows, rowdex++, 0 );
		radio23->setSizeAsCol( radio2inside, rows, rowdex++, 0 );
	}

	BrushPtr itsBgColor;
	BrushPtr itsBgColor2;

	BrushPtr bgColor( WidgetGroupBoxPtr group, sw::Canvas & canvas )
	{
		// The background color returned won't really be used (unless you comment
		// away the line that sets the onEraseBackground) Event Handler, but we
		// must return a brush anyway...
		itsBgColor = BrushPtr( new sw::Brush( canvas, RGB( 0, 0, 255 ) ) );
		SmartUtil::tstring txt = group->getText();
		FontPtr font = group->getFont();
		canvas.selectFont( font );
		canvas.setBkMode( true );
		canvas.drawText( txt, sw::Rectangle( 0, 0, 100, 30 ), 0 );
		return itsBgColor;
	}

	void erase( WidgetGroupBoxPtr group, sw::Canvas & canvas )
	{
		sw::Rectangle rect( group->getClientAreaSize() );
		itsBgColor2 = BrushPtr( new sw::Brush( canvas, RGB( 0, 255, 0 ) ) );
		canvas.fillRectangle( rect, * itsBgColor2 );
	}

	void clicked( WidgetGroupBoxPtr box )
	{
		this->setText( _T( "Clicked" ) );
	}

	void dblClicked( WidgetGroupBoxPtr box )
	{
		this->setText( _T( "DOUBLE Clicked" ) );
	}

	void enabled( WidgetGroupBoxPtr box, bool isEnabled )
	{
		if ( isEnabled )
			this->setText( _T( "Enabled" ) );
		else
			this->setText( _T( "NOT Enabled" ) );
	}

	void painting( WidgetRadioButtonPtr btn, Canvas & canvas )
	{
		FontPtr font = btn->getFont();
		canvas.selectFont( font );
		canvas.setBkMode( true );
		SmartUtil::tstring txt = btn->getText();
		if ( checked21 )
		{
			canvas.setTextColor( RGB( 255, 0, 0 ) );
		}
		else
		{
			canvas.setTextColor( RGB( 128, 128, 128 ) );
		}
		canvas.drawText( txt, sw::Rectangle( btn->getSize() ), 0 );
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 80, 0, 600, 600 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "WidgetRadioButtonGroup compile and execute test" );
		createWindow( windowCS );

		checked21 = false;

		WidgetTextBox::Seed textBoxCS;

		textBoxCS.caption = _T( "The first group will disable after clicking on its first button." )
			_T( "To enable it back, make a selection on the second group. The " )
			_T( "third button of the fisrt group is always disabled. Note that " )
			_T( "the background of the first group has changed. " )
			_T( "The status of the first group is shown in the window title bar. The " )
			_T( "first button of the second group uses color to show its state." )
			_T( "Some fonts might have been substituted by the operative system. " );
		descr = createTextBox( textBoxCS );

		textBoxCS.caption.clear();
		textBoxCS.font = createFont( _T( "Comic Sans MS" ), 24, 24 );
		txt = createTextBox( textBoxCS );

		// Group 1
		WidgetGroupBox::Seed groupBoxCS;

		// TODO: paint title correctly
		//groupBoxCS.style |= BS_OWNERDRAW;
		groupBoxCS.caption = _T( "Group 1" );
		groupBoxCS.font = createFont( _T( "Tahoma" ), 25, 15 );
		group = createGroupBox( groupBoxCS );
		group->onBackgroundColor( & RadioButGroupLayout::bgColor );
		group->onClicked( & RadioButGroupLayout::clicked );
		group->onDblClicked( & RadioButGroupLayout::dblClicked );
		group->onEnabled( & RadioButGroupLayout::enabled );
		group->onEraseBackground( & RadioButGroupLayout::erase );

		WidgetRadioButton::Seed radioButtonCS;

		radioButtonCS.caption = _T( "Radio 1 - 1" );
		radioButtonCS.font = createFont( _T( "SimHei" ), 25, 12 );
		radio1 = createRadioButton( group, radioButtonCS );
		radio1->onClicked( & RadioButGroupLayout::radioClicked );

		radioButtonCS.style |= BS_NOTIFY; // In order to get the dblClicked Event!!
		radioButtonCS.caption = _T( "Radio 2 - 1" );
		radioButtonCS.font = createFont( _T( "Nina" ), 25, 12 );
		radio2 = createRadioButton( group, radioButtonCS );
		radio2->onClicked( & RadioButGroupLayout::radioClicked );
		radio2->onDblClicked( & RadioButGroupLayout::radioDblClicked );

		// button created without using Seed has to set its properties after it has
		// been created
		radio3 = createRadioButton( group );
		radio3->setText( _T( "Radio 3 - 1" ) );
		radio3->onClicked( & RadioButGroupLayout::radioClicked );
		radio3->setEnabled( false );
		radio3->setFont( createFont( _T( "MS Gothic" ), 25, 12 ) );

		// Group 2
		WidgetGroupBox::Seed groupBoxCS2;

		groupBoxCS2.caption = _T( "Group 2" );
		groupBoxCS2.font = createFont( _T( "Kartika" ), 25, 15 );
		group2 = createGroupBox( groupBoxCS2 );

		radioButtonCS.style ^= BS_NOTIFY; // erase BS_ set before
		radioButtonCS.style |= BS_OWNERDRAW; // In order to get the painting Event!!
		radioButtonCS.caption = _T( "Radio 1 - 2" );
		radioButtonCS.font = createFont( _T( "Comic Sans MS" ), 25, 12 );
		radio21 = createRadioButton( group2, radioButtonCS );
		radio21->onClicked( & RadioButGroupLayout::radioClicked );
		radio21->onPainting( & RadioButGroupLayout::painting );

		radioButtonCS.style = WidgetRadioButton::getDefaultSeed().style;
		radioButtonCS.caption = _T( "Radio 2 - 2" );
		radioButtonCS.font = createFont( _T( "BatangChe" ), 25, 12 );
		radio22 = createRadioButton( group2, radioButtonCS );
		radio22->onClicked( & RadioButGroupLayout::radioClicked );

		radioButtonCS.caption = _T( "Radio 3 - 2" );
		radioButtonCS.font = createFont( _T( "DilleniaUPC" ), 25, 8 );
		radio23 = createRadioButton( group2, radioButtonCS );
		radio23->onClicked( & RadioButGroupLayout::radioClicked );

		// Updating

		SmartWin::Point pos( 80, 0 ), size( 600, 600 );
		setBounds( pos, size );

		size = getClientAreaSize(); // Gives (592,573)
		layout( size );

		updateWidget();
		onSized( & RadioButGroupLayout::isResized );
	}
};

// ---------------------------------------------

int SmartWinMain( Application & app )
{
	RadioButGroupLayout * testWnd2 = new RadioButGroupLayout;
	testWnd2->initAndCreate();

	return app.run();
}
