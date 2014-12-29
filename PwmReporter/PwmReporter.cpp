#include "PwmReporter.h"
#include "BuildStatusReporter.h"
#include "BuildStatus.h"
#include "SignalTower.h"
#include <UnicornHat.h>

#include <boost/program_options.hpp>

#include <memory>
#include <iostream>
#include <stdexcept>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace boost::program_options;


// Register ourselves as a reporter.
namespace {
	BuildStatusRegistrar PwmRegistrar(make_unique<PwmReporter>());
}


auto PwmReporter::getName() const -> std::string
{
	return "PWM";
}


auto PwmReporter::getOptionsDescription() const -> options_description
{
	options_description desc("Pwm Reporter options");
	desc.add_options()
		("red-pin,r",     value<int>()->default_value(8),   "GPIO pin number for red light.")
		("yellow-pin,y",  value<int>()->default_value(9),   "GPIO pin number for yellow light.")
		("green-pin,g",   value<int>()->default_value(7),   "GPIO pin number for green light.")
		("fade-period,p", value<int>()->default_value(500), "Millisecond period for pulsing LEDs to blink.")
	;
	
	return desc;
}


void PwmReporter::init(const variables_map& options)
{
	int redPin = options["red-pin"].as<int>();
	int yellowPin = options["yellow-pin"].as<int>();
	int greenPin = options["green-pin"].as<int>();
	int fadeTime = options["fade-period"].as<int>();
	lights_ = make_unique<SignalTower>(redPin, yellowPin, greenPin, milliseconds(fadeTime));
	
	lights_->runSelfTest();
}


void PwmReporter::reportBuildStatus(const BuildStatus& status)
{
	if (!lights_) {
		throw logic_error("PwmReporter::init not called before reportBuildStatus.");
	}

	lights_->turnAllOff();
	switch(status) {
		case BuildStatus::building:
			lights_->setYellowState(SignalTower::LightState::pulsing);
			break;
		case BuildStatus::successful:
			lights_->setGreenState(SignalTower::LightState::solid, 100);
			break;
		case BuildStatus::failed:
			lights_->setRedState(SignalTower::LightState::solid, 100);
			break;
		case BuildStatus::unknown:
			lights_->setRedState(SignalTower::LightState::pulsing);
			break;
	}
}