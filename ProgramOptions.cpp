#include "ProgramOptions.h"

#include <string>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

po::variables_map getProgramOptions(int argc, char* argv[])
{
	// Declare the possible command line args.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print this help message.")
		("reporter", po::value<string>()->required(), "The module that will be used to report build status. (e.g. PWM, UnicornHat)")
		("status-uri", po::value<string>()->required(), "The Jenkins '/api/json' url to periodically extract build status from.")
		("polling-period,t", po::value<int>()->default_value(30), "Seconds between status updates from the Jenkins server.")
		("red-pin,r", po::value<int>()->default_value(8), "GPIO pin number for red light.")
		("yellow-pin,y", po::value<int>()->default_value(9), "GPIO pin number for yellow light.")
		("green-pin,g", po::value<int>()->default_value(7), "GPIO pin number for green light.")
		("fade-period,p", po::value<int>()->default_value(500), "Millisecond period for pulsing LEDs to blink.")
	;

	po::variables_map opts;
	po::store(po::parse_command_line(argc, argv, desc), opts);

	// Process the help option.
	if (opts.count("help") > 0) 
	{ 
		cout << desc << endl; 
		exit(0);
	}
	
	po::notify(opts);
	return opts;
}