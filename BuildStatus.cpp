#include "BuildStatus.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <string>
#include <sstream>
#include <array>

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
	BuildStatus getBuildStatusBallColor(const string& ballColorString)
	{
		
		if (boost::iequals(ballColorString, "blue")) {
			return BuildStatus::success;
		} else if (boost::iequals(ballColorString, "red")) {
			return BuildStatus::failure;
		} else if (boost::iequals(ballColorString, "blue_anime")) {
			return BuildStatus::building_success;
		} else if (boost::iequals(ballColorString, "red_anime")) {
			return BuildStatus::building_failure;
		} else if (boost::iequals(ballColorString, "grey_anime")) {
			return BuildStatus::building_unknown;
		} else {
			return BuildStatus::unknown;
		}
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
	
	string buildResult = pt.get<string>("color");
	return getBuildStatusBallColor(buildResult);
}

BuildStatus pickMostInterestingStatus(BuildStatus s1, BuildStatus s2)
{
	return (static_cast<int>(s1) < static_cast<int>(s2)) ? s1 : s2;
}