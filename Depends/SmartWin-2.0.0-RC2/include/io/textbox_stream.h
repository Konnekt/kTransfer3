// $Revision: 1.5 $
/*
  Copyright (c) 2005, Thomas Hansen
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

	  * Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.
	  * Redistributions in binary form must reproduce the above copyright notice, 
		this list of conditions and the following disclaimer in the documentation 
		and/or other materials provided with the distribution.
	  * Neither the name of the SmartWin++ nor the names of its contributors 
		may be used to endorse or promote products derived from this software 
		without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <iostream>
#include <queue>

namespace SmartWin
{
//////////////////////////////////////////////////////////////////////////

	// This is an iostream that outputs to a text box, and gets input from the keyboard.
	// credit: http://gcc.gnu.org/onlinedocs/libstdc++/27_io/howto.html#6
	//

	template< class Parent, class MessageMapPolicy = MessageMapPolicyNormalWidget >
	class WidgetTextBox_streambuf : public std::streambuf
		{
		private:
			SmartWin::WidgetTextBox< Parent, MessageMapPolicy > * itsTextBox;

			std::queue< int > itsPendingInput;
			enum
			{ BUFF_SIZE = 100
			};
			char itsBuff[ BUFF_SIZE ];

		public:
			WidgetTextBox_streambuf()
				: std::streambuf()
			{
				// Set input pointers for no input pending.
				//    eback(), gptr(),             egptr()
				setg( itsBuff, & itsBuff[BUFF_SIZE], & itsBuff[BUFF_SIZE] );
			}

			void setTextBox( SmartWin::WidgetTextBox< Parent, MessageMapPolicy > * TextBox )
			{
				itsTextBox = TextBox;
			}

			void addChar( int key )
			{
				itsPendingInput.push( key );
			}

		protected:

		// Fetch of keyboard data
		virtual int_type underflow()
		{
			if ( gptr() != egptr() ) return ( EOF );

			unsigned int n;

			if ( 0 == itsPendingInput.size() )
			{
				// Get keystrokes starting now...
				itsBuff[0] = getAsyncKey();
				n = 1;

				itsTextBox->setSelection( ( long ) itsTextBox->getText().size() );
				SmartUtil::tstring addstr( ( char * ) itsBuff, 1 );
				adjustLineTerm( addstr );
				itsTextBox->replaceSelection( addstr );
			}
			else
			{
				// Get previously entered keystrokes
				for ( n = 0; n < itsPendingInput.size(); n++ )
				{
					itsBuff[n] = 0xff & itsPendingInput.front();
					itsPendingInput.pop();
				}
			}

			// Now ensure that the data is in the
			// last part of the buffer
			char * d = & itsBuff[ BUFF_SIZE ];
			char * s = & itsBuff[ n ];

			if ( BUFF_SIZE == n )
			{
				d = itsBuff;
			}
			else
			{
				// Move the data from the begining
				//  of the buffer to the end.
				while ( s != itsBuff )
				{
					d--; s--;
					* d = * s;
				}
			}
			setg( itsBuff, d, & itsBuff[BUFF_SIZE] );
			return ( ( unsigned char ) * d );
		}

		virtual int_type overflow ( int_type c )
		{
			if ( c != EOF ) xsputn( & c, 1 );
			return c;
		}

		void adjustLineTerm( SmartUtil::tstring & str )
		{
			size_t pos_n = str.find( '\n' );
			if ( - 1 != pos_n )
			{
				str.replace( pos_n, 1, "\r\n" );
				return;
			}

			size_t pos_r = str.find( '\r' );
			if ( - 1 != pos_r )
			{
				str.replace( pos_r, 1, "\r\n" );
			}
		}

		// Output of data to text box.
		virtual std::streamsize xsputn( int_type * data, std::streamsize _Count )
		{
			itsTextBox->setSelection( ( long ) itsTextBox->getText().size() );

			SmartUtil::tstring addstr( ( char * ) data, _Count );
			adjustLineTerm( addstr );
			itsTextBox->replaceSelection( addstr );

			itsTextBox->showCaret();

			return( _Count ); // Number of characters copied.
		}

		private:

		char getAsyncKey()
		{
			MSG Msg;
			UINT wm = WM_KEYDOWN; // Get only this message.
			BOOL retv = ::GetMessage( & Msg, NULL, wm, wm );
			if ( 1 == retv )
			{
				return( itsTextBox->virtualKeyToChar( Msg.wParam ) );
			}

			// -1 is an error, 0 is WM_QUIT
			return( '\n' );
		}
	};

	/// An iostream that outputs to a SmartWin++ text box and gets input from the keyboard.
	/** This is a new iostream based upon the custom streambuf called WidgetTextBox_streambuf.
	  * All that is needed is to use the iostream constructor with a tcp_streambuf.
	  * Then you have the normal iostream operations just as cout or cin.
	  */
	template< class Parent, class MessageMapPolicy = MessageMapPolicyNormalWidget >
	class TextBox_iostream : public std::iostream
	{
		WidgetTextBox_streambuf< Parent, MessageMapPolicy > tb_sb;

	public:
		/// The constructor requires a created WidgetTextBox.
		TextBox_iostream( SmartWin::WidgetTextBox< Parent, MessageMapPolicy > * TextBox )
			: std::iostream( & tb_sb )
		{
			tb_sb.setTextBox( TextBox );
		};
	};
}

//////////////////////////////////////////////////////////////////////////
