#pragma once
#include "../../codegen/header/ASTNodes.hpp"
#include <map>

namespace Pukser {

	struct PukserScope {
		size_t stack_size = 0;
		std::map<std::string, const VariableExprAST> vars;

		PukserScope() = default;
		PukserScope(const PukserScope& other) = default;
		~PukserScope() = default;
	};

}
