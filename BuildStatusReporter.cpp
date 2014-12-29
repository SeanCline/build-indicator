#include "BuildStatus.h"
#include "BuildStatusReporter.h"

#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

#include <string>
#include <memory>
#include <stdexcept>

using namespace std;
using namespace boost::program_options;


BuildStatusReporter::~BuildStatusReporter()
{

}


void BuildStatusReporter::init(const boost::program_options::variables_map&)
{

}


auto BuildStatusReporter::getOptionsDescription() const -> options_description
{
	return options_description();
}


BuildStatusReporterRegistry& BuildStatusReporterRegistry::instance()
{
	static BuildStatusReporterRegistry reporterRegistry;
	return reporterRegistry;
}


void BuildStatusReporterRegistry::registerReporter(std::unique_ptr<BuildStatusReporter> reporter)
{
	auto reporterName = reporter->getName();
	registry_[reporterName] = move(reporter);
}


auto BuildStatusReporterRegistry::getReporter(const std::string& reporterName) -> BuildStatusReporter&
{
	try {
		return *registry_.at(reporterName);
	} catch (out_of_range&) {
		throw runtime_error("Unknown reporter: \"" + reporterName + "\"");
	}
}


BuildStatusRegistrar::BuildStatusRegistrar(unique_ptr<BuildStatusReporter> reporter)
{
	BuildStatusReporterRegistry::instance().registerReporter(move(reporter));
}
