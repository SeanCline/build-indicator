#pragma once
#include "BuildStatusReporter.h"
#include "BuildStatus.h"

#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

#include <memory>

class SignalTower;

class PwmReporter : public BuildStatusReporter
{
public:
	auto getName() const -> std::string override;
	auto getOptionsDescription() const -> boost::program_options::options_description override;
	void init(const boost::program_options::variables_map& options) override;
	void reportBuildStatus(const BuildStatus&) override;
	
private:
	std::unique_ptr<SignalTower> lights_;
};