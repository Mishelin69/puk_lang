#pragma once

#include "PukserASTNodesDef.h"
#include "PuxerTokenResponse.h"
#include <map>

namespace Puxer {
	class Puxer;
}

namespace Pukser {

    static std::map<char, int> BinopPrecedence = {{'<', 10}, {'+', 20}, {'-', 20}, {'*', 40}};

	class Pukser {

	public:
		void parse(const char* fn);
		std::unique_ptr<ExprAST> log_error(const char* s);

		std::unique_ptr<ExprAST> handle_number(Puxer::PuxerTokenResponse& res);
        int handle_token_precedence(Puxer::PuxerTokenResponse& res);
		void handle_toplevel(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		void handle_functiondef(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);

		std::unique_ptr<ExprAST> parse_expression(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<ExprAST> parse_paren(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		void parse_curly(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res, std::vector<std::unique_ptr<ExprAST>>& body);
		std::unique_ptr<ExprAST> parse_identifier(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<ExprAST> parse_primary(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
        std::unique_ptr<ExprAST> parse_binary_ops_rhs(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res, int prec, std::unique_ptr<ExprAST> LHS);
		std::unique_ptr<PrototypeAST> parse_prototype(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<FunctionAST> parse_definition(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<FunctionAST> parse_toplevel(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
	};

}
