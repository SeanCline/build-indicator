#pragma once

#include <string>

struct BuildStatus {
	enum Status : char {
		failure, success, unknown //< In order of how important to report to the user.
	} status;
	
	bool isCurrentlyBuilding;
};

auto operator==(const BuildStatus&, const BuildStatus&) -> bool;

auto to_string(const BuildStatus&) -> std::string;

auto queryLastBuildStatus(const std::string& statusUrl) -> BuildStatus;

auto combineBuildStatus(const BuildStatus& s1, const BuildStatus& s2) -> BuildStatus;
