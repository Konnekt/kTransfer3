// $Revision: 1.4 $
#pragma once

#include "SmartWin.h"

class DialogWidget
	: public SmartWin::WidgetFactory< SmartWin::WidgetDialog, ::DialogWidget, SmartWin::MessageMapPolicyDialogWidget >
{
private:
	bool closing();
	WidgetButtonPtr itsOk;
	WidgetButtonPtr itsCancel;
	WidgetButtonPtr itsBtnTest;
	WidgetTextBoxPtr itsTxt;
	WidgetSplitterCoolPtr itsSplitter;
	void sizingSplitter( WidgetSplitterCoolPtr splitter,
		const SmartWin::Point & pt );
	void buttonPushed( WidgetButtonPtr button );
public:
	DialogWidget( SmartWin::Widget * parent );
	virtual ~DialogWidget();
};
