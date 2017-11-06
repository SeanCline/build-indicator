#pragma once
#include "BuildStatusReporter.h"
#include "BuildStatus.h"

#include <Gif.h>

#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

#include <memory>

class AsyncAnimationPlayer;

class UnicornHatReporter : public BuildStatusReporter
{
public:
	auto getName() const -> std::string override;
	auto getOptionsDescription() const -> boost::program_options::options_description override;
	void init(const boost::program_options::variables_map& options) override;
	void reportBuildStatus(const BuildStatus&) override;

private:
	auto getGifFromStatus(const BuildStatus& status) const -> Gif2UnicornHat::Gif;

private:
	std::unique_ptr<AsyncAnimationPlayer> player_;
	BuildStatus lastBuildStatus_ = {BuildStatus::unknown, false};
	boost::program_options::variables_map options_;
};
