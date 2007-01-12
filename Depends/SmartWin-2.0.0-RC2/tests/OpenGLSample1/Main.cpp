// $Revision: 1.4 $
/*  Show a little OpenGL
	Sample of how to use SmartWin together with Open GL ported from Ron Fosner's OpenGL Programming
	for Win95/NT by Andrew Webb.
	All though SW doesn't contain any Open GL classes as of this writing (10. Dec 2005) it
	still can interact with Open GL like pure Windows API apps can...
	This is a sample of such an interaction!
 */

#include "SmartWin.h"
using namespace SmartWin;

#include <GL/gl.h>     // OpenGL interface
#include <GL/glu.h>    // OpenGL utility Library interface

class OpenGLWin
	: public WidgetFactory< WidgetWindow, OpenGLWin >
{
private:
	HGLRC hRC;
	PAINTSTRUCT ps;
	HDC hDC;
	HWND hWnd;

public:
	void init()
	{
		Seed cs;

		cs.style |= CS_HREDRAW | CS_VREDRAW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		onCreate( & OpenGLWin::whenCreate );
		createWindow( cs );

		setText( _T( "OpenGLWin" ) );

		COLORREF itsColor = RandColor();
		onSized( & OpenGLWin::isResized );
		onPainting( & OpenGLWin::painting );
		onClosing( & OpenGLWin::willClose );
		setBounds( 100, 100, 700, 600 );
		updateWidget();
	}

private:
	void whenCreate( const SmartWin::Seed & cs )
	{
		// Select a pixel format and then
		// create a rendering context from it.
		hRC = setUpOpenGL( this->handle() );
	}

	COLORREF RandColor()
	{
		return( RGB( rand()%255, rand()%255, rand()%255 ) );
	}

	//  Called when the window needs to be redrawn
	void painting( SmartWin::Canvas & c )
	{
		// Get a DC, then make the RC current and
		// associated with this DC
		// Note!
		// BeginPaint and EndPaint is AUTOMAGICALLY called for us by SmartWin inernally before and after this event handler is called!!
		hDC = c.getDc();
		wglMakeCurrent( hDC, hRC );

		drawOpenGLScene();

		// we're done with the RC, so
		// deselect it
		// (note: This technique is not recommended!)
		wglMakeCurrent( NULL, NULL );
	}

	// There are new dimensions to the window because the user resized the window.
	void isResized( const SmartWin::WidgetSizedEventResult & sz )
	{
		// Redefine the viewing volume and viewport
		// when the window size changes.

		// Make the RC current since we're going to
		// make an OpenGL call here...
		hWnd = this->handle();
		hDC = GetDC ( hWnd );

		wglMakeCurrent ( hDC, hRC );

		// get the new size of the client window
		// note that we size according to the height,
		// not the smaller of the height or width.
		GLsizei glnWidth = sz.newSize.x;
		GLsizei glnHeight = sz.newSize.y;

		GLdouble gldAspect = ( GLdouble ) glnWidth / ( GLdouble ) glnHeight;

		// set up a projection matrix to fill the
		//  client window
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		// a perspective-view matrix...
		gluPerspective(
			30.0,   // Field-of-view angle
			gldAspect, // Aspect ratio of view volume
			1.0,    // Distance to near clipping plane
			10.0 ); // Distance to far clipping plane
		glViewport( 0, 0, glnWidth, glnHeight );
		wglMakeCurrent( NULL, NULL );
		ReleaseDC( hWnd, hDC );
		// Need to explicitly say we need an update since by default Windows will only trigger an update (WM_PAINT) if
		// any of the sizes is INCREASED!!
		updateWidget();
	}

	bool willClose()
	{
		// Clean up and terminate.
		wglDeleteContext( hRC );
		return true;
	}

	/////////////////////////////////////////////////////////
	//  setUpOpenGL sets the pixel format and a rendering
	//  context then returns the RC
	/////////////////////////////////////////////////////////
	HGLRC setUpOpenGL( HWND hWnd )
	{
		static PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof ( PIXELFORMATDESCRIPTOR ), // strcut size
			1,                              // Version number
			PFD_DRAW_TO_WINDOW |    // Flags, draw to a window,
				PFD_SUPPORT_OPENGL, // use OpenGL
			PFD_TYPE_RGBA,          // RGBA pixel values
			24,                     // 24-bit color
			0, 0, 0,                // RGB bits & shift sizes.
			0, 0, 0,                // Don't care about them
			0, 0,                   // No alpha buffer info
			0, 0, 0, 0, 0,          // No accumulation buffer
			32,                     // 32-bit depth buffer
			0,                      // No stencil buffer
			0,                      // No auxiliary buffers
			PFD_MAIN_PLANE,         // Layer type
			0,                      // Reserved (must be 0)
			0,                      // No layer mask
			0,                      // No visible mask
			0                       // No damage mask
		};

		int nMyPixelFormatID;
		HDC hDC;
		HGLRC hRC;

		hDC = GetDC( hWnd );
		nMyPixelFormatID = ChoosePixelFormat( hDC, & pfd );

		// catch errors here.
		// If nMyPixelFormat is zero, then there's
		// something wrong... most likely the window's
		// style bits are incorrect (in CreateWindow() )
		// or OpenGl isn't installed on this machine
		SetPixelFormat( hDC, nMyPixelFormatID, & pfd );
		hRC = wglCreateContext( hDC );
		ReleaseDC( hWnd, hDC );
		return hRC;
	}

	//////////////////////////////////////////////////////////
	//  DrawScene uses OpenGL commands to draw a cube.
	//  This is where the OpenGL drawing commands live
	//////////////////////////////////////////////////////////
	void drawOpenGLScene()
	{
		// Enable depth testing and clear the color and depth buffers.
		glEnable( GL_DEPTH_TEST );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Define the modelview transformation.
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		// move the viewpoint out to where we can see everything
		glTranslatef( 0.0f, 0.0f, - 5.0f );

		// Draw a large triangle out of three smaller triangles
		// sharing common vertex colors
		//  Upper left triangle
		glBegin( GL_POLYGON );
			glColor3f( 0.0f, 0.0f, 0.0f ); // black center
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glColor3f( 0.0f, 1.0f, 0.0f ); // left vertex green
			glVertex3f( - 1.0f, - 1.0f, 0.0f );
			glColor3f( 1.0f, 0.0f, 0.0f ); // upper vertex red
			glVertex3f( 0.0f, 1.0f, 0.0f );
		glEnd();

		//  bottom triangle
		glBegin( GL_POLYGON );
			glColor3f( 0.0f, 0.0f, 0.0f ); // black center
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glColor3f( 0.0f, 0.0f, 1.0f ); // right vertex blue
			glVertex3f( 1.0f, - 1.0f, 0.0f );
			glColor3f( 0.0f, 1.0f, 0.0f ); // left vertex green
			glVertex3f( - 1.0f, - 1.0f, 0.0f );
		glEnd();

		//  upper right triangle
		glBegin( GL_POLYGON );
			glColor3f( 0.0f, 0.0f, 0.0f ); // black center
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glColor3f( 1.0f, 0.0f, 0.0f ); // upper vertex red
			glVertex3f( 0.0f, 1.0f, 0.0f );
			glColor3f( 0.0f, 0.0f, 1.0f ); // bottom right blue
			glVertex3f( 1.0f, - 1.0f, 0.0f );
		glEnd();

		// Flush the drawing pipeline since it's single buffered
		glFlush ();
	}
};

int SmartWinMain( Application & app )
{
	OpenGLWin * tmp = new OpenGLWin();
	tmp->init();
	return app.run();
}
