#include <SmartWin.h>
#include "resource.h"
using namespace SmartWin;

class CTest :
   public WidgetFactory<WidgetModalDialog, CTest, MessageMapPolicyModalDialogWidget>
{
public:
   WidgetButtonPtr testbutton;

   CTest(Widget * parent)
       : Widget( parent )
   {
       onInitDialog(&CTest::initDialog);
       onClosing(&CTest::closing);
   }
   ~CTest(void) { }
   bool initDialog()
   {
       testbutton = subclassButton(IDOK);
       testbutton->onClicked( &CTest::test_clicked );
       return false;
   }
   bool closing()
   {
       this->endDialog(IDOK);
       return true;
   }
   void test_clicked( WidgetButtonPtr btn )
   {
       this->endDialog(IDOK);
   }
};

class MainDlg :
   public WidgetFactory<WidgetModalDialog, MainDlg, MessageMapPolicyModalDialogWidget>
{
public:
   MainDlg( Widget * parent )
       : Widget( parent )
   {
       onInitDialog(&MainDlg::initDialog);
       onClosing(&MainDlg::closing);
   }
   ~MainDlg(void) { }

   WidgetButtonPtr testbutton;

   bool initDialog()
   {
       testbutton = subclassButton(IDC_BUTTON_ABOUT);
       testbutton->onClicked( &MainDlg::test_clicked );
       return false;
   }
   bool closing()
   {
       this->endDialog(IDOK);
       return true;
   }
   void test_clicked( WidgetButtonPtr btn )
   {
       CTest testdlg(this);
       testdlg.createDialog( IDD_DIALOG2 );
   }
};

int SmartWinMain( Application & app )
{
   MainDlg m_mainDlg(0);
   m_mainDlg.createDialog(IDD_DIALOG1);
   return app.run();
}