#pragma once

#include "PukserASTNodesDef.h"
#include "PuxerTokenResponse.h"

namespace Puxer {
	class Puxer;
}

namespace Pukser {

	class Pukser {

	public:
		void parse(const char* fn);
		std::unique_ptr<ExprAST> log_error(const char* s);

		std::unique_ptr<ExprAST> handle_number(Puxer::PuxerTokenResponse& res);
		std::unique_ptr<ExprAST> parse_expression(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<ExprAST> parse_paren(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<ExprAST> parse_identifier(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
	};

}