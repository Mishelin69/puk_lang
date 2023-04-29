#pragma once

#include "puxer_helper_defs.h"
#include "PuxerTokenResponse.h"
#
#include <iostream>
#include <stack>
#include <string>
#include <queue>

namespace Puxer {

	class PukScope;

	class Puxer {

	private:

		FILE* fs;
		std::queue<int> identifier_queue;

	public:

		Puxer();
		Puxer(Puxer& other) = delete;
		~Puxer();

		int open_file(const char* filename);
		PuxerTokenResponse get_token();

		bool PuxerHandleNumber(std::string& s, PuxerIdentifier& i);
		PuxerToken PuxerHandleIndentifier(std::string& s, PuxerIdentifier &i);

	};

}

