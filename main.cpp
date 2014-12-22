#include "BuildStatus.h"
#include "ProgramOptions.h"
#include "BuildStatusReporter.h"



#include <boost/program_options/errors.hpp>
#include <boost/program_options/variables_map.hpp>

#include <string>
#include <iostream>
#include <exception>
#include <chrono>
#include <thread>

using namespace std;

void runBuildStatusLoop(const boost::program_options::variables_map& opts)
{
		auto reporter = BuildStatusReporterRegistry::instance().instantiateReporter(opts["reporter"].as<string>(), opts);
		string statusUri(opts["status-uri"].as<string>());
		chrono::seconds pollingPeriod(opts["polling-period"].as<int>());
		while (true) {
		
			// Get the build status from the server.
			cout << "Polling for build status." << endl;
			
			BuildStatus status = BuildStatus::unknown;
			try {
				status = queryLastBuildStatus(statusUri);
			} catch (exception& ex) {
				cerr << "Failed to get build status. Error = " << ex.what() << endl;
			}
			
			reporter->reportBuildStatus(status);
			
			this_thread::sleep_for(pollingPeriod);
		}
}


int main(int argc, char* argv[])
{
	try {		
		auto opts = getProgramOptions(argc, argv);
		runBuildStatusLoop(opts);
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
