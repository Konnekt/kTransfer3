******************************************************************************************
*                         ReadMe.txt for the SmartWin++ library                          *
******************************************************************************************

- Compilation

  To compile with mingw you need
  MinGW:
  http://prdownloads.sourceforge.net/mingw/MinGW-4.1.1.exe?download 
  or use DeV-C++ which also includes mingw.
  http://prdownloads.sourceforge.net/dev-cpp/devcpp-4.9.9.2_setup.exe?download 

  To compile with the free msvc toolkit 2003 you need the
  Toolkit:
  http://msdn.microsoft.com/visualc/vctoolkit2003/
  Platfrom SDK:   
  http://www.microsoft.com/downloads/details.aspx?FamilyId=A55B6B43-E24F-4EA3-A93E-40C0EC4F68E5&displaylang=en
  cvtres.exe:
  http://support.microsoft.com/default.aspx?scid=kb;en-us;q187280
  
  To use the library, set up your compiler so that it will find the header file
  SmartWin.h and the library file SmartWin.lib/SmartWin.a. Normally you must add the 
  include directory path in which the SmartWin.h file resides and also add the library
  directory path where SmartWin.lib/SmartWin.a is found. Consult your compiler 
  documentation or go to http://smartwin.sourceforge.net/ to find out how to do this 
  for the most commonly-used compilers.

- Licensing
  The license for using SmartWin++ is in the same folder as this ReadMe file and
  named "License.txt". The library is free for all usage, including commercial
  applications.

- Do you need help?
  Please read the FAQ first at http://smartwin.sourceforge.net/faq.php. If your
  question is not answered there, use the support forums at
  http://sourceforge.net/forum/?group_id=92327.

- There is also a "Getting Started" page at the SmartWin++ website which as of 
  this writing resides at http://smartwin.sourceforge.net/getting_started.php
  in addition to a smaller (for Dummies tutorial) which resides at
  http://smartwin.sourceforge.net/tutorial.php

- If you're interested in contributing to SmartWin++ please subscribe to the general 
  mailing list which can be found here:
  http://sourceforge.net/mail/?group_id=92327
  or send in a post to the general forum which can be found here:
  http://sourceforge.net/forum/forum.php?forum_id=318866
  Or as a last resort if none of the above suits you send Thomas Hansen a personal
  email at polterguy@gmail.com


- Introduction to the library directory structure

                SmartWin
                ¦
                +---doxygenIn
                ¦   +---html
                ¦   +---Images
                +---doxygenOut
                ¦   +---doc
                ¦
                +---include
                ¦   +---boost
                ¦   +---io
                ¦   +---smartwin
                ¦       +---aspects
                ¦       +---widgets
                +---lib
                +---source
                +---tests


  Since SmartWin++ is mostly .h files, the few .cpp files are in the source directory.  
  
  The include\smartwin directory has the core .h files.
  The Widget or Controls or GUI building blocks classes are located in the Widgets subdirectory.
  Most widgets derive from the classes in the Aspects subdirectory.  Thus both WidgetButton and WidgetStatic
  derive from AspectClickable, but WidgetComboBox does not.

  The io directory contains optional io helper files that are not part of the SmartWin++ library, or
  SmartWin.h but may be helpful.  For example, they contain classes to serialize some of the Widgets into
  a html subset, socket, ftp and printing functions.
 
  The boost directory contains external code extracted from Boost.
  If you wish to use your own boost installation just delete this directory!
  

  The doxygenIn directory holds extra images and html files used to produce the Doxygen documentation.
  The doxygenOut\doc directory holds the output of the generation of Doxygen documentation.
    
  
  The lib directory gets the .lib or .o files after the source is compiled and linked.
  The test directory holds the 40 samples or tests for SmartWin++.


  
- Source structure

  For those who wish to extend the library.
  The fundamental include is SmartWin.h, which includes all the other .h files that define SmartWin++.
  Thus, after you add a new class, put an #include "newclass.h" into SmartWin.h like for instance if
  you create a new Widget or something which you wish to make public.


- Coding standards used in SmartWin++

    class Zoo            // We capitalize Classes.
    {
        Human itsKeeper; // Prefix with "its" to indicate member variables.

        void setKeeper( Human keeper ) // Camel case for member functions.
        {
            itsKeeper= keeper;
        }
    };




- Conditional Compilation for various OS targets

  The following #ifdef symbols are currently used in the SmartWin++ source:

  _MSC_VER		Microsoft Compiler version
  COMCTRL_V6
  IDCONTINUE
  IDTRYAGAIN
  MB_CANCELTRYCONTINUE
  UNICODE

  WINCE		for Windows Compact Edition or Windows Mobile.
  _DEVICE_RESOLUTION_AWARE
  _X86_	
    
  __WINE__	for compiling with Winelib to target the Linux operating system.
  __GNUC__
  
  
  
