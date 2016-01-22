#include "BuildStatus.h"
#include "ProgramOptions.h"
#include "BuildStatusReporter.h"

#include <string>
#include <iostream>
#include <exception>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>

using namespace std;
using namespace std::chrono;
using namespace boost::program_options;


namespace {
	void runBuildStatusLoop(BuildStatusReporter& reporter, const vector<string>& statusUris, seconds pollingPeriod)
	{
		vector<BuildStatus> statuses;
		while (true) {
			statuses.clear();
			
			try {
				transform(begin(statusUris), end(statusUris), back_inserter(statuses), queryLastBuildStatus);
			} catch (exception& ex) {
				cerr << "Failed to get build status. Error = " << ex.what() << endl;
			}
			
			BuildStatus aggregateStatus = accumulate(begin(statuses), end(statuses), BuildStatus::unknown, pickMostInterestingStatus);
			reporter.reportBuildStatus(aggregateStatus);
			
			this_thread::sleep_for(pollingPeriod);
		}
	}
}


int main(int argc, char* argv[])
{
	try {
		ProgramOptions opts(argc, argv);
		runBuildStatusLoop(opts.getReporter(), opts.getStatusUris(), opts.getPollingPeriod());
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
