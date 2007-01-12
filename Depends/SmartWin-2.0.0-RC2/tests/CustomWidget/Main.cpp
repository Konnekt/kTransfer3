// $Revision: 1.13 $

#include "SmartWin.h"
#include <vector>
#include <assert.h>
using namespace SmartWin;

// This class represents a "generic Widget" which can be reused in several projects
template< class Parent >
class WidgetCustom :
	// We want resizing capabilities
	public AspectSizable< WidgetCustom< Parent >, WidgetCustom< Parent >, MessageMap< WidgetCustom< Parent >, MessageMapPolicyNormalWidget > >,

	// We HAVE to expose this one by public inheritence since aspectsizable depends
	// on public access to Widget base class...
	public MessageMap< WidgetCustom< Parent >, MessageMapPolicyNormalWidget >,

	// We would prefer PRIVATE inheritence here, but since gcc fails in a
	// dynamic_cast when we later tries to cast to MessageMap... we CAN'T use
	// private inheritence!
	public AspectEraseBackground< WidgetCustom< Parent >, WidgetCustom< Parent >, MessageMap< WidgetCustom< Parent >, MessageMapPolicyNormalWidget > >
{
public:
	typedef void ( Parent::* itsVoidFunctionVoid ) ();

	class Seed
		: public SmartWin::Seed
	{
	public:
		typedef WidgetCustom< Parent > WidgetType;

		COLORREF color;

		// explicit to avoid conversion through SmartWin::CreationalStruct
		explicit Seed();

		/// Doesn't fill any values
		Seed( DontInitialize )
		{}
	};

	/// Default values for creation. Also registration.
	static const Seed & getDefaultSeed()
	{
		static bool d_NeedsInit = true;
		static Seed d_DefaultValues( DontInitializeMe );

		if ( d_NeedsInit )
		{
			Application::instance().generateLocalClassName( d_DefaultValues );
			d_DefaultValues.style |= WS_CHILD;
			d_DefaultValues.color = RGB( 180, 70, 0 );

			WNDCLASSEX wc;
			wc.cbSize = sizeof( WNDCLASSEX );
			wc.style = CS_VREDRAW | CS_HREDRAW;
			wc.lpfnWndProc = & MessageMapPolicyNormalWidget::mainWndProc_;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = Application::instance().getAppHandle();
			wc.hIcon = LoadIcon( 0, IDI_APPLICATION );
			wc.hCursor = LoadCursor( 0, IDC_ARROW );
			wc.hbrBackground = ( HBRUSH ) ( COLOR_BTNTEXT );
			wc.lpszMenuName = 0;
			wc.lpszClassName = d_DefaultValues.getClassName().c_str();
			wc.hIconSm = LoadIcon( 0, IDI_APPLICATION );

			ATOM registeredClass = ::RegisterClassEx( & wc );
			if ( 0 == registeredClass )
			{
				xCeption x( _T( "WidgetWindowBase.createWindow() ::RegisterClassEx fizzled..." ) );
				throw x;
			}
			Application::instance().addLocalWindowClassToUnregister( d_DefaultValues );
			d_NeedsInit = false;
		}
		return d_DefaultValues;
	}

	static WidgetCustom< Parent > * createCustom( Parent * dad, const Seed & cs = getDefaultSeed() )
	{
		WidgetCustom< Parent > * d_ReturnValue = new WidgetCustom< Parent >( dad, cs.color );
		d_ReturnValue->create( cs );

		// Here we're USING and EXISTING Aspect, though since we derive PRIVATELY
		// from AspectEraseBackgroundd this will NOT be EXPOSED to derived classes
		// or classes using this class through composition
		d_ReturnValue->onEraseBackground( & WidgetCustom::eraseBackground );

		// Instead of doing it the "hard way" like this, I could probably have
		// reused the AspectClickable instead but I want to demostrate how to add
		// new signals and handle them OUTSIDE the library! The message actually
		// handled here could have been ANYTHING, also messages not at all existing
		// in the library and even custom created messages...
		d_ReturnValue->addNewSignal(
			typename ThisMessageMap::SignalTupleType(
				private_::SignalContent(
					Message( WM_LBUTTONDOWN ),
					reinterpret_cast< itsVoidFunction >( & WidgetCustom::areaClicked ),
					d_ReturnValue
				),
				typename ThisMessageMap::SignalType(
					typename ThisMessageMap::SignalType::SlotType( & DispatcherClick::dispatchThis )
				)
			)
		);

		return d_ReturnValue;
	}
private:

	typedef MessageMap< WidgetCustom< Parent >, MessageMapPolicyNormalWidget > ThisMessageMap;

	COLORREF itsColorRef;

	// Dispatcher class for handling "clicks" and our Custom Event...
	class DispatcherClick
	{
	public:
		static HRESULT dispatchThis( private_::SignalContent & params )
		{
			ThisMessageMap * This = boost::polymorphic_cast< ThisMessageMap * >( params.This );
			typename ThisMessageMap::itsVoidFunctionTakingVoid func =
				reinterpret_cast< typename ThisMessageMap::itsVoidFunctionTakingVoid >( params.FunctionThis );

			( ( * boost::polymorphic_cast< WidgetCustom< Parent > * >( params.This ) ).* func ) (
			);

			return 1;
		}

		static HRESULT dispatchThisHowdy( private_::SignalContent & params )
		{
			ThisMessageMap * This = boost::polymorphic_cast< ThisMessageMap * >( params.This );
			itsVoidFunctionVoid func =
				reinterpret_cast< itsVoidFunctionVoid >( params.FunctionThis );

			( ( * boost::polymorphic_cast< Parent * >( params.This->getParent() ) ).* func ) (
			);

			return 1;
		}
	};

	void areaClicked()
	{
		itsColorRef = RGB( rand() % 255, rand() % 255, rand() % 255 );
		this->updateWidget();
	}

	void eraseBackground( Canvas & canvas )
	{
		// Control is "transparent"
		canvas.setBkMode( true );

		// Getting client area size
		SmartWin::Rectangle rc( this->getSize() );

		// Making a dark red circle
		Pen pen( canvas, RGB( 100, 0, 0 ) );
		canvas.ellipse( 0, 0, rc.size.x, rc.size.y );
		canvas.ellipse( 5, 5, rc.size.x - 5, rc.size.y - 5 );
		Brush brushBorder( canvas, RGB( 100, 0, 0 ) );
		canvas.extFloodFill( rc.size.x / 2, 2, RGB( 100, 0, 0 ), true );

		// Filling our circle with a bit lighter reddish color
		Brush brushInterior( canvas, itsColorRef );
		canvas.extFloodFill( rc.size.x / 2, rc.size.y / 2, RGB( 100, 0, 0 ), true );

		// Drawing text
		rc.pos.x = 0;
		rc.pos.y = 50;
		rc.shrinkHeight( 50 );
		rc.shrinkWidth( 10 );
		Brush brushTxt( canvas, RGB( 0, 0, 0 ) );
		canvas.drawText( _T( "Click me!\r\nWith both mouse buttons" ), rc, DT_CENTER );
	}
public:

	// This is an "event handler setter" which will expect to get a member function
	// function passed of the type of the "Parent" class
	void onHowdy( itsVoidFunctionVoid function )
	{
		// I'm just using the RBUTTONDOWN here, but I could have handled ANY type
		// of message, even custom made messages thanx to the internally used
		// SigSlot library!
		addNewSignal(
			typename ThisMessageMap::SignalTupleType(
				private_::SignalContent(
					Message( WM_RBUTTONDOWN ),
					reinterpret_cast< itsVoidFunction >( function ),
					this
				),
				typename ThisMessageMap::SignalType(
					typename ThisMessageMap::SignalType::SlotType( & DispatcherClick::dispatchThisHowdy )
				)
			)
		);
	}

protected:
	// protected to force the use of createCustom.
	explicit WidgetCustom( SmartWin::Widget * parent, const COLORREF & a_Color )
		: Widget( parent ), itsColorRef( a_Color )
	{}

	// When creating Widgets it's good practise to make DTOR protected to disable
	// "stack creation" of them since they destroy themself in the WM_DESTROY
	// message
	virtual ~WidgetCustom()
	{}
}
;

template< class Parent >
WidgetCustom< Parent >::Seed::Seed()
{
	* this = WidgetCustom< Parent >::getDefaultSeed();
}

// The "factory" version
class WidgetTest1
			: public WidgetFactory< WidgetWindow, WidgetTest1 >
{
	typedef WidgetCustom< WidgetTest1 > WidgetCustom;
	WidgetCustom * itsCustom;

	SmartUtil::tstring itsHowdy;

	void howdy()
	{
		if ( itsHowdy == _T( "" ) )
		{
			createMessageBox().show( _T( "Watch the title bar of the main Widget!" ) );
		}
		itsHowdy += _T( "Howdy, " );
		setText( itsHowdy );
	}

public:
	WidgetTest1()
	{}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;

		windowCS.location = SmartWin::Rectangle( 10, 50, 300, 300 );
		windowCS.background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.caption = _T( "CustomWidget compile and execute test" );
		createWindow( windowCS );

		// Creating custom Widget
		WidgetCustom::Seed customSeed;

		customSeed.location = SmartWin::Rectangle( 50, 50, 200, 200 );
		itsCustom = WidgetCustom::createCustom( this, customSeed );
		itsCustom->onHowdy( & WidgetTest1::howdy );
	}
};

int SmartWinMain( Application & app )
{
	// Widget (Factory) version
	WidgetTest1 * testWnd1 = new WidgetTest1;
	testWnd1->initAndCreate();
	return app.run();
}
