#pragma once
#include "BuildStatusReporter.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <string>
#include <chrono>

class ProgramOptions
{
public: // Construction/Destruction.
	ProgramOptions(int argc, char* argv[]);
	~ProgramOptions();
	
private: // Non-copyable.
	ProgramOptions(const ProgramOptions&) = delete;
	ProgramOptions& operator=(const ProgramOptions&) = delete;
	
public: // Methods.
	void printHelp() const;
	auto isHelpRequested() const -> bool;
	auto getReporterName() const -> std::string;
	auto getReporter() const -> BuildStatusReporter&;
	auto getStatusUri() const -> std::string;
	auto getPollingPeriod() -> std::chrono::seconds;
	
private: // Helpers.
	void initReporter(int argc, char* argv[]);
	
private: // Data.
	boost::program_options::variables_map varsMap_;
	boost::program_options::options_description optionsDesc_;
};
