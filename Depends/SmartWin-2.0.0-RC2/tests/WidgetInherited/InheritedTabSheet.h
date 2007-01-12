// $Revision: 1.4 $
#include "SmartWin.h"
using namespace SmartWin;

class WidgetTest1;

template< class Parent >
class InheritedTabSheet
	: public WidgetTabSheet< Parent, SmartWin::MessageMapPolicyNormalWidget >
{
	Parent * itsParent;
public:
	InheritedTabSheet( WidgetTest1 * parent )
		: Widget( parent ),
		WidgetTabSheet< Parent, SmartWin::MessageMapPolicyNormalWidget >( parent ),
		itsParent( parent )
	{}

	void init()
	{
		this->create();
		this->addPage( _T( "Main" ), 0 );
		this->addPage( _T( "Extra" ), 1 );
		layout();
	}

	void layout()
	{
		sw::Rectangle rc( itsParent->getSize() );
		rc = rc.shrink( 20, 20 );
		rc.size.y -= 30; // Accomodate for Border and stuff
		this->setBounds( rc );
	}
};
