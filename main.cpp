#include "BuildStatus.h"
#include "ProgramOptions.h"
#include "BuildStatusReporter.h"

#include <string>
#include <iostream>
#include <exception>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace boost::program_options;


namespace {
	void runBuildStatusLoop(BuildStatusReporter& reporter, const string& statusUri, seconds pollingPeriod)
	{
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
}


int main(int argc, char* argv[])
{
	try {
		ProgramOptions opts(argc, argv);
		runBuildStatusLoop(opts.getReporter(), opts.getStatusUri(), opts.getPollingPeriod());
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
