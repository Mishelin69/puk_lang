#pragma once

#include <stdint.h>

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