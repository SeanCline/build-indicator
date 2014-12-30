#include "UnicornHatReporter.h"
#include "BuildStatusReporter.h"
#include "BuildStatus.h"
#include "AsyncAnimationPlayer.h"

#include <Gif.h>

#include <boost/program_options.hpp>

#include <memory>
#include <iostream>

using namespace std;
using namespace boost::program_options;
using namespace Gif2UnicornHat;

// Register ourselves as a reporter.
namespace {
	BuildStatusRegistrar UnicornHatRegistrar(make_unique<UnicornHatReporter>());
}


auto UnicornHatReporter::getName() const -> std::string
{
	return "UnicornHat";
}


auto UnicornHatReporter::getOptionsDescription() const -> options_description
{
	options_description desc("UnicornHat Reporter options");
	desc.add_options()
		("boot-gif",      value<string>()->default_value("UnicornHatReporter/boot.gif"),       "GIF to show when starting up.")
		("success-gif",   value<string>()->default_value("UnicornHatReporter/successful.gif"), "GIF to show when build succeeded.")
		("failed-gif",    value<string>()->default_value("UnicornHatReporter/failed.gif"),     "GIF to show when build failed.")
		("building-gif",  value<string>()->default_value("UnicornHatReporter/building.gif"),   "GIF to show when currently building.")
		("unknown-gif",   value<string>()->default_value("UnicornHatReporter/unknown.gif"),    "GIF to show when build status is unknown.")
	;
	
	return desc;
}


void UnicornHatReporter::init(const variables_map& options)
{
	options_ = options;

	auto bootGif = options_["boot-gif"].as<string>();
	player_ = make_unique<AsyncAnimationPlayer>();
	player_->playAnimation(Gif::fromFile(bootGif).getAnimation());
}


void UnicornHatReporter::reportBuildStatus(const BuildStatus& status)
{
	if (!player_) {
		throw logic_error("UnicornHatReporter::init not called before reportBuildStatus.");
	}

	if (lastBuildStatus_ == status && status != BuildStatus::unknown) {
		return; //< Nothing to do.
	}
	lastBuildStatus_ = status;
	
	switch(status) {
	case BuildStatus::building:
		player_->playAnimation(Gif::fromFile(options_["boot-gif"].as<string>()).getAnimation());
		break;
	case BuildStatus::successful:
		player_->playAnimation(Gif::fromFile(options_["success-gif"].as<string>()).getAnimation());
		break;
	case BuildStatus::failed:
		player_->playAnimation(Gif::fromFile(options_["failed-gif"].as<string>()).getAnimation());
		break;
	case BuildStatus::unknown:
		player_->playAnimation(Gif::fromFile(options_["unknown-gif"].as<string>()).getAnimation());
		break;
	default:
		player_->playAnimation(Gif::fromFile(options_["unknown-gif"].as<string>()).getAnimation());
		break;
	}
}
