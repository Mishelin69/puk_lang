#pragma once
#include "PukserASTNodesDef.h";
#include <map>

namespace Pukser {

	struct PukserScope {
		size_t stack_size;
		std::map<std::string, const VariableExprAST> vars;

		PukserScope() = default;
		PukserScope(PukserScope& other) = delete;
		~PukserScope() = default;
	};

}