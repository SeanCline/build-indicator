#pragma once
#include "BuildStatusReporter.h"

class UnicornHatReporter : public BuildStatusReporter
{
public:
	UnicornHatReporter();
	void reportBuildStatus(const BuildStatus&) override;
};