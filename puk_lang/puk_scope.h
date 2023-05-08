#pragma once
#include "Puxer.h"
#include <unordered_map>

namespace Puxer {

	class Puxer;

	class PukScope {

	private:

		Puxer* puxer_ref;
		std::unordered_map<std::string, PuxerIdentifier> scope_vars;

		FILE* fs;

	public:

		int get_token();

		PukScope() = default;
		PukScope(PukScope &other) = default;
		~PukScope() = default;

	};
}
