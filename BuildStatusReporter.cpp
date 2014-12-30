#include "BuildStatus.h"
#include "BuildStatusReporter.h"

#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>

using namespace std;
using namespace boost::program_options;


BuildStatusReporter::~BuildStatusReporter()
{

}


void BuildStatusReporter::init(const boost::program_options::variables_map&)
{

}


auto BuildStatusReporter::getHelpString() const -> string
{
	ostringstream oss;
	oss << getOptionsDescription();
	return oss.str();
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


auto BuildStatusReporterRegistry::getHelpString() -> std::string
{
	ostringstream oss;
	for (auto& reporterPair : registry_) {
		auto helpString = reporterPair.second->getHelpString();
		if (!helpString.empty()) { 
			oss << helpString << endl;
		}
	}
	return oss.str();
}


BuildStatusRegistrar::BuildStatusRegistrar(unique_ptr<BuildStatusReporter> reporter)
{
	BuildStatusReporterRegistry::instance().registerReporter(move(reporter));
}
