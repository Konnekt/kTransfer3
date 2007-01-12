// $Revision: 1.4 $

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
	OPERATION operation;
	unsigned number;
	bool clear;
public:
	CalculatorDialog();
	~CalculatorDialog();

	WidgetTextBoxPtr textField;
};
