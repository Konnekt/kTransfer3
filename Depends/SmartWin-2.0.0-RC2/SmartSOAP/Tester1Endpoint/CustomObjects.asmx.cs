using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Web;
using System.Web.Services;

namespace Tester1Endpoint
{
	public class TestReturnStuffContents
	{
		public string stringValue;
	}

	public class TestReturnStuff
	{
		public int intValue;
		public string stringValue;
		public decimal decimalValue;
		public TestReturnStuffContents[] arrayOfContents;
	}

	public class TestGetStuff
	{
		public string getStuffString;
	}

	public class ReallyAdvancedStuff
	{
		public decimal decimalValue;
		public int intValue;
		public string stringValue;
		public float floatValue;
		public double doubleValue;
		public DateTime dateTimeValue;
		public CustomObjects.ReallyAdvancedStuffExtra[] extraValues;
	}

	/// <summary>
	/// Summary description for CustomObjects.
	/// </summary>
	[WebService(Namespace="http://smartwin.org/SmartSOAP/WebServices/Test/CustomObject")]
	public class CustomObjects : System.Web.Services.WebService
	{
		public class ReallyAdvancedStuffExtraEmpty
		{
			// Intentionally empty!!
		}

		public class ReallyAdvancedStuffExtra
		{
			public int y;
			public ReallyAdvancedStuffExtraEmpty[] tmp;
			public ReallyAdvancedStuffExtraEmpty tmp2;
		}

		public class ReallyAdvancedReturnValue
		{
			public ReallyAdvancedStuff[] arrayOfreallyAdvancedStuff;
			public ReallyAdvancedStuff reallyAdvancedStuff;
		}

		public class ReallyAdvancedInputValue
		{
			public ReallyAdvancedStuff[] arrayOfreallyAdvancedStuff;
			public ReallyAdvancedStuff reallyAdvancedStuff;
		}

		public CustomObjects()
		{
			//CODEGEN: This call is required by the ASP.NET Web Services Designer
			InitializeComponent();
		}

		#region Component Designer generated code
		
		//Required by the Web Services Designer 
		private IContainer components = null;
				
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if(disposing && components != null)
			{
				components.Dispose();
			}
			base.Dispose(disposing);		
		}
		
		#endregion

		// WEB SERVICE EXAMPLE
		// The HelloWorld() example service returns the string Hello World
		// To build, uncomment the following lines then save and build the project
		// To test this web service, press F5

		[WebMethod]
		public string HelloWorld( int number )
		{
			if( number < 10 && number > 0 )
				return "Hello World: " + number.ToString();
			else
				throw new Exception( "The number you typed was too high!!\r\nPlease keep the number between 0 and 10" );
		}

		[WebMethod]
		public string[] GetArrayOfStrings( int howMany, string whatShouldTheyContain )
		{
			string[] retVal = new string[howMany];
			for( int n = 0; n < howMany; n++ )
			{
				retVal[n] = n.ToString() + ": " + whatShouldTheyContain;
			}
			return retVal;
		}

		[WebMethod]
		public TestReturnStuff GetCustomObject( TestGetStuff getStuff )
		{
			TestReturnStuff retVal = new TestReturnStuff();
			retVal.decimalValue = 1.56M;
			retVal.intValue = 56;
			retVal.stringValue = "Thomas";
			TestReturnStuffContents[] array = new TestReturnStuffContents[2];
			array[0] = new TestReturnStuffContents();
			array[0].stringValue = getStuff.getStuffString + " TJOHOO";
			array[1] = new TestReturnStuffContents();
			array[1].stringValue = getStuff.getStuffString + " TJOHOO";
			retVal.arrayOfContents = array;
			return retVal;
		}

		[WebMethod]
		public DateTime GetDateTimeNow( int offset )
		{
			return DateTime.Now.AddDays(offset);
		}

		[WebMethod]
		public ReallyAdvancedReturnValue GetReallyAdvancedReturnValue( ReallyAdvancedInputValue input )
		{
			CustomObjects.ReallyAdvancedReturnValue retVal = new CustomObjects.ReallyAdvancedReturnValue();
			retVal.arrayOfreallyAdvancedStuff = new ReallyAdvancedStuff[3];
			retVal.reallyAdvancedStuff = new ReallyAdvancedStuff();
			retVal.reallyAdvancedStuff.decimalValue = 54.5M;
			for( int x = 0; x < 3; x++ )
			{
				if( x != 0 )
				{
					retVal.arrayOfreallyAdvancedStuff[x] = new ReallyAdvancedStuff();
					retVal.arrayOfreallyAdvancedStuff[x].dateTimeValue = DateTime.Now;
					retVal.arrayOfreallyAdvancedStuff[x].decimalValue = 5.54M;
					retVal.arrayOfreallyAdvancedStuff[x].doubleValue = 4.45D;
					retVal.arrayOfreallyAdvancedStuff[x].floatValue = 3.2F;
					retVal.arrayOfreallyAdvancedStuff[x].intValue = 54;
					retVal.arrayOfreallyAdvancedStuff[x].stringValue = "Thomas Hansen";
					retVal.arrayOfreallyAdvancedStuff[x].extraValues = new CustomObjects.ReallyAdvancedStuffExtra[3];
					for( int y = 0; y < 3; y++ )
					{
						if( y != 0 )
						{
							retVal.arrayOfreallyAdvancedStuff[x].extraValues[y] = new CustomObjects.ReallyAdvancedStuffExtra();
							retVal.arrayOfreallyAdvancedStuff[x].extraValues[y].y = 32;
							retVal.arrayOfreallyAdvancedStuff[x].extraValues[y].tmp = new ReallyAdvancedStuffExtraEmpty[2];
							retVal.arrayOfreallyAdvancedStuff[x].extraValues[y].tmp[0] = new ReallyAdvancedStuffExtraEmpty();
						}
					}
				}
			}
			return retVal;
		}
	}
}
