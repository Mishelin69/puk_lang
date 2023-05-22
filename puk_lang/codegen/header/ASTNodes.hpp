#pragma once

#include "../../helper/header/PuxerHelperDef.hpp"

#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

static inline llvm::Value* log_error(const char* c) {
    fprintf(stderr, "Error: %s", c);
    return nullptr;
}

namespace Pukser {

	enum ExprType {

		Undefined = 0,
		SingleChar,
		UnknownType,
		CustomType,
		I32,
		I64,
		U32,
		U64,
		F32,
		F64,
		Char,
		Variable,
		ExprBody,
		BinaryExpr,
		CallExpr,
	};

	//base class
	class ExprAST {

	public:
		Puxer::PuxerType type;
		ExprType expr_type;

	public:
		virtual ~ExprAST() = default;
        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) = 0;

		ExprAST(Puxer::PuxerType type, ExprType expr_type)
			: type(type), expr_type(expr_type) {}
		ExprAST()
			: type(Puxer::PuxerUnknown), expr_type(Undefined) {}

		friend class Pukser;
	};

	class SingleCharExpr : public ExprAST {

	public: 
		char c;

	public:
		SingleCharExpr(char c)
			: c(c), ExprAST(Puxer::PuxerChar, Char) {}

		SingleCharExpr()
			: ExprAST(Puxer::PuxerChar, Char) {}

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

		friend class Pukser;
	};

    class UknownExprAST: public ExprAST {

    public:
        std::string val;

    public:
        UknownExprAST(std::string val) :
            val(val), ExprAST(Puxer::PuxerUnknown, UnknownType) { }

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

		friend class Pukser;
    };

	class CustomTypeExprAST: public ExprAST {

	public:
		Puxer::PuxerCustomType var_info;
		std::vector<Puxer::PuxerCustomType> members_list;

	public:
		CustomTypeExprAST(Puxer::PuxerCustomType var_info,
			std::vector<Puxer::PuxerCustomType> members_list) 
			: var_info(var_info), members_list(std::move(members_list)), 
			  ExprAST(Puxer::PuxerCustom, CustomType) {}

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

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
			val(val), ExprAST(Puxer::PuxerI32, I32) {}

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

		friend class Pukser;
	};

	class I64ExprAST : public ExprAST {

	private:
		int64_t val;

	public:
		I64ExprAST(int64_t val) :
			val(val), ExprAST(Puxer::PuxerI64, I64) {}

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

		friend class Pukser;
	};

	class F32ExprAST : public ExprAST {

	private:
		float val;

	public:
		F32ExprAST(float val) :
			val(val), ExprAST(Puxer::PuxerF32, F32) {}

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

		friend class Pukser;
	};

	class F64ExprAST : public ExprAST {

	private:
		double val;

	public:
		F64ExprAST(double val) :
			val(val), ExprAST(Puxer::PuxerF64, F64) {}

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

		friend class Pukser;
	};

	class U32ExprAST : public ExprAST {

	private:
		uint32_t val;

	public:
		U32ExprAST(uint32_t val) :
			val(val), ExprAST(Puxer::PuxerU32, U32) {}

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

		friend class Pukser;
	};

	class U64ExprAST : public ExprAST {

	private:
		uint64_t val;

	public:
		U64ExprAST(uint64_t val) :
			val(val), ExprAST(Puxer::PuxerU64, U64) {}

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

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
			: name(name), type(type), type_info(type_info), ExprAST(Puxer::PuxerVariable, Variable) {}

		VariableExprAST()
			: ExprAST(Puxer::PuxerVariable, Variable) {}
		VariableExprAST(const VariableExprAST& other) = default;
		~VariableExprAST() = default;

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

		friend class Pukser;
	};

    class ExpressionBodyAST : public ExprAST {

    private:
        std::vector<std::unique_ptr<ExprAST>> body;

    public:
        ExpressionBodyAST()
			: ExprAST(Puxer::PuxerNone, ExprBody) {};
		ExpressionBodyAST(const ExpressionBodyAST& other);
        ~ExpressionBodyAST() = default;

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

		friend class Pukser;
    };

	//Binary Operation

	class BinaryExprAST : public ExprAST {

	private:
		char op;
		std::unique_ptr<ExprAST> LHS, RHS;

    private:
        llvm::Value* handle_add(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder);
        llvm::Value* handle_sub(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder);
        llvm::Value* handle_mul(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder);
        llvm::Value* handle_div(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder);

	public:
		BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
			std::unique_ptr<ExprAST> RHS)
			: op(op), LHS(std::move(LHS)), RHS(std::move(RHS)), 
			  ExprAST(Puxer::PuxerNone, BinaryExpr) {}

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

		friend class Pukser;
	};

	class CallExprAST : public ExprAST {

	private:
		std::string callee;
		std::vector<std::unique_ptr<ExprAST>> args;

	public:
		CallExprAST(const std::string callee,
			std::vector<std::unique_ptr<ExprAST>> args)
			: callee(callee), args(std::move(args)), 
			  ExprAST(Puxer::PuxerFunctionCall, CallExpr) {}

        virtual llvm::Value* gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) override;

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
		std::unique_ptr<ExprAST> body;

	public:
		FunctionAST(std::unique_ptr<PrototypeAST> proto,
			std::unique_ptr<ExprAST> body)
			: proto(std::move(proto)), body(std::move(body)) {}

		friend class Pukser;
	};

}
