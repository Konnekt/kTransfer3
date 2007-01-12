// $Revision: 1.15 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "TabSheetWidget.h"
#include "MainWindow.h"
#include "DataGrid.h"
#include "Buttons.h"
#include "Sliders.h"
#include "Misc.h"
using namespace SmartWin;

TabSheetWidget::TabSheetWidget( MainWindow * parent )
		: Widget( parent ),
		sw::WidgetFactory< sw::WidgetTabSheet, TabSheetWidget >( parent ),
		itsParent( parent )
{}

void TabSheetWidget::selectedChanged( WidgetTabSheetPtr This )
{
	switch ( getSelectedIndex() )
	{
	case 0 :
		// DataGrid
		itsDataGrid->setVisible( true );
		itsButtons->setVisible( false );
		itsSliders->setVisible( false );
		itsMisc->setVisible( false );
		break;
	case 1 :
		// Buttons
		itsDataGrid->setVisible( false );
		itsButtons->setVisible( true );
		itsSliders->setVisible( false );
		itsMisc->setVisible( false );
		break;
	case 2 :
		// Sliders
		itsDataGrid->setVisible( false );
		itsButtons->setVisible( false );
		itsSliders->setVisible( true );
		itsMisc->setVisible( false );
		break;
	case 3 :
		// Misc
		itsDataGrid->setVisible( false );
		itsButtons->setVisible( false );
		itsSliders->setVisible( false );
		itsMisc->setVisible( true );
		break;
	}
}

void TabSheetWidget::initAndCreate()
{
	// Creating and initializing
	create();
	setHotTrack();
	sw::Rectangle rc( itsParent->getClientAreaSize() );
	setBounds( 10, 10, rc.size.x - 20, rc.size.y - 20 );
	onSelectionChanged( & TabSheetWidget::selectedChanged );

	// Adding pages
	addPage( _T( "DataGrid" ), 0 );
	addPage( _T( "Buttons" ), 1 );
	addPage( _T( "Sliders" ), 2 );
	addPage( _T( "Misc" ), 3 );

	// Creating the DataGrid item
	itsDataGrid = new DataGrid( this );
	itsDataGrid->initAndCreate();

	// Creating the Buttons tab
	itsButtons = new Buttons( this );
	itsButtons->initAndCreate();

	// Creating Sliders tab
	itsSliders = new Sliders( this );
	itsSliders->initAndCreate();

	// Creating Misc tab
	itsMisc = new Misc( this );
	itsMisc->initAndCreate();

	selectedChanged( 0 );
}

void TabSheetWidget::layout()
{
	sw::Rectangle rc( itsParent->getClientAreaSize() );
	setBounds( 10, 10, rc.size.x - 20, rc.size.y - 20 );
	itsDataGrid->layout();
	itsButtons->layout();
	itsSliders->layout();
	itsMisc->layout();
}
