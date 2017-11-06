#include "ConsoleReporter.h"
#include "BuildStatusReporter.h"
#include "BuildStatus.h"

#include <boost/program_options.hpp>

#include <iostream>

using namespace std;
using namespace boost::program_options;

// Register ourselves as a reporter.
namespace {
	BuildStatusRegistrar ConsoleRegistrar(make_unique<ConsoleReporter>());
}


auto ConsoleReporter::getName() const -> std::string
{
	return "Console";
}


void ConsoleReporter::init(const variables_map&)
{
	cout << "Console reporter initted." << endl;
}


void ConsoleReporter::reportBuildStatus(const BuildStatus& st)
{
	cout << "Build status: " << st.status << ":" << st.isCurrentlyBuilding << " - " << to_string(st) << endl;
}
