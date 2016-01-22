#include "ProgramOptions.h"
#include "BuildStatusReporter.h"

#include <string>
#include <chrono>
#include <vector>
#include <boost/program_options.hpp>

using namespace std;
using namespace std::chrono;
using namespace boost::program_options;


ProgramOptions::ProgramOptions(int argc, char* argv[])
	 : optionsDesc_("Main options")
{
	optionsDesc_.add_options()
		("help,h", "Print this help message.")
		("reporter", value<string>()->required(), "The module that will be used to report build status. (e.g. PWM, UnicornHat)")
		("status-uri", value<std::vector<std::string>>()->multitoken()->composing()->required(), "The Jenkins '/api/json' url to periodically extract build status from.")
		("polling-period,t", value<int>()->default_value(30), "Seconds between status updates from the Jenkins server.")
	;
	
	auto parsedOpts = command_line_parser(argc, argv).options(optionsDesc_).allow_unregistered().run();      
	store(parsedOpts, varsMap_);

	// Process the help option.
	if (isHelpRequested()) 
	{ 
		printHelp();
		exit(0);
	}
	
	notify(varsMap_);
	
	initReporter(argc, argv);
}


ProgramOptions::~ProgramOptions()
{

}


void ProgramOptions::printHelp() const
{
	cout << optionsDesc_ << endl;
	cout << BuildStatusReporterRegistry::instance().getHelpString() << endl;
}


auto ProgramOptions::isHelpRequested() const -> bool
{
	return (varsMap_.count("help") > 0);
}


auto ProgramOptions::getReporterName() const -> string
{
	return varsMap_["reporter"].as<string>();
}


auto ProgramOptions::getReporter() const -> BuildStatusReporter&
{
	return BuildStatusReporterRegistry::instance().getReporter(getReporterName());
}


auto ProgramOptions::getStatusUris() const -> vector<string>
{
	return varsMap_["status-uri"].as<vector<string>>();
}


auto ProgramOptions::getPollingPeriod() -> seconds
{
	return seconds(varsMap_["polling-period"].as<int>());
}


void ProgramOptions::initReporter(int argc, char* argv[])
{
	auto& reporter = getReporter();

	variables_map reporterOpts;
	auto desc = reporter.getOptionsDescription();
	auto parsedOpts = command_line_parser(argc, argv).options(desc).allow_unregistered().run();      
	store(parsedOpts, reporterOpts);
	notify(reporterOpts);
	
	reporter.init(reporterOpts);
}