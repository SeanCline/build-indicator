#include "UnicornHatReporter.h"
#include "BuildStatusReporter.h"
#include "BuildStatus.h"
#include "AsyncAnimationPlayer.h"

#include <Gif.h>

#include <boost/program_options.hpp>

#include <memory>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
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
		("brightness",   value<double>()->default_value(.2), "Unicorn Hat brightness (0 to 1)")
		("boot-gif",     value<string>()->default_value("UnicornHatReporter/boot.gif"), "GIF to show when starting up.")
		("success-gif",  value<string>()->default_value("UnicornHatReporter/successful.gif"), "GIF to show when build succeeded.")
		("failed-gif",   value<string>()->default_value("UnicornHatReporter/failed.gif"), "GIF to show when build failed.")
		("building-unknown-gif", value<string>()->default_value("UnicornHatReporter/building.gif"), "GIF to show when currently building the last status is not known yet..")
		("building-success-gif", value<string>()->default_value("UnicornHatReporter/building-success.gif"), "GIF to show when currently building and the last build succeeded.")
		("building-failed-gif", value<string>()->default_value("UnicornHatReporter/building-failed.gif"), "GIF to show when currently building and the last build failed.")
		("unknown-gif",  value<string>()->default_value("UnicornHatReporter/unknown.gif"), "GIF to show when build status is unknown.")
	;
	
	return desc;
}


void UnicornHatReporter::init(const variables_map& options)
{
	options_ = options;

	auto bootGif = options_["boot-gif"].as<string>();
	player_ = make_unique<AsyncAnimationPlayer>();
	player_->setBrightness(options_["brightness"].as<double>());
	
	auto fut = player_->playAnimation(Gif::fromFile(bootGif).getAnimation());
	fut.wait_for(3000ms); //< Wait for the animation to end or time out after a few seconds.
}


void UnicornHatReporter::reportBuildStatus(const BuildStatus& status)
{
	if (!player_) {
		throw logic_error("UnicornHatReporter::init not called before reportBuildStatus.");
	}

	if (lastBuildStatus_ == status && status != BuildStatus::unknown) {
		return; //< Nothing to do.
	}

	auto statusGif = getGifFromStatus(status);
	player_->playAnimation(statusGif.getAnimation());

	lastBuildStatus_ = status;
}


auto UnicornHatReporter::getGifFromStatus(const BuildStatus& status) const -> Gif
{
	string gifFilename = "";

	switch (status) {
	case BuildStatus::building:
		if (lastBuildStatus_ == BuildStatus::successful) {
			gifFilename = options_["building-success-gif"].as<string>();
		} else if (lastBuildStatus_ == BuildStatus::failed) {
			gifFilename = options_["building-failed-gif"].as<string>();
		} else {
			gifFilename = options_["building-unknown-gif"].as<string>();
		}
		break;
	case BuildStatus::successful:
		gifFilename = options_["success-gif"].as<string>();
		break;
	case BuildStatus::failed:
		gifFilename = options_["failed-gif"].as<string>();
		break;
	default:
		gifFilename = options_["unknown-gif"].as<string>();
		break;
	}
	
	return Gif::fromFile(gifFilename);
}
