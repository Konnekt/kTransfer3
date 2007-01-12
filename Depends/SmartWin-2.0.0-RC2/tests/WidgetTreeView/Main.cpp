// $Revision: 1.12 $
/*!
  \file Main.cpp
  \brief Unit test for WidgetTreeView

  It tries to instantiate all different forms of WidgetTreeView and it also tries to use all functions.
*/

#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

// The "factory" version
class WidgetTest1
	: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	WidgetTextBoxPtr textBox;
	WidgetTreeViewPtr tree;

	void selChanged( WidgetTreeViewPtr tree )
	{
		SmartUtil::tstring item = tree->getSelectedItemText();
		textBox->setText( _T( "Clicked : " ) + item );
	}

	bool edited( WidgetTreeViewPtr tree, const SmartUtil::tstring & newValue )
	{
		textBox->setText( _T( "New value : " ) + newValue );
		return true;
	}

	void killFocus( WidgetTextBoxPtr tree )
	{
		setText( _T( "TextBox NOT focus" ) );
	}

	static void focus( WidgetTest1 * wnd, WidgetTextBoxPtr tree )
	{
		wnd->setText( _T( "TextBox focus" ) );
	}

	// TODO: why was this function here? Was it deprecated?
	//virtual void preCreate( SmartWin::Seed & cs )
	//{
	//  assert(0);
	//  cs.exStyle
	//}

	SmartWin::Rectangle doLayout( const unsigned int WidgetNo )
	{
		SmartWin::Rectangle d_Answer = SmartWin::Rectangle( getClientAreaSize() );

		d_Answer = d_Answer.shrink( 10l );
		switch ( WidgetNo )
		{
		case 0 : // textBox
			d_Answer = d_Answer.right().top().cropLeft( 5 );
			break;
		case 1 : // tree
			d_Answer = d_Answer.left().cropRight( 5 );
			break;
		}
		return d_Answer;
	}

	void sized( const SmartWin::WidgetSizedEventResult & newSize )
	{
		textBox->setBounds( doLayout( 0 ) );
		tree->setBounds( doLayout( 1 ) );
	}

	void whenCreate( const SmartWin::Seed & cs )
	{
		// Creating text box
		WidgetTextBox::Seed textBoxCS;

		textBoxCS.location = doLayout( 0 );
		textBoxCS.caption = _T( "This textbox will display the caption of your selection on the treeview." );
		textBox = createTextBox( textBoxCS );
		textBox->onKillFocus( & WidgetTest1::killFocus );
		textBox->onFocus( & WidgetTest1::focus );

		// Creating tree view image list
		// Some magic to load system folder icons,
		// of course you can use your own icons
		ImageListPtr imageList( new ImageList( 16, 16, ILC_COLOR32 | ILC_MASK ) );
		HICON hIcon = NULL;
		ExtractIconEx( _T( "shell32.dll" ), 3, NULL, & hIcon, 1 );
		imageList->add( Icon( hIcon ) );

		// Creating tree view
		WidgetTreeView::Seed widgetTreeViewCS;

		widgetTreeViewCS.font = createFont( _T( "Times New Roman" ), 0, 10, 1, ANSI_CHARSET, true, false, false, 0, OUT_DEFAULT_PRECIS );
		widgetTreeViewCS.location = doLayout( 1 );
		widgetTreeViewCS.exStyle |= WS_EX_CLIENTEDGE;
		tree = createTreeView( widgetTreeViewCS );
		tree->setEditLabels();
		tree->onSelectionChanged( & WidgetTest1::selChanged );
		tree->onValidateEditLabels( & WidgetTest1::edited );
		tree->setNormalImageList( imageList );

		// Inserting items into tree view
		TreeViewNode root = tree->insertNode( _T( "A" ), TreeViewNode(), 1, 0 );
		tree->insertNode( _T( "AA" ), root, 2, 0 );
		tree->insertNode( _T( "AB" ), root, 3, 0 );
		TreeViewNode AC = tree->insertNode( _T( "AC" ), root, 4, 0 );
		tree->insertNode( _T( "ACA" ), AC, 5 );
		tree->insertNode( _T( "ACB" ), AC, 6 );
		tree->insertNode( _T( "ACC" ), AC, 7 );
	}

public:
	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 100, 100, 350, 200 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "WidgetTreeView compile and execute test" );
		onCreate( & WidgetTest1::whenCreate );
		onSized( & WidgetTest1::sized );
		createWindow( windowCS );
	}
};

int SmartWinMain( Application & app )
{
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
