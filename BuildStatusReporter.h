#pragma once
#include <string>
#include <memory>
#include <map>

#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

enum class BuildStatus;

class BuildStatusReporter {
public:
	virtual ~BuildStatusReporter();
	virtual auto getName() const -> std::string = 0; //< Retrieves the name of the reporter.
	virtual auto getOptionsDescription() const -> boost::program_options::options_description; //< Retrieves the parameters requested by the reporter.
	virtual void init(const boost::program_options::variables_map& options); //< Prepares the reporter to receive build statuses.
	virtual void reportBuildStatus(const BuildStatus&) = 0; //< Informs the reporter of the latest build status.
};


// Keeps track of all the registered reporters.
class BuildStatusReporterRegistry {
public: // Singleton.
	static BuildStatusReporterRegistry& instance();

public:	// Registry methods.
	void registerReporter(std::unique_ptr<BuildStatusReporter> reporter);
	auto getReporter(const std::string& reporterName) -> BuildStatusReporter&;
	
private: // Data.
	std::map<std::string, std::unique_ptr<BuildStatusReporter>> registry_;
};


// A helper to register reporters by declaring a statically allocated variable in a compilation unit.
struct BuildStatusRegistrar {
	BuildStatusRegistrar(std::unique_ptr<BuildStatusReporter> reporter);
};
