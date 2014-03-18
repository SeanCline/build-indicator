#include "SignalTower.h"
#include "BuildStatus.h"

#include <wiringPi.h>

#include <boost/program_options.hpp>

#include <string>
#include <iostream>
#include <exception>
#include <chrono>
#include <thread>

using namespace std;

boost::program_options::variables_map getProgramOptions(int argc, char* argv[])
{
	namespace po = boost::program_options;
	
	// Declare the possible command line args.
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Print this help message.")
		("status-uri", po::value<string>()->required(), "The Jenkins '/api/json' url to periodically extract build status from.")
		("polling-period,t", po::value<int>()->default_value(30), "Seconds between status updates from the Jenkins server.")
		("red-pin,r", po::value<int>()->default_value(8), "GPIO pin number for red light.")
		("yellow-pin,y", po::value<int>()->default_value(9), "GPIO pin number for yellow light.")
		("green-pin,g", po::value<int>()->default_value(7), "GPIO pin number for green light.")
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


void runBuildStatusLoop(const boost::program_options::variables_map& opts)
{
		wiringPiSetup();
		
		int r = opts["red-pin"].as<int>();
		int y = opts["yellow-pin"].as<int>();
		int g = opts["green-pin"].as<int>();
		SignalTower lights(r, y, g);
		lights.runSelfTest();
		
		string statusUri(opts["status-uri"].as<string>());
		chrono::seconds pollingPeriod(opts["polling-period"].as<int>());
		while (true) {
		
			// Get the build status from the server.
			cout << "Polling for build status." << endl;
			
			BuildStatus status = BuildStatus::unknown;
			try {
				status = queryLastBuildStatus(statusUri);
			} catch (exception& ex) {
				cerr << "Failed to get build status." << endl;
			}
			
			// Update the lights to match the build status.
			lights.turnAllOff();
			switch(status) {
				case BuildStatus::building:
					lights.setYellowState(SignalTower::LightState::pulsing);
					break;
				case BuildStatus::successful:
					lights.setGreenState(SignalTower::LightState::solid, 100);
					break;
				case BuildStatus::failed:
					lights.setRedState(SignalTower::LightState::solid, 100);
					break;
				case BuildStatus::unknown:
					lights.setRedState(SignalTower::LightState::pulsing);
					break;
			}
			
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
