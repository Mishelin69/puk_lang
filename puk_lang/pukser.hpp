#pragma once

#include "PukserASTNodesDef.h"
#include "PuxerTokenResponse.h"
#include "PukserScope.h"

#include <stack>

namespace Puxer {
	class Puxer;
}

namespace Pukser {

	static std::map<char, int> BinopPrecedence = { {'=', 2},  {'<', 10}, {'+', 20}, {'-', 20}, {'*', 40} };
	static std::map<std::string, std::shared_ptr<Puxer::PuxerCustomType>> types_map = {
			{ std::string("i32"), std::make_shared<Puxer::PuxerCustomType>(Puxer::PuxerCustomType(std::string("i32"), Puxer::PuxerI32, 4)) },
			{ std::string("i64"), std::make_shared<Puxer::PuxerCustomType>(Puxer::PuxerCustomType("i64", Puxer::PuxerI64, 8)) },
			{ std::string("u8"), std::make_shared<Puxer::PuxerCustomType>(Puxer::PuxerCustomType("u8", Puxer::PuxerF64, 1)) },
			{ std::string("u32"), std::make_shared<Puxer::PuxerCustomType>(Puxer::PuxerCustomType("u32", Puxer::PuxerU32, 4)) },
			{ std::string("u64"), std::make_shared<Puxer::PuxerCustomType>(Puxer::PuxerCustomType("u64", Puxer::PuxerU64, 8)) },
			{ std::string("f32"), std::make_shared<Puxer::PuxerCustomType>(Puxer::PuxerCustomType("f32", Puxer::PuxerF32, 4)) },
			{ std::string("f64"), std::make_shared<Puxer::PuxerCustomType>(Puxer::PuxerCustomType("f64", Puxer::PuxerF64, 8)) },
			{ std::string("None"), std::make_shared<Puxer::PuxerCustomType>(Puxer::PuxerCustomType("None", Puxer::PuxerNone, 0)) },
		};

	class Pukser {

	private:

		std::map<std::string, std::shared_ptr<Puxer::PuxerCustomType>> builtin_types;
		std::map<std::string, VariableExprAST> globals;
		std::stack<PukserScope> scope;

        size_t stack_size = 0;
        std::unique_ptr<ExprAST> last_expr;

	public:

		Pukser() = default;
		Pukser(Pukser& other) = delete;
		~Pukser() = default;

		void parse(const char* fn);
		std::unique_ptr<ExprAST> log_error(const char* s);

		void create_scope();
		void drop_cur_scope();
		std::unique_ptr<ExprAST> declare_var(VariableExprAST var);
		std::unique_ptr<ExprAST> fetch_var(std::string& name);

		std::unique_ptr<ExprAST> handle_number(Puxer::PuxerTokenResponse& res);
		std::unique_ptr<ExprAST> handle_SCE(char c);
		std::shared_ptr<Puxer::PuxerCustomType> handle_type(Puxer::PuxerTokenResponse& res);
        int handle_token_precedence(Puxer::PuxerTokenResponse& res);
		void handle_toplevel(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		void handle_functiondef(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		void handle_valdef(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);

		void parse_curly(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res, std::vector<std::unique_ptr<ExprAST>>& body);
		std::unique_ptr<ExprAST> parse_expression(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<ExprAST> parse_paren(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<ExprAST> parse_identifier(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<ExprAST> parse_primary(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
        std::unique_ptr<ExprAST> parse_binary_ops_rhs(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res, int prec, std::unique_ptr<ExprAST> LHS);
		std::unique_ptr<ExprAST> parse_valdef(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<PrototypeAST> parse_prototype(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<FunctionAST> parse_definition(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
		std::unique_ptr<FunctionAST> parse_toplevel(Puxer::Puxer& pux, Puxer::PuxerTokenResponse& res);
	};

}
