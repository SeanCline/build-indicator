#include "BuildStatus.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <string>
#include <map>
#include <sstream>

using namespace std;
using namespace boost::property_tree;


string to_string(BuildStatus status)
{
	switch(status) {
	case BuildStatus::success:          return "success";
	case BuildStatus::failure:          return "failure";
	case BuildStatus::unknown:          return "unknown";
	case BuildStatus::building_success: return "building_success";
	case BuildStatus::building_failure: return "building_failure";
	case BuildStatus::building_unknown: return "building_unknown";
	default: throw runtime_error("Unexpected value for BuildStatus.");
	}
}

namespace {
	BuildStatus getBuildStatusFromBallColor(const string& ballColorName)
	{
		const static map<string, BuildStatus> colorToBuildStatus {
			{"blue", BuildStatus::success},
			{"red", BuildStatus::failure},
			{"grey", BuildStatus::unknown},
			{"blue_anime", BuildStatus::building_success},
			{"red_anime", BuildStatus::building_failure},
			{"grey_anime", BuildStatus::building_unknown}
		};

		auto status = colorToBuildStatus.find(boost::to_lower_copy(ballColorName));
		return (status != end(colorToBuildStatus)) ? status->second : BuildStatus::unknown;
	}
}

BuildStatus queryLastBuildStatus(const string& statusUrl)
{
	// Pull the status json from the server.
	stringstream ss;
	ss << curlpp::options::Url(statusUrl);
	
	// Parse the json document with help of our favourite json parser...
	ptree pt;
	read_json(ss, pt);
	
	string ballColor = pt.get<string>("color");
	return getBuildStatusFromBallColor(ballColor);
}

BuildStatus pickMostInterestingStatus(BuildStatus s1, BuildStatus s2)
{
	return (static_cast<int>(s1) < static_cast<int>(s2)) ? s1 : s2;
}