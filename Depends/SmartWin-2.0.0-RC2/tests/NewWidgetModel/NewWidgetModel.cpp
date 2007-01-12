// $Revision: 1.2 $
/// \file NewWidgetModel
///
/// This file contains a guide to create new widgets that could be included in the
/// factory. It shows that the new widget should
///
///     - include WidgetCreator as friend
///     - define ThisType and ObjectType
///     - use Seed to pass construction parameters
///     - define/overload create( const Seed & )
///     - call Widget(parent) in the constructor
///
/// In the factory, the following actions are needed:
///
///     - define the class type and the object type
///     - implement the function createWidgetXXX
/// ++Hector C.
#include <SmartWin.h>
using namespace SmartWin;

template< class Parent, class WidgetMessageMapType >
class ColoredStatic
  : public WidgetStatic< Parent, WidgetMessageMapType >
{
	/// WidgetCreator
	friend class WidgetCreator< ColoredStatic >;
public:
	/// Class type
	typedef ColoredStatic< Parent, WidgetMessageMapType > ThisType;

	/// Object type
	typedef ColoredStatic< Parent, WidgetMessageMapType > * ObjectType;

	/// Parameters for construction
	class Seed : public WidgetStatic< Parent, WidgetMessageMapType >::Seed
	{
	public:
		COLORREF m_Backgrnd;

		explicit Seed()
		{ * this = getDefaultSeed();
		}

		Seed( const COLORREF a_Backgrnd ) : m_Backgrnd( a_Backgrnd )
		{
			WidgetStatic< Parent, WidgetMessageMapType >::getDefaultSeed().cloneInto( * this );
		}
	};

	static const Seed & getDefaultSeed()
	{
		// TODO: get default background color from themes
		static Seed d_( RGB( 127, 127, 127 ) );
		return d_;
	}

	virtual void create( const Seed & = getDefaultSeed() );

	static BrushPtr bkgrColor( Parent *, WidgetStatic< Parent, WidgetMessageMapType > *, Canvas & canvas );
protected:
	BrushPtr m_Brush;
	explicit ColoredStatic( SmartWin::Widget * parent );
};

template< class Parent, class WidgetMessageMapType >
ColoredStatic< Parent, WidgetMessageMapType >::ColoredStatic( SmartWin::Widget * parent )
 : Widget( parent )
, WidgetStatic< Parent, WidgetMessageMapType >( parent )
{}

template< class Parent, class WidgetMessageMapType >
BrushPtr ColoredStatic< Parent, WidgetMessageMapType >::bkgrColor( Parent *, WidgetStatic< Parent, WidgetMessageMapType > * x_This, Canvas & canvas )
{
	canvas.setBkMode( true );
	ColoredStatic< Parent, WidgetMessageMapType > * d_TS = reinterpret_cast< ColoredStatic< Parent, WidgetMessageMapType > * >( x_This );
	return d_TS->m_Brush;
}

template< class Parent, class WidgetMessageMapType >
void ColoredStatic< Parent, WidgetMessageMapType >::create( const Seed & cs )
{
	UpdateCanvas d_ParentCanvas( itsParent );
	m_Brush = BrushPtr( new Brush( d_ParentCanvas, cs.m_Backgrnd ) );
	onBackgroundColor( bkgrColor );
	WidgetStatic< Parent, WidgetMessageMapType >::create( cs );
}

template< template< class, class > class Base, class Parent, class WidgetMessageMapType = NormalWidget >
class MyFactory
	: public WidgetFactory< Base, Parent, WidgetMessageMapType >
{
public:
  /// StatusBar class type.
  typedef ColoredStatic< Parent, WidgetMessageMapType > ColoredStatic;

  /// StatusBar object type.
  typedef ColoredStatic * ColoredStaticPtr;

  ColoredStaticPtr createColoredStatic( const typename ColoredStatic::Seed & = ColoredStatic::getDefaultSeed() );
};

template< template< class, class > class Base, class Parent, class WidgetMessageMapType >
typename MyFactory< Base, Parent, WidgetMessageMapType >::ColoredStaticPtr
MyFactory< Base, Parent, WidgetMessageMapType >::createColoredStatic( const typename ColoredStatic::Seed & cs )
{
  return WidgetCreator< ColoredStatic >::create( this, cs );
}

class WidgetExample
	: public MyFactory< WidgetWindow, WidgetExample >
{
private:
	ColoredStaticPtr m_Message;
public:
	void menuEventHandler( WidgetMenuPtr menu, unsigned item )
	{
		return;
	}

	void initAndCreate()
	{
		WidgetWindow::Seed windowCS;
		windowCS.rect.size = SmartWin::Point( 130, 100 );
		windowCS.Background = CreateSolidBrush( RGB( 0, 0, 125 ) );
		windowCS.Caption = _T( "Colored Static" );
		createWindow( windowCS );

		ColoredStatic::Seed colorStaticCS;
		colorStaticCS.InitialText = _T( "Now you see me painted in a different color." );
		colorStaticCS.rect = sw::Rectangle( 10, 10, 100, 40 );
		colorStaticCS.m_Backgrnd = COLORREF( RGB( 255, 255, 125 ) );
		m_Message = createColoredStatic( colorStaticCS );
	}
};

int SmartWinMain( Application & app )
{
	// Widget (MyFactory) version
	WidgetExample * Wnd = new WidgetExample;
	Wnd->initAndCreate();
	return app.run();
}
