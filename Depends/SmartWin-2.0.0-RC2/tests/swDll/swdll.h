// $Revision: 1.2 $
// Please #define DLLMODE to be dllimport from a calling module,
//    and #define DLLMODE to be dllexport from the DLL.
//
// Don't allocate memory in the DLL which will be freed in the executable,
// or you will get a run time exception.
//
#include <string>

__declspec( DLLMODE )
bool getNumberAndString( int * numb, char str[] );

__declspec( DLLMODE )
bool guiGetNumberAndString( int * numb, char str[] );
