#pragma once
#include "BuildStatusReporter.h"
#include "SignalTower.h"

class PwmReporter : public BuildStatusReporter
{
public:
	PwmReporter(int redPin, int yellowPin, int greenPin, int fadeTime);
	void reportBuildStatus(const BuildStatus&) override;
	
private:
	SignalTower lights_;
};