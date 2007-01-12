// $Revision: 1.10 $
#include "TabWidget.h"
#include "CentralWidget.h"

ABTabWidget::ABTabWidget( ABCentralWidget * parent )
	: Widget( parent ),
	SmartWin::WidgetFactory< SmartWin::WidgetTabSheet, ABTabWidget >( parent )
{
	create();

	// Setting initial size
	SmartWin::Rectangle rc = sw::Rectangle( parent->getClientAreaSize() );
	setBounds( 10, 10, rc.size.x - 20, 150 );

	// Adding pages up
	addPage( _T( "Add/Change Entry" ), 0 );
	addPage( _T( "Search" ), 1 );

	// Creating child widgets (actual content of pages)
	createChildWidgets();

	// Adding up some "cool looks" for the fun of it... ;)
	setHotTrack();

	// Setting event handlers for the stuff we're interested in
	onSelectionChanged( & ABTabWidget::selectedChanged );
	onSized( & ABTabWidget::sized );
}

ABTabWidget::WidgetStaticPtr ABTabWidget::createStaticHelper( WidgetChildWindowPtr parent, const SmartUtil::tstring & txt )
{
	WidgetStaticPtr tmp = parent->createStatic();
	tmp->setText( txt );
	return tmp;
}

ABTabWidget::WidgetTextBoxPtr ABTabWidget::createTextBoxHelper( WidgetChildWindowPtr parent, WidgetTextBox::Seed & cs )
{
	WidgetTextBoxPtr tmp = parent->createTextBox( cs );
	tmp->setBorder();
	return tmp;
}

ABTabWidget::WidgetCheckBoxPtr ABTabWidget::createCheckBoxHelper( WidgetChildWindowPtr parent, WidgetCheckBox::Seed & cs, const SmartUtil::tstring & txt )
{
	WidgetCheckBoxPtr tmp = parent->createCheckBox( cs );
	tmp->setText( txt );
	return tmp;
}

void ABTabWidget::createChildWidgets()
{
	WidgetTextBox::Seed textSeed;
	textSeed.style |= WS_TABSTOP | ES_AUTOHSCROLL;
	textSeed.exStyle |= WS_EX_CLIENTEDGE;

	// First the Add/Change Entry
	addChange = createWidgetChildWindow();

	firstName = createStaticHelper( addChange, _T( "First Name" ) );
	txtFirstName = createTextBoxHelper( addChange, textSeed );
	txtFirstName->setFocus();

	lastName = createStaticHelper( addChange, _T( "Last Name" ) );
	txtLastName = createTextBoxHelper( addChange, textSeed );

	address = createStaticHelper( addChange, _T( "Adr" ) );
	txtAddress = createTextBoxHelper( addChange, textSeed );

	email = createStaticHelper( addChange, _T( "E-Mail" ) );
	txtEmail = createTextBoxHelper( addChange, textSeed );

	explanation = createStaticHelper( addChange, _T( "Add new entry" ) );

	WidgetButton::Seed buttonSeed;
	buttonSeed.style |= WS_TABSTOP;
	buttonSeed.exStyle |= WS_EX_STATICEDGE;

	btnAdd = addChange->createButton( buttonSeed );
	btnAdd->setText( _T( "&Add" ) );
	btnAdd->onClicked( & ABTabWidget::addFunc );

	// MARK!
	// Since WE (SmartWin++ users) don't need the Change button (grid is editable) we don't add it up neither!!
	// In the Qt sample there's a "Change" button...
	// ;)

	// Then the Search
	search = createWidgetChildWindow();

	WidgetCheckBox::Seed checkSeed;
	checkSeed.style |= WS_TABSTOP;

	chkFirstName = createCheckBoxHelper( search, checkSeed, _T( "First Name" ) );
	txtSearchFirstName = createTextBoxHelper( search, textSeed );

	chkLastName = createCheckBoxHelper( search, checkSeed, _T( "Last Name" ) );
	txtSearchLastName = createTextBoxHelper( search, textSeed );

	chkAddress = createCheckBoxHelper( search, checkSeed, _T( "Adr" ) );
	txtSearchAddress = createTextBoxHelper( search, textSeed );

	chkEmail = createCheckBoxHelper( search, checkSeed, _T( "Mail" ) );
	txtSearchEmail = createTextBoxHelper( search, textSeed );

	btnSearch = search->createButton( buttonSeed );
	btnSearch->setText( _T( "&Search" ) );
	btnSearch->onClicked( & ABTabWidget::searchFunc );

	searchExplenation = createStaticHelper( search, _T( "Search for items" ) );

	// Setting to INVISIBLE initially
	search->setVisible( false );

	// Setting initial sizes...
	layout();
}

void ABTabWidget::layout()
{
	SmartWin::Rectangle clientArea = sw::Rectangle( getClientAreaSize() );
	addChange->setBounds( 5, 30, clientArea.size.x - 10, clientArea.size.y - 34 );
	search->setBounds( 5, 30, clientArea.size.x - 10, clientArea.size.y - 34 );

	// First the Add New Item page
	SmartWin::Place pAdd;
	pAdd.setBoundsBorders(
		addChange->getClientAreaSize(), // Current client area size.
		2,  // X border between widgets
		4   // Y border
	);

	// The addNewEntry and the search page both has 2 rows and 5 columns
	int rows = 2;
	int cols = 5;

	// The first row.
	firstName->setSizeAsGridPerPlace( pAdd, rows, cols );
	lastName->setSizeAsGridPerPlace( pAdd, rows, cols );
	address->setSizeAsGridPerPlace( pAdd, rows, cols );
	email->setSizeAsGridPerPlace( pAdd, rows, cols );
	explanation->setSizeAsGridPerPlace( pAdd, rows, cols );

	// Second row
	txtFirstName->setSizeAsGridPerPlace( pAdd, rows, cols );
	txtLastName->setSizeAsGridPerPlace( pAdd, rows, cols );
	txtAddress->setSizeAsGridPerPlace( pAdd, rows, cols );
	txtEmail->setSizeAsGridPerPlace( pAdd, rows, cols );
	btnAdd->setSizeAsGridPerPlace( pAdd, rows, cols );

	// Then the Search Item page
	SmartWin::Place pSearch;
	pSearch.setBoundsBorders(
		search->getClientAreaSize(), // Current client area size.
		2,  // X border between widgets
		4   // Y border
	);

	// The first row.
	chkFirstName->setSizeAsGridPerPlace( pSearch, rows, cols );
	chkLastName->setSizeAsGridPerPlace( pSearch, rows, cols );
	chkAddress->setSizeAsGridPerPlace( pSearch, rows, cols );
	chkEmail->setSizeAsGridPerPlace( pSearch, rows, cols );
	searchExplenation->setSizeAsGridPerPlace( pSearch, rows, cols );

	// Second row
	txtSearchFirstName->setSizeAsGridPerPlace( pSearch, rows, cols );
	txtSearchLastName->setSizeAsGridPerPlace( pSearch, rows, cols );
	txtSearchAddress->setSizeAsGridPerPlace( pSearch, rows, cols );
	txtSearchEmail->setSizeAsGridPerPlace( pSearch, rows, cols );
	btnSearch->setSizeAsGridPerPlace( pSearch, rows, cols );
}

void ABTabWidget::selectedChanged( WidgetTabSheetPtr This )
{
	// Remember the
	switch ( getSelectedIndex() )
	{
	case 0 :
		// Add/Change Entry
		addChange->setVisible( true );
		search->setVisible( false );
		txtFirstName->setFocus();
		break;
	case 1 :
		// Search
		addChange->setVisible( false );
		search->setVisible( true );
		txtSearchFirstName->setFocus();
		break;
	}
}

void ABTabWidget::sized( WidgetTabSheetPtr This, const SmartWin::WidgetSizedEventResult & rc )
{
	layout();
}

void ABTabWidget::addFunc( WidgetButtonPtr btn )
{
	ABCentralWidget * parent = dynamic_cast< ABCentralWidget * >( getParent() );
	parent->add( txtFirstName->getText(), txtLastName->getText(), txtAddress->getText(), txtEmail->getText() );
}

void ABTabWidget::searchFunc( WidgetButtonPtr btn )
{
	SmartUtil::tstring firstName = chkFirstName->getChecked() ? txtSearchFirstName->getText() : _T( "" );
	SmartUtil::tstring lastName = chkLastName->getChecked() ? txtSearchLastName->getText() : _T( "" );
	SmartUtil::tstring address = chkAddress->getChecked() ? txtAddress->getText() : _T( "" );
	SmartUtil::tstring email = chkEmail->getChecked() ? txtEmail->getText() : _T( "" );

	ABCentralWidget * parent = dynamic_cast< ABCentralWidget * >( getParent() );
	parent->search( firstName, lastName, address, email );
}
