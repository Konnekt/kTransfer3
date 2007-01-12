// $Revision: 1.4 $
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

	bool itsLoadFile; // True implies use of WidgetLoadFile, false implies WidgetSaveFile.
	bool itsRelative;
	unsigned itsFilterNo;
	SmartUtil::tstring itsFilePath, itsRelativeFilePath;
	vector< std::pair< SmartUtil::tstring, SmartUtil::tstring > > itsFilters;
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
	/// Constructor accepting  char * inStrings[] = { "scissors", "rock", "paper" };
	ChoiceString ( char * inStrings[], int inNumStrings, int inChossenIndex = 0 )
		: itsChoosenIndex( inChossenIndex )
	{
		for ( int i = 0; i < inNumStrings; i++ )
		{
			itsPossibleStrings.push_back( inStrings[i] );
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
		while ( string::npos != ( barDex = inBarSeperatedStrings.find( "|", dex ) ) )
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
