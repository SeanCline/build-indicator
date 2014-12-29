#pragma once
#include "BuildStatusReporter.h"
#include "BuildStatus.h"

#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

class ConsoleReporter : public BuildStatusReporter
{
public:
	auto getName() const -> std::string override;
	void init(const boost::program_options::variables_map& options) override;
	void reportBuildStatus(const BuildStatus&) override;
};
