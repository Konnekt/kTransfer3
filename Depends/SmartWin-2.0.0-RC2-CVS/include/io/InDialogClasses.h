// $Revision: 1.7 $
#ifndef InDialogClasses_9857_H
#define InDialogClasses_9857_H

#include "SmartWin.h"
using namespace SmartWin;
using namespace std;

#include "boost/any.hpp"

/// These classes can all be passed to an InDialog,
/// which will fill their values by the user.
/// In fact, they are only intended to be used with InDialog.

/// A long bounded by a minimum and maximum value.
/**
 * BoundedLong * can be passed to InDialog.add functions. <br>
 * and then the user cannot input values outside the valid range.
*/
class BoundedLong
{
public:
	/// Constructor that constrains itsVal as: itsMinval <= itsVal <= itsMaxval
	/**
	 *  BoundedInteger( 12 ) forces input values to be positive. <br>
	 *  BoundedInteger( 12, -2, 2 ) allows only -2, -1, 0, 1, or 2. <br>
	*/
	BoundedLong ( long inVal, long inMinval = 0, long inMaxval = 0x7fffffff ) :
		itsVal( inVal ), itsMinval( inMinval ), itsMaxval( inMaxval )
	{
	}

	long itsMinval, itsVal, itsMaxval;
};

/** A password string should not be visible when entered,
 *  and extra caution is taken to remove its characters
 * from memory in the destructor. <br>
 * PasswordString * can be passed to InDialog.add functions, <br>
 * and InDialog uses *** to mask the input characters.
 */
class PasswordString
{
public:
	/// Constructor that can specify a known length and an initial value.
	/** A nonzero inKnownLength force the user to input that many characters,
	 *  and thus enforces password strength standards, and prevents some typos.
	*/
	PasswordString ( int inKnownLength = 0, const SmartUtil::tstring & inString = _T( "" ) )
		: itsKnownLength( inKnownLength ),
		itsString( inString )
	{
	}

	/// Destructor attempts to remove the password characters from memory.
	/** This might not work depending on how string is implemented,
	 *  but the goal is to not leave the password in the heap somewhere.
	*/
	~ PasswordString ()
	{
		TCHAR blank( ' ' ); // Done only to prevent the password hanging around in memory.
		for ( unsigned i = 0; i < itsString.size(); i++ ) itsString[ i ] = blank;
	}

	SmartUtil::tstring itsString;
	int itsKnownLength;
};

/// A string that contains a filename, and thus can be specified with the file dialogs.
/** The prompt should be like  "Soup file ... "
 * to indicate that you can click on the ... and get a file dialog. <br>
 * FileString * can be passed to InDialog.add functions.
 * InDialog will allow a leftclick on the prompt to bring up a file dialog.
 */
class FileString
{
public:
	/// Constructor for an initial filename.  FileString( "me.txt" )
	FileString( const SmartUtil::tstring & inFilePath, bool inLoadFile = true, bool inRelative = false )
		: itsFilePath( inFilePath ),
		  itsFilterNo( 0 ),
		  itsLoadFile( inLoadFile ),
		  itsRelative( inRelative )
	{}

	/// Accumulates a "JPG files", "*.jpg"  fileter for Dialog files shown.
	void addFilter( const SmartUtil::tstring & filterName, const SmartUtil::tstring & filter )
	{
		itsFilters.push_back( std::pair< SmartUtil::tstring, SmartUtil::tstring >( filterName, filter ) );
	}

	/// Sets the active filter to the specified index.
	void activeFilter( unsigned inFilterNo )
	{
		itsFilterNo = inFilterNo;
	}


	// Static functions
	//-------------------------------------------------------------
	
	// return c:\dir\subdir
	//   from c:\dir\subdir\file.ext 
	//
	static SmartUtil::tstring pathOf( const SmartUtil::tstring & pathFile )
	{
		size_t slashDex= pathFile.find_last_of('\\');
		if ( string::npos == slashDex ) {

		}			
		return pathFile.substr( 0, slashDex ); 
	}

	//-------------------------------------------------------------
	
	// return               file.ext
	//   from c:\dir\subdir\file.ext 
	//
	static SmartUtil::tstring fileOf( const SmartUtil::tstring & pathFile )
	{
		size_t slashDex= pathFile.find_last_of('\\');
		if ( string::npos == slashDex ) {

		}	
		return pathFile.substr( ++slashDex ); 
	}


	//-------------------------------------------------------------
	
	// return     .ext
	//   from file.ext 
	//
	static SmartUtil::tstring extensionOf( const SmartUtil::tstring & fileDotExtension )
	{
		size_t dotDex= fileDotExtension.find_last_of('.');
		if ( string::npos == dotDex ) {

		}	
		return fileDotExtension.substr( dotDex ); 
	}


	//-------------------------------------------------------------
	
	// return file
	//   from file.ext 
	//
	static SmartUtil::tstring discardExtensionOf( const SmartUtil::tstring & fileDotExtension )
	{
		size_t dotDex= fileDotExtension.find_last_of('.');
		if ( string::npos == dotDex ) {

		}	
		return fileDotExtension.substr( 0, dotDex ); 
	}

	//-------------------------------------------------------------

	/// copy a file from the source file to the destination file
	//
	static bool copyFile( const SmartUtil::tstring & source,
					 const SmartUtil::tstring & dest,
					 bool failIfExists = false )
	{
		if ( ::CopyFile( source.c_str(), dest.c_str(), failIfExists ? TRUE : FALSE ) ) {
			return true;
		} else {
			return false;
		}
	}

	//-------------------------------------------------------------

	static bool deleteFile( const SmartUtil::tstring & filePath )
	{
		if ( ::DeleteFile( filePath.c_str() ) ) {
			return true;
		} else {
			return false;
		}
	}

	//-------------------------------------------------------------

	static bool setFileAttributes( const SmartUtil::tstring & filePath,
								   DWORD attributes )
	{
		if ( ::SetFileAttributes( filePath.c_str(), attributes ) ) {
			return true;
		} else {
			return false;
		}
	}

	//-------------------------------------------------------------


	// member variables

	bool itsLoadFile; // True implies use of WidgetLoadFile, false implies WidgetSaveFile.
	bool itsRelative;
	unsigned itsFilterNo;
	SmartUtil::tstring itsFilePath, itsRelativeFilePath;
	vector< std::pair< SmartUtil::tstring, SmartUtil::tstring > > itsFilters;
};





/// A string that contains a directory name.
/** The prompt should be like  "Install directory: "
 *
 * DirString * can be passed to InDialog.add functions.
 * InDialog will fill a combo box with subdirectories if the dir exists
 * otherwise with the directories above.  Double clicking on directories will
 * display a new level of directories.  Double clicking on .. will move up a level.
 * At the top level, InDialog will give a list of drives:  a:\ b:\ c:\ etc
 */
class DirString
{
public:
	/// Constructor for an initial directory name.  DirString( "c:\program files\tootsie" )
	DirString( const SmartUtil::tstring & inDirPath, bool inAllowCreation = true )
		: itsDirPath( inDirPath ),
		  itsAllowCreation( inAllowCreation )
	{}

	bool allowsCreation() {
		return itsAllowCreation;
	}

	SmartUtil::tstring getDir()
	{
		return itsDirPath;
	}

	void setDir( const SmartUtil::tstring & inDirPath )
	{
		itsDirPath= inDirPath;
	}


	//
	//
	bool dirExists()
	{
		DWORD  retv= GetFileAttributes( itsDirPath.c_str() );
		if ( 0xffffffff != retv ) {
			if ( FILE_ATTRIBUTE_DIRECTORY & retv ) return true;
		}
		return false;
	}


	// Create this directory
	// Returns true if this directory was created
	//
	bool createDir()
	{
		if ( CreateDirectory( itsDirPath.c_str(), NULL ) ) {
			return( true );
		} else {
			return( false );
		}
	}

	bool removeDirectory()
	{
		if ( RemoveDirectory( itsDirPath.c_str() ) ) {
			return( true );
		} else {
			return( false );
		}
	}



	// IN:  A new directory for this class instance.
	// OUT: returns a vector of the parent and subdirectories of inDirPath
	//
	// Given the current itsDirPath, generate a list of
	// alternate directories, including the root directory, ("\"), the
	// parent directory ("..") and any sub directories.
	// 
	// The hierarchy is:
	//
	// a:\
	// b:\
	// c:\
	//    c:\dir
	//          c:\dir\subdir
	// d:\
	//
	// The format of the vector returned is:
	// CurrentDir
	// ParentDir
	// SubDirectories
	//
	vector< SmartUtil::tstring > genNearDirs()
	{
		vector< SmartUtil::tstring > choices;

		if ( 0 == itsDirPath.compare(_T("My Computer") ) ) {
			genLogicalDrives( choices );
			choices.insert( choices.begin(), itsDirPath );
		} else {
			choices= genSubDirectories();
			choices.insert( choices.begin(), parentOf( itsDirPath ) );
			choices.insert( choices.begin(), itsDirPath );
		}

		return choices;
	}


	// return c:\dir\subdir
	//   from c:\dir\subdir\file.ext 
	//
	SmartUtil::tstring parentOf( const SmartUtil::tstring & pathFile )
	{
		if ( isJustDrive( pathFile ) ) {
			return _T("My Computer");
		}

		size_t slashDex= pathFile.find_last_of('\\');
		if ( string::npos == slashDex ) {
			return _T("My Computer");
		}
		SmartUtil::tstring parent = pathFile.substr( 0, slashDex );
		if ( string::npos == parent.find( '\\' ) ) {
			parent += _T("\\");	// Set "c:\" from c:
		}
		return parent; 
	}

	bool isJustDrive( const SmartUtil::tstring & pathFile )
	{
		if ( 1 == pathFile.find(':') ) {
			if ( pathFile.size() <= 3 ) {
				return true;	// "c:\"
			}
		}
		if ( 1 == pathFile.size() ) {
			if ( '\\' == pathFile[0] ) {
				return true;	// "\"
			}
		}

		return false;	// 
	}


	void genLogicalDrives( vector< SmartUtil::tstring > & subdirs ) 
	{
		SmartUtil::tstring drive;
		DWORD drives= GetLogicalDrives();
		int mask= 1;
		for( TCHAR b= 'A'; b <= 'Z'; b++ ) {
			if ( mask & drives ) {
				drive= _T("A:\\");
				drive[0]= b;
				subdirs.push_back( drive );
			}
			mask= mask << 1;
		}

	}

	//	Generate the vector of subdirectories
	//
	vector< SmartUtil::tstring > genSubDirectories() 
	{
		vector< SmartUtil::tstring > subdirs;	// Return value
		SmartUtil::tstring  curPath, curFile, cur_dir_star;

		curPath= itsDirPath;
		if ( '\\' != curPath[curPath.size()-1] ) {
			curPath += TEXT( "\\" );
		}
		// Now curPath is terminated with a \
		
		cur_dir_star= curPath + TEXT( "*" );

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile( cur_dir_star.c_str(), &FindFileData);

		while ( INVALID_HANDLE_VALUE != hFind ) {

			if ( FILE_ATTRIBUTE_DIRECTORY == FindFileData.dwFileAttributes ) {
				if ( '.' != FindFileData.cFileName[0] ) {
					curFile= curPath + FindFileData.cFileName;
					subdirs.push_back( curFile );
				}
			}

			if ( ! FindNextFile( hFind, &FindFileData) ) {
				break;			// No more files in the directory
			}
		}
		FindClose( hFind );
		return subdirs;
	}


	// Generate a vector of files that match a wildcard.
	// fullpath, if true, returns files as c:\dir\subdir\file.ext
	//		     if false,returns files as file.ext
	//
	vector< SmartUtil::tstring > genFiles( bool fullPath= true, SmartUtil::tstring ext = _T("*") ) 
	{
		vector< SmartUtil::tstring > subdirs;	// Return value
		SmartUtil::tstring  curPath, curFile, cur_dir_star;

		curPath= itsDirPath;
		if ( '\\' != curPath[curPath.size()-1] ) {
			curPath += TEXT( "\\" );
		}
		// Now curPath is terminated with a \
		
		cur_dir_star= curPath + ext;

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = FindFirstFile( cur_dir_star.c_str(), &FindFileData);

		while ( INVALID_HANDLE_VALUE != hFind ) {

			if ( FILE_ATTRIBUTE_DIRECTORY != FindFileData.dwFileAttributes ) {
				if ( fullPath ) {
					curFile= curPath + FindFileData.cFileName;
				} else {
					curFile= FindFileData.cFileName;
				}
				subdirs.push_back( curFile );
			}

			if ( ! FindNextFile( hFind, &FindFileData) ) {
				break;			// No more files in the directory
			}
		}
		FindClose( hFind );
		return subdirs;
	}


	// Static functions

	
	//-------------------------------------------------------------

	static SmartUtil::tstring currentDir()
	{
		TCHAR cur_dir[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, cur_dir );

		return SmartUtil::tstring( cur_dir );
	}

	//-------------------------------------------------------------

	static bool setCurrentDirectory(  const SmartUtil::tstring & pathFile )
	{
		if ( SetCurrentDirectory( pathFile.c_str() ) )
			return true;
		else 
			return false;
	}


	//-------------------------------------------------------------

	static SmartUtil::tstring getWindowsDirectory()
	{
		TCHAR winDir[MAX_PATH];
		if ( GetWindowsDirectory( winDir, MAX_PATH ) ) {
			return SmartUtil::tstring( winDir );
		}

		return SmartUtil::tstring();	// Empty if err.
	}



private:
	bool itsAllowCreation;	// If true, allow directories that don't exist yet.
	SmartUtil::tstring itsDirPath;
};





/// A string that must be selected from one of a list of possible strings.
/**
 * The current selection is itsChoosenIndex.
 * FileString * can be passed to InDialog.add functions,
 * and it results in a combo box presenting the list of possible strings.
 */
class ChoiceString
{
public:
	/// Constructor accepting  char * inStrings[] = { _T("scissors"), _T("rock"), _T("paper") };
	ChoiceString ( TCHAR * inStrings[], int inNumStrings, int inChossenIndex = 0 )
		: itsChoosenIndex( inChossenIndex )
	{
		for ( int i = 0; i < inNumStrings; i++ )
		{
			itsPossibleStrings.push_back( SmartUtil::tstring(inStrings[i]) );
		}
	}

	/// Constructor accepting vector< SmartUtil::tstring >
	ChoiceString ( const vector< SmartUtil::tstring > & inPossibleStrings, int inChossenIndex = 0 )
		: itsChoosenIndex( inChossenIndex )
//      ,  itsPossibleStrings( inPossibleStrings )
	{
		for ( unsigned int i = 0; i < inPossibleStrings.size(); i++ )
		{
			itsPossibleStrings.push_back( inPossibleStrings[i] );
		}
	}

	/// Constructor accepting "scissors|rock|paper"
	ChoiceString ( const SmartUtil::tstring & inBarSeperatedStrings, int inChossenIndex = 0 )
		: itsChoosenIndex( inChossenIndex )
	{
		SmartUtil::tstring choice;

		// Get each string before a |
		string::size_type dex = 0, barDex;
		SmartUtil::tstring bar( _T("|") );
		while ( string::npos != ( barDex = inBarSeperatedStrings.find( bar, dex ) ) )
		{
			choice = inBarSeperatedStrings.substr( dex, barDex - dex );
			itsPossibleStrings.push_back( choice );
			dex = barDex + 1;
		}

		// Get the string after the last |
		choice = inBarSeperatedStrings.substr( dex, inBarSeperatedStrings.size() - dex );
		itsPossibleStrings.push_back( choice );
	}

	int itsChoosenIndex;
	vector< SmartUtil::tstring > itsPossibleStrings;
};

#endif
