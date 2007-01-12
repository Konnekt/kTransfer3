// $Revision: 1.13 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "TabWidget.h"
#include "CentralWidget.h"

ABTabWidget::ABTabWidget( ABCentralWidget * parent )
		: Widget( parent ),
		sw::WidgetFactory< sw::WidgetTabSheet, ABTabWidget >( parent )
{
	create();

	// Setting initial size
	sw::Rectangle rc( parent->getClientAreaSize() );
	setBounds( 10, 10, rc.size.x - 20, 150 );

	// Adding pages up
	addPage( _T( "Add/Change Entry" ), 0 );
	addPage( _T( "Search" ), 1 );

	// Creating child Widgets (actual content of pages)
	createChildWidgets();

	// Adding some "cool looks" for the fun of it... ;)
	setHotTrack();

	// Setting event handlers for the stuff we're interested in
	onSelectionChanged( & ABTabWidget::selectedChanged );
	onSized( & ABTabWidget::sized );
}

ABTabWidget::WidgetStaticPtr ABTabWidget::createStaticHelper( WidgetChildWindowPtr parent, sw::FontPtr & font, const SmartUtil::tstring & txt )
{
	WidgetStaticPtr tmp = parent->createStatic();
	tmp->setFont( font );
	tmp->setText( txt );
	return tmp;
}

ABTabWidget::WidgetTextBoxPtr ABTabWidget::createTextBoxHelper( WidgetChildWindowPtr parent, sw::FontPtr & font, WidgetTextBox::Seed & cs )
{
	WidgetTextBoxPtr tmp = parent->createTextBox( cs );
	tmp->setFont( font );
	tmp->setBorder();
	return tmp;
}

ABTabWidget::WidgetCheckBoxPtr ABTabWidget::createCheckBoxHelper( WidgetChildWindowPtr parent, sw::FontPtr & font, WidgetCheckBox::Seed & cs, const SmartUtil::tstring & txt )
{
	WidgetCheckBoxPtr tmp = parent->createCheckBox( cs );
	tmp->setFont( font );
	tmp->setText( txt );
	return tmp;
}

void ABTabWidget::createChildWidgets()
{
	// Creating a font to use...
	sw::FontPtr fontTxt = sw::createFont( sw::DefaultGuiFont );

	WidgetTextBox::Seed textBoxCS;

	textBoxCS.style |= WS_TABSTOP | ES_AUTOHSCROLL;
	textBoxCS.exStyle |= WS_EX_CLIENTEDGE;

	// First the Add/Change Entry
	itsAddChange = createWidgetChildWindow();

	itsFirstName = createStaticHelper( itsAddChange, fontTxt, _T( "First Name" ) );
	itsTxtFirstName = createTextBoxHelper( itsAddChange, fontTxt, textBoxCS );
	itsTxtFirstName->setFocus();

	itsLastName = createStaticHelper( itsAddChange, fontTxt, _T( "Last Name" ) );
	itsTxtLastName = createTextBoxHelper( itsAddChange, fontTxt, textBoxCS );

	itsAddress = createStaticHelper( itsAddChange, fontTxt, _T( "Address" ) );
	itsTxtAddress = createTextBoxHelper( itsAddChange, fontTxt, textBoxCS );

	itsEmail = createStaticHelper( itsAddChange, fontTxt, _T( "E-Mail" ) );
	itsTxtEmail = createTextBoxHelper( itsAddChange, fontTxt, textBoxCS );

	itsExplanation = createStaticHelper( itsAddChange, fontTxt, _T( "Add new entry" ) );

	WidgetButton::Seed buttonCS;

	buttonCS.style |= WS_TABSTOP;
	buttonCS.exStyle |= WS_EX_STATICEDGE;
	buttonCS.caption = _T( "&Add" );
	itsBtnAdd = itsAddChange->createButton( buttonCS );
	itsBtnAdd->onClicked( & ABTabWidget::addFunc );

	// Note!
	// Since WE (SmartWin++ users) don't need the Change button (grid is editable) we don't add it up neither!!
	// In the Qt sample there's a "Change" button...
	// ;)

	// Then the Search
	itsSearch = createWidgetChildWindow();

	textBoxCS = WidgetTextBox::getDefaultSeed();
	textBoxCS.style |= WS_TABSTOP | ES_AUTOHSCROLL;
	textBoxCS.exStyle |= WS_EX_CLIENTEDGE;

	WidgetCheckBox::Seed checkBoxCS;

	checkBoxCS.style |= WS_TABSTOP | BS_AUTOCHECKBOX | BS_CHECKBOX;

	itsChkFirstName = createCheckBoxHelper( itsSearch, fontTxt, checkBoxCS, _T( "First Name" ) );
	itsTxtSearchFirstName = createTextBoxHelper( itsSearch, fontTxt, textBoxCS );

	itsChkLastName = createCheckBoxHelper( itsSearch, fontTxt, checkBoxCS, _T( "Last Name" ) );
	itsTxtSearchLastName = createTextBoxHelper( itsSearch, fontTxt, textBoxCS );

	itsChkAddress = createCheckBoxHelper( itsSearch, fontTxt, checkBoxCS, _T( "Address" ) );
	itsTxtSearchAddress = createTextBoxHelper( itsSearch, fontTxt, textBoxCS );

	itsChkEmail = createCheckBoxHelper( itsSearch, fontTxt, checkBoxCS, _T( "E-Mail" ) );
	itsTxtSearchEmail = createTextBoxHelper( itsSearch, fontTxt, textBoxCS );

	buttonCS.caption = _T( "&Search" );
	itsBtnSearch = itsSearch->createButton( buttonCS );
	itsBtnSearch->onClicked( & ABTabWidget::searchFunc );

	itsSearchExplanation = createStaticHelper( itsSearch, fontTxt, _T( "Search for items" ) );

	// Setting to INVISIBLE initially
	itsSearch->setVisible( false );

	// Setting initial sizes...
	layout();
}

void ABTabWidget::layout()
{
	sw::Rectangle clientArea( getClientAreaSize() );
	itsAddChange->setBounds( 5, 30, clientArea.size.x - 10, clientArea.size.y - 34 );
	itsSearch->setBounds( 5, 30, clientArea.size.x - 10, clientArea.size.y - 34 );

	// First the Add New Item page
	sw::Place pAdd;
	pAdd.setBoundsBorders(
		itsAddChange->getClientAreaSize(),   // Current client area size.
		4,      // X border between Widgets
		8   // Y border
	);

	// The addNewEntry and the search page both has 2 rows and 5 columns
	int rows = 2;
	int cols = 5;

	// The first row.
	itsFirstName->setSizeAsGridPerPlace( pAdd, rows, cols );
	itsLastName->setSizeAsGridPerPlace( pAdd, rows, cols );
	itsAddress->setSizeAsGridPerPlace( pAdd, rows, cols );
	itsEmail->setSizeAsGridPerPlace( pAdd, rows, cols );
	itsExplanation->setSizeAsGridPerPlace( pAdd, rows, cols );

	// Second row
	itsTxtFirstName->setSizeAsGridPerPlace( pAdd, rows, cols );
	itsTxtLastName->setSizeAsGridPerPlace( pAdd, rows, cols );
	itsTxtAddress->setSizeAsGridPerPlace( pAdd, rows, cols );
	itsTxtEmail->setSizeAsGridPerPlace( pAdd, rows, cols );
	itsBtnAdd->setSizeAsGridPerPlace( pAdd, rows, cols );

	// Then the Search Item page
	sw::Place pSearch;
	pSearch.setBoundsBorders(
		itsSearch->getClientAreaSize(),   // Current client area size.
		4,      // X border between Widgets
		8   // Y border
	);

	// The first row.
	itsChkFirstName->setSizeAsGridPerPlace( pSearch, rows, cols );
	itsChkLastName->setSizeAsGridPerPlace( pSearch, rows, cols );
	itsChkAddress->setSizeAsGridPerPlace( pSearch, rows, cols );
	itsChkEmail->setSizeAsGridPerPlace( pSearch, rows, cols );
	itsSearchExplanation->setSizeAsGridPerPlace( pSearch, rows, cols );

	// Second row
	itsTxtSearchFirstName->setSizeAsGridPerPlace( pSearch, rows, cols );
	itsTxtSearchLastName->setSizeAsGridPerPlace( pSearch, rows, cols );
	itsTxtSearchAddress->setSizeAsGridPerPlace( pSearch, rows, cols );
	itsTxtSearchEmail->setSizeAsGridPerPlace( pSearch, rows, cols );
	itsBtnSearch->setSizeAsGridPerPlace( pSearch, rows, cols );
}

void ABTabWidget::selectedChanged( WidgetTabSheetPtr This )
{
	// Remember the
	switch ( getSelectedIndex() )
	{
		case 0 :
			// Add/Change Entry
			itsAddChange->setVisible( true );
			itsSearch->setVisible( false );
			itsTxtFirstName->setFocus();
			break;
		case 1 :
			// Search
			itsAddChange->setVisible( false );
			itsSearch->setVisible( true );
			itsTxtSearchFirstName->setFocus();
			break;
	}
}

void ABTabWidget::sized( WidgetTabSheetPtr This, const sw::WidgetSizedEventResult & rc )
{
	layout();
}

void ABTabWidget::addFunc( WidgetButtonPtr btn )
{
	ABCentralWidget * parent = dynamic_cast< ABCentralWidget * >( getParent() );
	parent->add( itsTxtFirstName->getText(), itsTxtLastName->getText(), itsTxtAddress->getText(), itsTxtEmail->getText() );
}

void ABTabWidget::searchFunc( WidgetButtonPtr btn )
{
	SmartUtil::tstring firstName = itsChkFirstName->getChecked() ? itsTxtSearchFirstName->getText() : _T( "" );
	SmartUtil::tstring lastName = itsChkLastName->getChecked() ? itsTxtSearchLastName->getText() : _T( "" );
	SmartUtil::tstring address = itsChkAddress->getChecked() ? itsTxtSearchAddress->getText() : _T( "" );
	SmartUtil::tstring email = itsChkEmail->getChecked() ? itsTxtSearchEmail->getText() : _T( "" );

	ABCentralWidget * parent = dynamic_cast< ABCentralWidget * >( getParent() );
	parent->search( firstName, lastName, address, email );
}
