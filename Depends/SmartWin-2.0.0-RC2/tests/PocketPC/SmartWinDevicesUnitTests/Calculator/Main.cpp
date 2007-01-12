// $Revision: 1.3 $
#include "SmartWin.h"
#include "CalculatorDialog.h"
#include <fstream>

int SmartWinMain( SmartWin::Application & app )
{
	CalculatorDialog * dlg = new CalculatorDialog();
	return app.run();
}
