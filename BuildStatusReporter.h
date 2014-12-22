#pragma once
#include <string>
#include <functional>
#include <memory>
#include <map>

#include <boost/program_options/variables_map.hpp>


enum class BuildStatus;


class BuildStatusReporter {
public:
	virtual ~BuildStatusReporter();
	virtual void reportBuildStatus(const BuildStatus&) = 0;
};


typedef std::function<std::unique_ptr<BuildStatusReporter>(const boost::program_options::variables_map&)> BuildStatusFactory;


class BuildStatusReporterRegistry {
public: // Singleton.
	static BuildStatusReporterRegistry& instance();

public:	// Registry methods.
	void registerReporter(const std::string& reporterName, BuildStatusFactory factoryFunction);
	std::unique_ptr<BuildStatusReporter> instantiateReporter(const std::string& reporterName,
		const boost::program_options::variables_map& options = boost::program_options::variables_map());
	
private: // Data.
	std::map<std::string, BuildStatusFactory> registry_;
};


struct BuildStatusRegistrar {
	BuildStatusRegistrar(const std::string& reporterName, BuildStatusFactory factoryFunction);
};
