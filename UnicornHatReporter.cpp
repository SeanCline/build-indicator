#include "UnicornHatReporter.h"
#include "BuildStatusReporter.h"
#include "BuildStatus.h"

#include <UnicornHat.h>
#include <Gif.h>

#include <boost/program_options/variables_map.hpp>

#include <memory>
#include <iostream>

using namespace std;
using namespace boost::program_options;
using namespace Gif2UnicornHat;

// Register ourselves as a reporter.
namespace {
	BuildStatusRegistrar UnicornHatRegistrar("UnicornHat", [](const variables_map& options) -> unique_ptr<UnicornHatReporter> {
		return unique_ptr<UnicornHatReporter>(new UnicornHatReporter());
	});
}


UnicornHatReporter::UnicornHatReporter()
{
	auto& hat = UnicornHat::instance();
	hat.showImage(Gif::fromFile("boot.gif").getStaticImage());
}


void UnicornHatReporter::reportBuildStatus(const BuildStatus& status)
{
	auto& hat = UnicornHat::instance();
	switch(status) {
	case BuildStatus::building:
		hat.showImage(Gif::fromFile("building.gif").getStaticImage());
		break;
	case BuildStatus::successful:
		hat.showImage(Gif::fromFile("successful.gif").getStaticImage());
		break;
	case BuildStatus::failed:
		hat.showImage(Gif::fromFile("failed.gif").getStaticImage());
		break;
	case BuildStatus::unknown:
		hat.showImage(Gif::fromFile("unknown.gif").getStaticImage());
		break;
	default:
		hat.showImage(Gif::fromFile("unknown.gif").getStaticImage());
		break;		
	}
}
