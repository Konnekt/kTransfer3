// $Revision: 1.6 $

#include "SmartWin.h"

class WidgetTest
	: public SmartWin::WidgetFactory< SmartWin::WidgetWindow, WidgetTest >
{
public:
	WidgetTest()
	{}

	virtual ~WidgetTest()
	{}

	void tryFolderSelection()
	{
		// Choose an existing folder.
		WidgetChooseFolder folderDlg;
		folderDlg = createChooseFolder();
		folderDlg.setTitleText( _T( "Choose a directory" ) );
		folderDlg.setRootDirectory( CSIDL_DRIVES );
		folderDlg.setStartDirectory( _T( "C:\\" ) );

		SmartUtil::tstring DirPath = folderDlg.showDialog();
		createMessageBox().show( DirPath );
	}

	virtual void initAndCreate()
	{
		createWindow();
		setText( _T( "WidgetLoadFile compile and execute test" ) );

		tryFolderSelection();

		// Choose an existing file.
		WidgetLoadFile dlg = createLoadFile();
		dlg.addFilter( _T( "Text Files" ), _T( "*.txt" ) );
		dlg.addFilter( _T( "Data Files" ), _T( "*.dat" ) );
		dlg.addFilter( _T( "Registry Files" ), _T( "*.reg" ) );
		dlg.addFilter( _T( "All Files" ), _T( "*.*" ) );
		dlg.activeFilter( 3 );
		SmartUtil::tstring filePath = dlg.showDialog();
		WidgetMessageBox msgBox = createMessageBox();
		msgBox.show( filePath );

		std::vector<SmartUtil::tstring> filePathList = dlg.showDialogMultiSelect();
		filePath = _T("");
		std::vector<SmartUtil::tstring>::iterator iter;
		for( iter = filePathList.begin(); iter != filePathList.end(); ++iter )
		{
			filePath += *iter + _T("\r\n");
		}
		msgBox.show( filePath );

		close();
	}
};

// Unit test for WidgetLoadFile, tries to instantiate all different forms of WidgetLoadFile and also tries to use all functions
int SmartWinMain( SmartWin::Application & app )
{
	using namespace SmartWin;
	// First the "free" version...
	{
		WidgetLoadFileFree dlg;
		dlg.addFilter( _T( "Text Files" ), _T( "*.txt" ) );
		dlg.addFilter( _T( "Data Files" ), _T( "*.dat" ) );
		dlg.addFilter( _T( "Registry Files" ), _T( "*.reg" ) );
		dlg.addFilter( _T( "All Files" ), _T( "*.*" ) );
		dlg.activeFilter( 3 );
		SmartUtil::tstring filePath = dlg.showDialog();
		WidgetMessageBoxFree msgBox;
		msgBox.show( filePath );
	}
	{
		WidgetTest * testWnd = new WidgetTest;
		testWnd->initAndCreate();
		return app.run();
	}
}
