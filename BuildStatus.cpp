#include "BuildStatus.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <string>
#include <sstream>

using namespace std;
using namespace boost::property_tree;

BuildStatus queryLastBuildStatus(const string& statusUrl)
{
	// Pull the status json from the server.
	stringstream ss;
	ss << curlpp::options::Url(statusUrl);
	
	// Parse the json document with help of our favourite json parser...
	ptree pt;
	read_json(ss, pt);
	
	bool isBuilding = pt.get<bool>("building");
	if (isBuilding) {
		return BuildStatus::building;
	}
	
	string buildResult = pt.get<string>("result");
	if (buildResult == "SUCCESS") {
		return BuildStatus::successful;
	} else if (buildResult == "FAILURE") {
		return BuildStatus::failed;
	} else {
		return BuildStatus::unknown;
	}
}
