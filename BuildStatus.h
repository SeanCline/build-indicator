#pragma once

#include <string>

enum class BuildStatus {
	building,
	successful,
	failed,
	unknown
};

BuildStatus queryLastBuildStatus(const std::string& statusUrl);
