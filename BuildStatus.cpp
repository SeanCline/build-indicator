#include "BuildStatus.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <string>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace boost::property_tree;


auto operator==(const BuildStatus& lhs, const BuildStatus& rhs) -> bool
{
	return (lhs.status == rhs.status) && (lhs.isCurrentlyBuilding == rhs.isCurrentlyBuilding);
}

auto to_string(const BuildStatus& st) -> string
{
	string statusStr;
	if (st.isCurrentlyBuilding) {
		statusStr += "building_";
	}
	
	switch(st.status) {
		case BuildStatus::success: statusStr += "success"; break;
		case BuildStatus::failure: statusStr += "failure"; break;
		case BuildStatus::unknown: statusStr += "unknown"; break;
		default: throw runtime_error("Unexpected value for BuildStatus.");
	}
	
	return statusStr;
}

namespace {
	const static map<string, BuildStatus> colorToBuildStatus {
		{"blue",       { BuildStatus::success, false} },
		{"red",        { BuildStatus::failure, false} },
		{"grey",       { BuildStatus::unknown, false} },
		{"blue_anime", { BuildStatus::success, true} },
		{"red_anime",  { BuildStatus::failure, true} },
		{"grey_anime", { BuildStatus::unknown, true} }
	};
		
	BuildStatus getBuildStatusFromBallColor(const string& ballColorName)
	{
		auto statusIt = colorToBuildStatus.find(boost::to_lower_copy(ballColorName));
		if (statusIt != end(colorToBuildStatus)) {
			return statusIt->second;
		} else {
			return { BuildStatus::unknown, false };
		}
	}
}

auto queryLastBuildStatus(const string& statusUrl) -> BuildStatus
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

auto combineBuildStatus(const BuildStatus& s1, const BuildStatus& s2) -> BuildStatus
{
	BuildStatus combinedStatus;
	combinedStatus.status = min(s1.status, s2.status); //< Pick the worst status.
	combinedStatus.isCurrentlyBuilding = s1.isCurrentlyBuilding | s2.isCurrentlyBuilding; //< Apply the building flag to the status.
	return combinedStatus;
}
