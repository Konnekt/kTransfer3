// $Revision: 1.4 $
/* Copyright (c) 2005 - Thomas Hansen
 * Part of the SmartWin++ library example code
 * Permission to use under the same terms as the actual library
 * The license to the library can be found at http://smartwinlib.org
 * License for library is Open Source and BSD
 */
#include <fstream>
#include "SmartSOAP.h"
#include "SmartWin.h"
#include "MainWindow.h"
#include "WsdlParser.h"

using namespace SmartWin;

int SmartWinMain( Application & app )
{
	const CommandLine & com = app.getCommandLine();
	const std::vector< std::string > params = com.getParams();

	bool hasCommandLineParams = false;
	std::string wsdl;
	std::string output;

	for ( std::vector< std::string >::const_iterator idx = params.begin();
		idx != params.end();
		++idx )
	{
		hasCommandLineParams = true;
		if ( * idx == "-wsdl" && idx != params.end() && ( idx + 1 ) != params.end() )
			wsdl = * ( idx + 1 );
		else if ( * idx == "-ouput" && idx != params.end() && ( idx + 1 ) != params.end() )
			output = * ( idx + 1 );
	}

	if ( hasCommandLineParams && output.size() != 0 )
	{
		// Code file
		boost::tuples::tuple< SmartUtil::tstring, SmartUtil::tstring > retVal =
			WsdlParser::instance().fetchWsdlFromUrl( SmartUtil::Ascii2CurrentBuild::doConvert( wsdl, SmartUtil::ConversionCodepage::ANSI ) );
		std::string tmpOuput = retVal.get< 1 >();
		for ( std::string::iterator idx = tmpOuput.begin();
			idx != tmpOuput.end();
			++idx )
		{
			if ( * idx == '\r' )
			{
				tmpOuput.replace( idx, idx + 1, "" );
			}
		}
		std::ofstream code( ( output + ".h" ).c_str() );
		code << tmpOuput;

		// Wsdl file
		tmpOuput = retVal.get< 0 >();
		for ( std::string::iterator idx = tmpOuput.begin();
			idx != tmpOuput.end();
			++idx )
		{
			if ( * idx == '\r' )
			{
				tmpOuput.replace( idx, idx + 1, "" );
			}
		}
		std::ofstream wsdlFile( ( output + ".wsdl" ).c_str() );
		wsdlFile << tmpOuput;
		return EXIT_SUCCESS;
	}
	else
	{
		MainWindow * main = new MainWindow();
		main->init( SmartUtil::Ascii2CurrentBuild::doConvert( wsdl, SmartUtil::ConversionCodepage::ANSI ) );
		return app.run();
	}
}
