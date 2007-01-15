WHAT IS DOXYGEN ?

Doxygen takes the Smartwin++ source tree and some configuration in doxygenIn
and produces HTML output in doxygenOut.

So the doxygenIn files should be kept under CVS, while the doxygenOut are transient.

If you are a SmartWin++ developer you should use doxygen to generate the
  documentation for a new class to catch functions that don't have understandable documentation.

See www.doxygen.org  for more details.  

  

HOW TO OPERATE DOXYGEN

Install doxygen from http://www.stack.nl/~dimitri/doxygen/download.html#latestsrc
You can just get the doxygen.exe binary and do "doxygen Doxyfile" or you can do the
full install and do:

	Run the Doxywizard.    
	Load Smartwin\doxygenIn\Doxyfile.   Your working directory should be doxygenIn.
	Click [Start]


Examine the errors.txt file in doxygenOut.
Use a web browser to click on doxygenOut\doc\index.html.

( As an alternative, you can use the doxyfileNoInheritence to generate documentation
that does not show inherited functions.  )


GOALS:

Improve the existing doxygen smartwin++ documentation system by:

1) Using only relative paths so doxygen runs after CVS extraction.
2) Update doxygen to the new directory structure
3) Add documentation via included HTML files
4) Fix misc problems to prepare for the 1.0 release.


FILES EXPLANATION:

Doxyfile		is the configuration file for Doxygen.
footer.txt		is what doxygen adds to each html output file
doxygen.css		is a cascading style sheet
Images dir		holds the various images referenced in the doxygen documented source code.



HOW TO DOCUMENT SOURCE CODE USING DOXYGEN:

- Embedded comments suitable for doxygen

  We use doxygen to generate automated C++ class documentation.  That's why you see:
  /// A line comment for a function shown in a list of member functions.
  /** Block comments that are shown when
    * the function is shown in more detail.
    */
    mySuperDuperFunction( ... )
  


- Special commands used in SmartWin++ documentation.

You may have noticed that doxygen.h has a series of special comments like this:

/** \defgroup AspectClasses Aspect Classes
  */


The defgroup defines the group "AspectClasses",
and associates it with the text "Aspect Classes".
Later on, we can refer to this group while documenting a class:

	/// Aspect class used by Widgets that have the possibility of retrieving the focus
	/** \ingroup AspectClasses
	  * explain it.
	  */
	template<class WindowOriginalType, class WidgetType, class MessageMapType>
	class AspectFocus

 
The groups show up as "Modules" in the doxygen output.


Here are some useful special commands:
 
/**
 * \include code.cpp			Brings in source code with links to doxygen documentation
 * \htmlinclude guide.html		Includes arbitrary html from doxygenIn\html
 * \image html menu.png			Includes an image file  from doxygenIn\images
 */



/** \page <name> (title)
 *
 /
You can see examples in doxygen.h; this is how we inserted the "Hello World" sample source code
 


/**
 * \file guide.cpp
 * \ingroup WidgetLayout
 * 
 */
This is a way to document a file.


A. Webb, August 28th, 2005