#include "UnicornHatReporter.h"
#include "BuildStatusReporter.h"
#include "BuildStatus.h"
#include "AsyncAnimationPlayer.h"

#include <Gif.h>

#include <boost/program_options/variables_map.hpp>

#include <memory>
#include <iostream>

using namespace std;
using namespace boost::program_options;
using namespace Gif2UnicornHat;

// Register ourselves as a reporter.
namespace {
	BuildStatusRegistrar UnicornHatRegistrar("UnicornHat", [](const variables_map& options) -> unique_ptr<UnicornHatReporter> {
		return unique_ptr<UnicornHatReporter>(new UnicornHatReporter());
	});
}


UnicornHatReporter::UnicornHatReporter()
{
	player_.playAnimation(Gif::fromFile("UnicornHatReporter/boot.gif").getAnimation());
}


void UnicornHatReporter::reportBuildStatus(const BuildStatus& status)
{
	if (lastBuildStatus_ == status && status != BuildStatus::unknown) {
		return; //< Nothing to do.
	}
	lastBuildStatus_ = status;
	
	switch(status) {
	case BuildStatus::building:
		player_.playAnimation(Gif::fromFile("UnicornHatReporter/building.gif").getAnimation());
		break;
	case BuildStatus::successful:
		player_.playAnimation(Gif::fromFile("UnicornHatReporter/successful.gif").getAnimation());
		break;
	case BuildStatus::failed:
		player_.playAnimation(Gif::fromFile("UnicornHatReporter/failed.gif").getAnimation());
		break;
	case BuildStatus::unknown:
		player_.playAnimation(Gif::fromFile("UnicornHatReporter/unknown.gif").getAnimation());
		break;
	default:
		player_.playAnimation(Gif::fromFile("UnicornHatReporter/unknown.gif").getAnimation());
		break;		
	}
}
