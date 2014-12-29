#include "BuildStatus.h"
#include "ProgramOptions.h"
#include "BuildStatusReporter.h"

#include <boost/program_options.hpp>

#include <string>
#include <iostream>
#include <exception>
#include <chrono>
#include <thread>

using namespace std;
using namespace boost::program_options;

namespace {
	void runBuildStatusLoop(const boost::program_options::variables_map& opts, BuildStatusReporter& reporter)
	{		
			string statusUri(opts["status-uri"].as<string>());
			chrono::seconds pollingPeriod(opts["polling-period"].as<int>());
			while (true) {
				BuildStatus status = BuildStatus::unknown;
				try {
					status = queryLastBuildStatus(statusUri);
				} catch (exception& ex) {
					cerr << "Failed to get build status. Error = " << ex.what() << endl;
				}
				
				reporter.reportBuildStatus(status);
				
				this_thread::sleep_for(pollingPeriod);
			}
	}


	BuildStatusReporter& getReporter(const string& reporterName, int argc, char* argv[]) //< TODO: Move into ProgramOptions.
	{
		auto& reporter = BuildStatusReporterRegistry::instance().getReporter(reporterName);

		variables_map reporterOpts;
		auto desc = reporter.getOptionsDescription();
		auto parsedOpts = command_line_parser(argc, argv).options(desc).allow_unregistered().run();      
		store(parsedOpts, reporterOpts);
		notify(reporterOpts);
		
		reporter.init(reporterOpts);
		
		return reporter;
	}
}

int main(int argc, char* argv[])
{
	try {
		auto opts = getProgramOptions(argc, argv);
		auto& reporter = getReporter(opts["reporter"].as<string>(), argc, argv);
		runBuildStatusLoop(opts, reporter);
	} catch(boost::program_options::error& ex) {
		cerr << "Commandline error: " << ex.what() << endl;
		return 1;
	} catch (exception& ex) {
		cerr << "Exception unwound to main. Details: " << ex.what() << endl;
		throw;
	} catch(...) {
		cerr << "Unknown exception unwound to main." << endl;
		throw;
	}
}
