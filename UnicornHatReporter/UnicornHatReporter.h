#pragma once
#include "BuildStatusReporter.h"
#include "BuildStatus.h"
#include "AsyncAnimationPlayer.h"

class UnicornHatReporter : public BuildStatusReporter
{
public:
	UnicornHatReporter();
	void reportBuildStatus(const BuildStatus&) override;

private:
	AsyncAnimationPlayer player_;
	BuildStatus lastBuildStatus_ = BuildStatus::unknown;
};
