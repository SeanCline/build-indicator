#pragma once

#include <string>

enum class BuildStatus { // Sorted in order of how important they are to present to the user.
	building_failure,
	building_success,
	failure,
	building_unknown,
	success,
	unknown
};

std::string to_string(BuildStatus);

BuildStatus queryLastBuildStatus(const std::string& statusUrl);

BuildStatus pickMostInterestingStatus(BuildStatus s1, BuildStatus s2);