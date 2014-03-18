#include "SignalTower.h"

#include "FadePin.h"

#include <thread>
#include <atomic>
#include <chrono>

using namespace std;
using namespace std::chrono;

SignalTower::SignalTower(int redPinNumber, int yellowPinNumber, int greenPinNumber, std::chrono::milliseconds fadeTime)
	: redPin_(redPinNumber),
	  yellowPin_(yellowPinNumber),
	  greenPin_(greenPinNumber),
	  exitThreadFlag_(false),
	  fadeTime_(fadeTime)
{
	updateThread_ = thread(bind(&SignalTower::runUpdateLoop, this));
}


SignalTower::~SignalTower()
{
	turnAllOff(false);
	exitThreadFlag_ = true;
	
	if (updateThread_.joinable())
		updateThread_.join();
}


namespace {
	void updatePin(FadePin& pin, SignalTower::LightState& state)
	{
		pin.tick();
		
		// See if we needs to reverse directions while pulsing.
		if (state == SignalTower::LightState::pulsing && pin.getDesiredValue() == pin.getActualValue()) {
			// Reverse fade direction.
			if (pin.getDesiredValue() != 0) {
				pin.setDesiredValue(0);
			} else {
				pin.setDesiredValue(100);
			}
		}
	}
}


void SignalTower::runUpdateLoop()
{	
	while (!exitThreadFlag_) {
		{
			lock_guard<mutex> lock(mutex_);
			updatePin(redPin_, redState_);
			updatePin(yellowPin_, yellowState_);
			updatePin(greenPin_, greenState_);
		}
		
		this_thread::sleep_for(fadeTime_ / 100);
	}
}


void SignalTower::setRedState(LightState state, int brightness, bool fade)
{
	lock_guard<mutex> lock(mutex_);
	redState_ = state;
	if (fade) {
		redPin_.setDesiredValue(brightness);
	} else {
		redPin_.setActualValue(brightness);
	}
}


void SignalTower::setYellowState(LightState state, int brightness, bool fade)
{
	lock_guard<mutex> lock(mutex_);
	yellowState_ = state;
	if (fade) {
		yellowPin_.setDesiredValue(brightness);
	} else {
		yellowPin_.setActualValue(brightness);
	}
}


void SignalTower::setGreenState(LightState state, int brightness, bool fade)
{
	lock_guard<mutex> lock(mutex_);
	greenState_ = state;
	if (fade) {
		greenPin_.setDesiredValue(brightness);
	} else {
		greenPin_.setActualValue(brightness);
	}
}


void SignalTower::turnAllOff(bool fade)
{
	setRedState(LightState::solid, 0, fade);
	setYellowState(LightState::solid, 0, fade);
	setGreenState(LightState::solid, 0, fade);
}


void SignalTower::runSelfTest()
{
	turnAllOff(false);
	
	setRedState(LightState::solid, 100);
	this_thread::sleep_for(fadeTime_ / 2);
	
	setYellowState(LightState::solid, 100);
	this_thread::sleep_for(fadeTime_ / 2);
	
	setGreenState(LightState::solid, 100);
	this_thread::sleep_for(fadeTime_);
	
	turnAllOff();
}