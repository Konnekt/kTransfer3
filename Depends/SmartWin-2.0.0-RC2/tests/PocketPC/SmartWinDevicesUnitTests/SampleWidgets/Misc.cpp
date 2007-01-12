// $Revision: 1.14 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "Misc.h"
#include "TabSheetWidget.h"
#include "MainWindow.h"

using namespace SmartWin;

Misc::Misc( TabSheetWidget * parent )
		: Widget( parent ),
		sw::WidgetFactory< sw::WidgetChildWindow, Misc >( parent ),
		itsParent( parent )
{}

void Misc::initAndCreate()
{
	createWindow();

	// Creating ComboBox
	itsCombo = createComboBox();
	itsCombo->onBackgroundColor( & Misc::backgroundColor );
	itsCombo->onClicked( & Misc::comboClicked );
	itsCombo->onFocus( & Misc::comboFocused );
	itsCombo->onKillFocus( & Misc::comboNotFocused );
	itsCombo->onKeyPressed( & Misc::comboKeyPressed );
	itsCombo->onMouseMove( & Misc::comboMouseMoved );
	itsCombo->addValue( _T( "Thomas" ) );
	itsCombo->addValue( _T( "Andrew" ) );
	itsCombo->addValue( _T( "Sergey" ) );
	itsCombo->addValue( _T( "Peter" ) );

	// Creating widgets
	itsDate = createDateTimePicker();
	itsDate->setFormat( _T( "yyyy-MM-dd HH:mm:ss" ) );
	itsDate->setBackgroundColor( RGB( 233, 128, 128 ) );
	itsDate->setMonthBackgroundColor( RGB( 233, 0, 233 ) );
	itsDate->setMonthTextColor( RGB( 0, 233, 0 ) );
	itsDate->setTitleBackgroundColor( RGB( 233, 0, 0 ) );
	itsDate->setTitleTextColor( RGB( 0, 128, 233 ) );
	itsDate->setTrailingTextColor( RGB( 0, 64, 115 ) );
	FontPtr font = createFont( _T( "Comic Sans" ), 18, 6, 1, ANSI_CHARSET, false, false, false, 0, OUT_DEFAULT_PRECIS );
	itsDate->setFont( font );

	// Tree View
	itsTree = createTreeView();
	itsTree->setHasButtons();
	itsTree->setHasLines();
	itsTree->setLinesAtRoot();
	TreeViewNode alpha = itsTree->insertNode( _T( "Alpha" ), TreeViewNode(), 1 );
	itsTree->insertNode( _T( "Beta" ), alpha, 2 );
	TreeViewNode circus = itsTree->insertNode( _T( "Circus" ), alpha, 3 );
	itsTree->insertNode( _T( "Delta" ), circus, 4 );
	itsTree->insertNode( _T( "Epsilon" ), alpha, 5 );
	itsTree->onClicked( & Misc::treeClicked );
	itsTree->onDblClicked( & Misc::treeDblClicked );
	itsTree->onRightClicked( & Misc::treeRightClicked );
	itsTree->onSelectionChanged( & Misc::selectionChanged );

	// Static control
	itsStatic = createStatic();
	itsStatic->setText( _T( "Yohoo!!" ) );

	itsStatic2 = createStatic();
	itsStatic2->setText( _T( "Yohoo2!!" ) );

	layout();
}

void Misc::treeClicked( WidgetTreeViewPtr tree )
{
	itsStatic->setText( SmartUtil::tstring( _T( "Tree clicked, selection: " ) ) + tree->getSelectedItemText() + _T( " item: " )
						+ boost::lexical_cast< SmartUtil::tstring >( tree->getSelectedIndex() ) );
}

void Misc::treeDblClicked( WidgetTreeViewPtr tree )
{
	itsStatic->setText( SmartUtil::tstring( _T( "Tree DOUBLE clicked, selection: " ) ) + tree->getSelectedItemText()
						+ boost::lexical_cast< SmartUtil::tstring >( tree->getSelectedIndex() ) );
}

void Misc::treeRightClicked( WidgetTreeViewPtr tree )
{
	itsStatic->setText( SmartUtil::tstring( _T( "Tree RIGHT clicked, selection: " ) ) + tree->getSelectedItemText()
						+ boost::lexical_cast< SmartUtil::tstring >( tree->getSelectedIndex() ) );
}

void Misc::selectionChanged( WidgetTreeViewPtr tree )
{
	itsStatic2->setText( SmartUtil::tstring( _T( "New selection: " ) ) +
						 tree->getSelectedItemText() + SmartUtil::tstring( _T( ", no: " ) ) + boost::lexical_cast< SmartUtil::tstring >( tree->getSelectedIndex() ) );
}

void Misc::comboClicked( WidgetComboBoxPtr cboBox )
{
	itsColorComboBox = RGB( rand()%255, 0, rand()%255 );
}

void Misc::comboFocused( WidgetComboBoxPtr cboBox )
{
	itsParent->itsParent->setText( _T( "ComboBox has focus" ) );
}

void Misc::comboNotFocused( WidgetComboBoxPtr cboBox )
{
	itsParent->itsParent->setText( _T( "ComboBox has NOT focus" ) );
}

bool Misc::comboKeyPressed( WidgetComboBoxPtr cboBox, int key )
{
	itsParent->itsParent->setText( SmartUtil::tstring( _T( "ComboBox key: " ) ) + boost::lexical_cast< SmartUtil::tstring >( key ) );
	return true;
}

void Misc::comboMouseMoved( WidgetComboBoxPtr cboBox, const MouseEventResult & mouse )
{
	itsParent->itsParent->setText( SmartUtil::tstring( _T( "ComboBox mouse: " ) ) + boost::lexical_cast< SmartUtil::tstring >( mouse.pos.x ) );
}

BrushPtr Misc::backgroundColor( WidgetComboBoxPtr cboBox, Canvas & canvas )
{
	itsBgBrushComboBox = boost::shared_ptr< sw::Brush >( new Brush( canvas, itsColorComboBox ) );
	return itsBgBrushComboBox;
}

void Misc::layout()
{
	// Resizing main client area
	sw::Rectangle rc( itsParent->getClientAreaSize() );
	setBounds( 10, 30, rc.size.x - 20, rc.size.y - 40 );

	rc = sw::Rectangle( this->getClientAreaSize() ); // lazy...
	rc = rc.getTop( 60 );

	Place place;
	place.setBoundsBorders( rc, 3, 3 );
	itsDate->setSizeAsGridPerPlace( place, 2, 1 );

	rc = sw::Rectangle( this->getClientAreaSize() ); // lazy...
	rc = rc.getTop( 120 );
	rc.pos.y = 30;
	rc.size.x = 90;
	rc.pos.x = 90;
	itsCombo->setBounds( rc );

	rc = sw::Rectangle( this->getClientAreaSize() ); // lazy...
	rc = rc.getTop( 140 );
	rc.pos.y = 60;
	rc.size.y -= 60;
	itsTree->setBounds( rc );

	rc = sw::Rectangle( this->getClientAreaSize() ); // lazy...
	rc = rc.getTop( 200 );
	rc.pos.y = 145;
	rc.size.y = 20;
	itsStatic->setBounds( rc );

	rc = sw::Rectangle( this->getClientAreaSize() ); // lazy...
	rc = rc.getTop( 200 );
	rc.pos.y = 170;
	rc.size.y = 20;
	itsStatic2->setBounds( rc );
}
