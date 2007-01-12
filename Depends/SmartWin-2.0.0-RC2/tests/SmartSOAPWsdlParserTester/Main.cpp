// $Revision: 1.2 $

#include "SmartSOAP.h"
#include "ProxyClasses.h"

using namespace std;

// Note that this project uses AUTO GENERATED WebService proxy objects
// The generator of the Proxy Classes is the SmartSOAPWsdlParser project!

int main()
{
	try
	{
		// Our WebService Proxy object
		CustomObjects proxy;

		// Calling HelloWorld
		HelloWorldInput hello;
		hello.number = 5;
		cout << "Calling hello world" << endl;
		HelloWorldResponseInput helloResponse = proxy.HelloWorld( hello );
		cout << * helloResponse.HelloWorldResult << endl << endl;

		// Calling GetDateTimeNow
		GetDateTimeNowInput getDate;
		getDate.offset = 1;
		cout << "Calling GetDateTimeNow" << endl;
		GetDateTimeNowResponseInput getDateResponse = proxy.GetDateTimeNow( getDate );
		cout << getDateResponse.GetDateTimeNowResult.toString() << endl << endl;

		// Calling GetCustomObject
		GetCustomObjectInput getCustom;
		getCustom.getStuff = new TestGetStuffInput();
		getCustom.getStuff->getStuffString = new SmartUtil::tstring( _T( "Thomas Hansen" ) );
		cout << "Calling GetCustomObject" << endl;
		GetCustomObjectResponseInput getCustomResponse = proxy.GetCustomObject( getCustom );
		cout << getCustomResponse.GetCustomObjectResult->decimalValue << endl;
		cout << getCustomResponse.GetCustomObjectResult->intValue << endl;
		cout << * getCustomResponse.GetCustomObjectResult->stringValue << endl;
		for ( int x = 0;
			x != getCustomResponse.GetCustomObjectResult->arrayOfContents->TestReturnStuffContents.size();
			++x )
		{
			cout << x << " " << * getCustomResponse.GetCustomObjectResult->arrayOfContents->TestReturnStuffContents[x]->stringValue << endl;
		}
		cout << endl;

		// Calling GetArrayOfStrings
		GetArrayOfStringsInput getArray;
		getArray.howMany = 2;
		getArray.whatShouldTheyContain = new SmartUtil::tstring( _T( "Thomas" ) );
		cout << "Calling GetArrayOfStrings" << endl;
		GetArrayOfStringsResponseInput getArrayResponse = proxy.GetArrayOfStrings( getArray );
		for ( int x = 0;
			x != getArrayResponse.GetArrayOfStringsResult->string.size();
			++x )
		{
			cout << x << " " << * getArrayResponse.GetArrayOfStringsResult->string[x] << endl;
		}
		cout << endl;

		// Calling GetReallyAdvancedReturnValue
		GetReallyAdvancedReturnValueInput sendObject;
		sendObject.input = new ReallyAdvancedInputValueInput();
		sendObject.input->arrayOfreallyAdvancedStuff = new ArrayOfReallyAdvancedStuffInput();
		SmartUtil::null_pointer< ReallyAdvancedStuffInput > tmp1( new ReallyAdvancedStuffInput );
		tmp1->dateTimeValue = SmartUtil::DateTime( 2006, 12, 24, 17, 15, 30 );
		tmp1->decimalValue = 5.65;
		tmp1->doubleValue = 67.89;
		tmp1->floatValue = 54.43f;
		tmp1->intValue = 12;
		tmp1->stringValue = new SmartUtil::tstring( _T( "Ola Dunk" ) );
		tmp1->extraValues = new ArrayOfReallyAdvancedStuffExtraInput;
		SmartUtil::null_pointer< ReallyAdvancedStuffExtraInput > buf1( new ReallyAdvancedStuffExtraInput );
		buf1->tmp = new ArrayOfReallyAdvancedStuffExtraEmptyInput;
		SmartUtil::null_pointer< ReallyAdvancedStuffExtraEmptyInput > bf1;
		buf1->tmp->ReallyAdvancedStuffExtraEmpty.push_back( bf1 );
		SmartUtil::null_pointer< ReallyAdvancedStuffExtraEmptyInput > bf2;
		buf1->tmp->ReallyAdvancedStuffExtraEmpty.push_back( bf2 );
		buf1->tmp2 = new ReallyAdvancedStuffExtraEmptyInput;
		tmp1->extraValues->ReallyAdvancedStuffExtra.push_back( buf1 );
		sendObject.input->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff.push_back( tmp1 );

		cout << "Calling GetReallyAdvancedReturnValue" << endl;
		GetReallyAdvancedReturnValueResponseInput resp = proxy.GetReallyAdvancedReturnValue( sendObject );

		if ( resp.GetReallyAdvancedReturnValueResult.get() )
		{
			if ( resp.GetReallyAdvancedReturnValueResult->reallyAdvancedStuff.get() )
			{
				cout << resp.GetReallyAdvancedReturnValueResult->reallyAdvancedStuff->dateTimeValue.toString() << endl;
				cout << resp.GetReallyAdvancedReturnValueResult->reallyAdvancedStuff->decimalValue << endl;
				cout << resp.GetReallyAdvancedReturnValueResult->reallyAdvancedStuff->doubleValue << endl;
				cout << resp.GetReallyAdvancedReturnValueResult->reallyAdvancedStuff->floatValue << endl;
				cout << resp.GetReallyAdvancedReturnValueResult->reallyAdvancedStuff->intValue << endl;
				if ( resp.GetReallyAdvancedReturnValueResult->reallyAdvancedStuff->stringValue.get() )
					cout << * resp.GetReallyAdvancedReturnValueResult->reallyAdvancedStuff->stringValue << endl;
				else
					cout << "resp.GetReallyAdvancedReturnValueResult->reallyAdvancedStuff->stringValue is NOT there" << endl;
			}

			if ( resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff.get() )
			{
				for ( size_t x = 0; x < resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff.size(); ++x )
				{
					if ( resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x].get() )
					{
						cout << resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->dateTimeValue.toString() << endl;
						cout << resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->decimalValue << endl;
						cout << resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->doubleValue << endl;
						cout << resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->floatValue << endl;
						cout << resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->intValue << endl;
						if ( resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->stringValue.get() )
							cout << * resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->stringValue << endl;
						cout << resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->doubleValue << endl;

						if ( resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->extraValues.get() )
						{
							for ( size_t y = 0; y < resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->extraValues->ReallyAdvancedStuffExtra.size(); ++y )
							{
								if ( resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->extraValues->ReallyAdvancedStuffExtra[y].get() )
								{
									cout << resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->extraValues->ReallyAdvancedStuffExtra[y]->y << endl;
									if ( resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->extraValues->ReallyAdvancedStuffExtra[y]->tmp.get() )
									{
										for ( size_t z = 0; z < resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->extraValues->ReallyAdvancedStuffExtra[y]->tmp->ReallyAdvancedStuffExtraEmpty.size(); ++z )
										{
											if ( resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->extraValues->ReallyAdvancedStuffExtra[y]->tmp->ReallyAdvancedStuffExtraEmpty[z].get() )
												cout << "ReallyAdvancedStuffExtraEmpty IS there" << endl;
											else
												cout << "ReallyAdvancedStuffExtraEmpty is NOT there" << endl;
										}
									}
									if ( resp.GetReallyAdvancedReturnValueResult->arrayOfreallyAdvancedStuff->ReallyAdvancedStuff[x]->extraValues->ReallyAdvancedStuffExtra[y]->tmp2.get() )
										cout << "tmp2 IS there" << endl;
									else
										cout << "tmp2 is NOT there" << endl;
								}
							}
						}
					}
				}
			}
		}

		cout << endl;
	}
	catch ( std::exception & err )
	{
		MessageBox( 0, err.what(), "", MB_OK );
	}

	int x;
	cin >> x;
	return EXIT_SUCCESS;
}
