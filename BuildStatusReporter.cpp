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
	} catch (out_of_range&) {
		throw runtime_error("Unknown reporter: \"" + reporterName + "\"");
	}
}


BuildStatusRegistrar::BuildStatusRegistrar(const string& reporterName, BuildStatusFactory factoryFunction)
{
	BuildStatusReporterRegistry::instance().registerReporter(reporterName, factoryFunction);
}
