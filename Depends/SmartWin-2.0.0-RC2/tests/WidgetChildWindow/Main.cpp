// $Revision: 1.7 $

#include "SmartWin.h"

using namespace SmartWin;

class MyWidget : public WidgetFactory< WidgetWindow, MyWidget >
{
private:
	WidgetChildWindowPtr containerChild;
	WidgetTextBoxPtr childOfContainerWindow;
public:

	// Note!
	// Since the MyWidget class is the event handler of the containerChild Widget (and must be) the passed Widget (first argument) will be the MyWidget and NOT
	// the containerChild widget...
	// This might look a bit peculiar compared to what it would look like if we derived from the WidgetFactory and created a new Widget which we embedded
	// inside the MyWidget...
	static void painting( MyWidget * containerChild, Canvas & canvas )
	{
		Brush bgBrush( canvas, RGB( 255, 0, 0 ) );
		canvas.fillRectangle( sw::Rectangle( containerChild->getSize() ), bgBrush );
	}

	void init()
	{
		// First creating outer most container widget (main window)
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 100, 100, 200, 200 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "Testing Container widgets being child of another container widget" );
		createWindow( windowCS );

		// Then we create another container widget which is a child of the main window
		containerChild = createWidgetChildWindow();
		containerChild->setBounds( 10, 10, 100, 100 );
		containerChild->onPainting( & MyWidget::painting );

		// Then when we create control widgets which are supposed to be inside the inner container widget we must create them "through" the container widget
		childOfContainerWindow = containerChild->createTextBox();
		childOfContainerWindow->setBounds( 10, 10, 50, 20 );
		childOfContainerWindow->setText( _T( "testing" ) );
	}
};

int SmartWinMain( Application & app )
{
	MyWidget * x = new MyWidget();
	x->init();
	return app.run();
}
