#include "PwmReporter.h"
#include "BuildStatusReporter.h"
#include "BuildStatus.h"
#include <UnicornHat.h>
#include <memory>

#include <boost/program_options/variables_map.hpp>

#include <memory>
#include <iostream>

using namespace std;
using namespace std::chrono;
using namespace boost::program_options;


// Register ourselves as a reporter.
namespace {
	BuildStatusRegistrar PwmRegistrar("PWM", [](const variables_map& options) -> unique_ptr<PwmReporter> {
		int r = options["red-pin"].as<int>();
		int y = options["yellow-pin"].as<int>();
		int g = options["green-pin"].as<int>();
		int fadeTime = options["fade-period"].as<int>();
		return unique_ptr<PwmReporter>(new PwmReporter(r, y, g, fadeTime));
	});
}


PwmReporter::PwmReporter(int redPin, int yellowPin, int greenPin, int fadeTime)
	 : lights_(redPin, yellowPin, greenPin, milliseconds(fadeTime))
{
	lights_.runSelfTest();
}


void PwmReporter::reportBuildStatus(const BuildStatus& status)
{
	lights_.turnAllOff();
	switch(status) {
		case BuildStatus::building:
			lights_.setYellowState(SignalTower::LightState::pulsing);
			break;
		case BuildStatus::successful:
			lights_.setGreenState(SignalTower::LightState::solid, 100);
			break;
		case BuildStatus::failed:
			lights_.setRedState(SignalTower::LightState::solid, 100);
			break;
		case BuildStatus::unknown:
			lights_.setRedState(SignalTower::LightState::pulsing);
			break;
	}
}