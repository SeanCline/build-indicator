#include "BuildStatus.h"
#include "BuildStatusReporter.h"

#include <boost/program_options/variables_map.hpp>

#include <string>
#include <functional>
#include <memory>
#include <stdexcept>

using namespace std;
using namespace boost::program_options;


BuildStatusReporter::~BuildStatusReporter()
{
}


BuildStatusReporterRegistry& BuildStatusReporterRegistry::instance()
{
	static BuildStatusReporterRegistry reporterRegistry;
	return reporterRegistry;
}


void BuildStatusReporterRegistry::registerReporter(const string& reporterName, BuildStatusFactory factoryFunction)
{
	registry_[reporterName] = factoryFunction;
}


unique_ptr<BuildStatusReporter> BuildStatusReporterRegistry::instantiateReporter(const string& reporterName, const variables_map& options)
{
	try {
		return registry_.at(reporterName)(options);
	} catch (...) {
		throw runtime_error("Registry does not have a factory registered as \"" + reporterName + "\"");
	}
}


BuildStatusRegistrar::BuildStatusRegistrar(const string& reporterName, BuildStatusFactory factoryFunction)
{
	BuildStatusReporterRegistry::instance().registerReporter(reporterName, factoryFunction);
}
