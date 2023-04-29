#pragma once

#include "puxer_helper_defs.h"

#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

namespace Pukser {

	class ExprAST {

	private:
		Puxer::PuxerType type;

	public:
		virtual ~ExprAST() = default;
	};

	/*
	* Number Nodes
	*/
	class I32ExprAST : public ExprAST {

	private:
		int32_t val;

	public:
		I32ExprAST(int32_t val) :
			val(val) {}
	};

	class I64ExprAST : public ExprAST {

	private:
		int64_t val;

	public:
		I64ExprAST(int64_t val) :
			val(val) {}
	};

	class F32ExprAST : public ExprAST {

	private:
		float val;

	public:
		F32ExprAST(float val) :
			val(val) {}
	};

	class F64ExprAST : public ExprAST {

	private:
		double val;

	public:
		F64ExprAST(double val) :
			val(val) {}
	};

	class U32ExprAST : public ExprAST {

	private:
		uint32_t val;

	public:
		U32ExprAST(uint32_t val) :
			val(val) {}
	};

	class U64ExprAST : public ExprAST {

	private:
		uint64_t val;

	public:
		U64ExprAST(uint64_t val) :
			val(val) {}
	};

	//Identifier Node
	class VariableExprAST : public ExprAST {

	private:
		std::string name;

	public:
		VariableExprAST(std::string name) :
			name(name) {}
	};

	//Binary Operation

	class BinaryExprAST : public ExprAST {

	private:
		char op;
		std::unique_ptr<ExprAST> &LHS, &RHS;

	public:
		BinaryExprAST(char op, std::unique_ptr<ExprAST> &LHS,
			std::unique_ptr<ExprAST> &RHS)
			: op(op), LHS(LHS), RHS(RHS) {}
	};

	class CallExprAST : public ExprAST {

	private:
		std::string callee;
		std::vector<std::unique_ptr<ExprAST>> args;

	public:
		CallExprAST(const std::string& callee,
			std::vector<std::unique_ptr<ExprAST>> args)
			: callee(callee), args(std::move(args)) {}
	};

	class PrototypeAST {
		std::string name;
		std::vector<std::string> args;

	public:
		PrototypeAST(const std::string& name, std::vector<std::string> args)
			: name(name), args(std::move(args)) {}

		const std::string& getName() const { return name; }
	};

	class FunctionAST {
		std::unique_ptr<PrototypeAST> proto;
		std::unique_ptr<ExprAST> body;

	public:
		FunctionAST(std::unique_ptr<PrototypeAST> proto,
			std::unique_ptr<ExprAST> body)
			: proto(std::move(proto)), body(std::move(body)) {}
	};

}