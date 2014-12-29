#pragma once

#include <string>

enum class BuildStatus {
	building,
	successful,
	failed,
	unknown
};

std::string to_string(BuildStatus);

BuildStatus queryLastBuildStatus(const std::string& statusUrl);
