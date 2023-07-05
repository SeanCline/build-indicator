#include "UnicornHatReporter.h"
#include "BuildStatusReporter.h"
#include "BuildStatus.h"
#include "AsyncAnimationPlayer.h"

#include <Gif.h>
#include <UnicornHat.h>

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
		("failure-gif",   value<string>()->default_value("UnicornHatReporter/failed.gif"), "GIF to show when build failed.")
		("building-unknown-gif", value<string>()->default_value("UnicornHatReporter/building.gif"), "GIF to show when currently building the last status is not known yet..")
		("building-success-gif", value<string>()->default_value("UnicornHatReporter/building-success.gif"), "GIF to show when currently building and the last build succeeded.")
		("building-failure-gif", value<string>()->default_value("UnicornHatReporter/building-failed.gif"), "GIF to show when currently building and the last build failed.")
		("unknown-gif",  value<string>()->default_value("UnicornHatReporter/unknown.gif"), "GIF to show when build status is unknown.")
	;
	
	return desc;
}


void UnicornHatReporter::init(const variables_map& options)
{
	options_ = options;
	hat_ = std::unique_ptr<UnicornHat>(new UnicornHat);
	
	auto bootGif = options_["boot-gif"].as<string>();
	player_ = make_unique<AsyncAnimationPlayer>();
	player_->setBrightness(*hat_, options_["brightness"].as<double>());
	
	auto fut = player_->playAnimation(*hat_, Gif::fromFile(bootGif).getAnimation());
	fut.wait_for(3000ms); //< Wait for the animation to end or time out after a few seconds.
}


void UnicornHatReporter::reportBuildStatus(const BuildStatus& st)
{
	if (!player_) {
		throw logic_error("UnicornHatReporter::init not called before reportBuildStatus.");
	}

	if (lastBuildStatus_ == st && st.status != BuildStatus::unknown) {
		return; //< Nothing to do.
	}

	auto statusGif = getGifFromStatus(st);
	player_->playAnimation(*hat_, statusGif.getAnimation());

	lastBuildStatus_ = st;
}


auto UnicornHatReporter::getGifFromStatus(const BuildStatus& st) const -> Gif
{
	const string gifFileKey = [&]() {
		switch (st.status) {
		case BuildStatus::success: return st.isCurrentlyBuilding ? "building-success-gif" : "success-gif";
		case BuildStatus::failure: return st.isCurrentlyBuilding ? "building-failure-gif" : "failure-gif";
		default:                   return st.isCurrentlyBuilding ? "building-unknown-gif" : "unknown-gif";
		}
	}();
	
	const string gifFilename = options_[gifFileKey].as<string>();
	return Gif::fromFile(gifFilename);
}
