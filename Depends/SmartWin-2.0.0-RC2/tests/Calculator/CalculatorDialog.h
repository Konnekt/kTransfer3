// $Revision: 1.5 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include "SmartWin.h"

class CalculatorDialog
	: public SmartWin::WidgetFactory< SmartWin::WidgetDialog, CalculatorDialog, SmartWin::MessageMapPolicyDialogWidget >
{
private:
	void numberClicked( WidgetButtonPtr btn );
	void addSubEqualClicked( WidgetButtonPtr btn );
	void acClicked( WidgetButtonPtr btn );

	typedef enum OPERATION
	{
		NONE, ADD, SUBTRACT
	};
	OPERATION itsOperation;
	unsigned itsNumber;
	bool itsClear;

public:
	CalculatorDialog();
	~CalculatorDialog();

	WidgetTextBoxPtr textField;
};
