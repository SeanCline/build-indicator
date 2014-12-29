#include "ProgramOptions.h"

#include <string>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

variables_map getProgramOptions(int argc, char* argv[])
{
	// Declare the possible command line args.
	options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print this help message.")
		("reporter", value<string>()->required(), "The module that will be used to report build status. (e.g. PWM, UnicornHat)")
		("status-uri", value<string>()->required(), "The Jenkins '/api/json' url to periodically extract build status from.")
		("polling-period,t", value<int>()->default_value(30), "Seconds between status updates from the Jenkins server.")
	;

	variables_map opts;
	auto parsedOpts = command_line_parser(argc, argv).options(desc).allow_unregistered().run();      
	store(parsedOpts, opts);

	// Process the help option.
	if (opts.count("help") > 0) 
	{ 
		cout << desc << endl; 
		exit(0);
	}
	
	notify(opts);
	return opts;
}