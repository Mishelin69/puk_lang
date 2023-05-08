#pragma once

#include "puxer_helper_defs.h"

#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

namespace Pukser {

	class ExprAST {

	public:
		Puxer::PuxerType type;

	public:
		virtual ~ExprAST() = default;

		friend class Pukser;
	};

	class SingleCharExpr : public ExprAST {

	public: 
		char c;

	public:
		SingleCharExpr(char c):
			c(c) {}

		friend class Pukser;
	};

    class UknownExprAST: public ExprAST {

    public:
        std::string val;

    public:
        UknownExprAST(std::string val) :
            val(val) { }

		friend class Pukser;
    };

	class CustomTypeExprAST: public ExprAST {

	public:
		Puxer::PuxerCustomType var_info;
		std::vector<Puxer::PuxerCustomType> members_list;

	public:
		CustomTypeExprAST(Puxer::PuxerCustomType var_info,
			std::vector<Puxer::PuxerCustomType> members_list) 
			: var_info(var_info), members_list(std::move(members_list)) {}

		friend class Pukser;
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

		friend class Pukser;
	};

	class I64ExprAST : public ExprAST {

	private:
		int64_t val;

	public:
		I64ExprAST(int64_t val) :
			val(val) {}

		friend class Pukser;
	};

	class F32ExprAST : public ExprAST {

	private:
		float val;

	public:
		F32ExprAST(float val) :
			val(val) {}

		friend class Pukser;
	};

	class F64ExprAST : public ExprAST {

	private:
		double val;

	public:
		F64ExprAST(double val) :
			val(val) {}

		friend class Pukser;
	};

	class U32ExprAST : public ExprAST {

	private:
		uint32_t val;

	public:
		U32ExprAST(uint32_t val) :
			val(val) {}

		friend class Pukser;
	};

	class U64ExprAST : public ExprAST {

	private:
		uint64_t val;

	public:
		U64ExprAST(uint64_t val) :
			val(val) {}

		friend class Pukser;
	};

	//Identifier Node
	class VariableExprAST : public ExprAST {

	private:
		std::string name;
		Puxer::PuxerType type;
		std::shared_ptr<Puxer::PuxerCustomType> type_info;

	public:
		VariableExprAST(std::string name, 
			Puxer::PuxerType type, std::shared_ptr<Puxer::PuxerCustomType> type_info)
			: name(name), type(type), type_info(type_info) {}

		VariableExprAST() = default;
		VariableExprAST(const VariableExprAST& other) = default;
		~VariableExprAST() = default;

		friend class Pukser;
	};

	//Binary Operation

	class BinaryExprAST : public ExprAST {

	private:
		char op;
		std::unique_ptr<ExprAST> LHS, RHS;

	public:
		BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
			std::unique_ptr<ExprAST> RHS)
			: op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

		friend class Pukser;
	};

	class CallExprAST : public ExprAST {

	private:
		std::string callee;
		std::vector<std::unique_ptr<ExprAST>> args;

	public:
		CallExprAST(const std::string callee,
			std::vector<std::unique_ptr<ExprAST>> args)
			: callee(callee), args(std::move(args)) {}

		friend class Pukser;
	};

	class PrototypeAST {

		std::shared_ptr<Puxer::PuxerCustomType> type;
		std::string name;
		std::vector<std::string> args;

	public:
		PrototypeAST(const std::string& name, 
			std::vector<std::string> args, std::shared_ptr<Puxer::PuxerCustomType> type)
			: name(name), args(std::move(args)), type(type) {}

		const std::string& getName() const { return name; }

		friend class Pukser;
	};

	class FunctionAST {
		std::unique_ptr<PrototypeAST> proto;
		std::vector<std::unique_ptr<ExprAST>> body;

	public:
		FunctionAST(std::unique_ptr<PrototypeAST> proto,
			std::vector<std::unique_ptr<ExprAST>> body)
			: proto(std::move(proto)), body(std::move(body)) {}

		friend class Pukser;
	};

}
