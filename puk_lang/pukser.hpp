#pragma once

#include "PukserASTNodesDef.h"
#include "PuxerTokenResponse.h"

namespace Pukser {

	class Pukser {

	public:
		void parse(const char* fn);
		void log_error(const char* s);

		std::unique_ptr<ExprAST> handle_number(Puxer::PuxerTokenResponse& res);
	};

}