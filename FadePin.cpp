#include "FadePin.h"

#include <wiringPi.h>
#include <softPwm.h>

FadePin::FadePin(int pinNum)
	: pinNumber_(pinNum),
	  desiredValue_(0),
	  actualValue_(0)
{
	wiringPiSetup();
	softPwmCreate(pinNumber_, 0, 100);
	softPwmWrite(pinNumber_, actualValue_);
}

void FadePin::tick()
{
	bool valueChanged = false;
	
	// Set the actual value one closer to the desired. (Do nothing if equal.)
	if (actualValue_ < desiredValue_) {
		++actualValue_;
		valueChanged = true;
	} else if (actualValue_ > desiredValue_) {
		--actualValue_;
		valueChanged = true;
	}
	
	if (valueChanged) { // No need to update the pin if we didn't change anything.
		softPwmWrite(pinNumber_, actualValue_);
	}
}


int FadePin::getDesiredValue()
{
	return desiredValue_;
}


void FadePin::setDesiredValue(int value)
{
	desiredValue_ = value;
}


int FadePin::getActualValue()
{
	return actualValue_;
}


void FadePin::setActualValue(int value)
{
	actualValue_ = desiredValue_ = value;
}
