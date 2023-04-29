#pragma once 

#include <stdint.h>
#include <string>

int32_t str_toi32(const std::string& s) {

	size_t strt = 0;
	int32_t v = 0;
	uint32_t mul = 1;
	bool negative = false;

	if (s[0] == '-') {
		negative = true;
		strt = 1;
	}

	for (strt; strt < s.length(); ++strt) {
		v *= mul;
		v += s[strt] - '0';
		mul *= 10;
	}

	return v;
}

int64_t str_toi64(const std::string& s) {

	size_t strt = 0;
	int64_t v = 0;
	uint32_t mul = 1;
	bool negative = false;

	if (s[0] == '-') {
		negative = true;
		strt = 1;
	}

	for (strt; strt < s.length(); ++strt) {
		v *= mul;
		v += s[strt] - '0';
		mul *= 10;
	}

	return v;
}

float str_tof32(const std::string& s) {

	float v = 0.0;
	double mul = 0.0;
	double pn = 1.0;

	for (auto& c : s) {

		if (c == '.') {
			pn = -1.0;
			continue;
		}

		v += (double)((c - '0') * pow(10, pn * mul));
		mul += 1.0;
	}

	return v;
}