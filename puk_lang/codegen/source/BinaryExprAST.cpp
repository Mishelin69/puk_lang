#include "../header/ASTNodes.hpp"

llvm::Value* Pukser::BinaryExprAST::handle_add(
        llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) {

    if (this->LHS->type != this->RHS->type) {
        //this is where "crack" will come in handy, feature not implemented yet, for now return nullptr
        return nullptr;
    }

    llvm::Value* L = this->LHS->gen_code(ctx, builder);
    llvm::Value* R = this->LHS->gen_code(ctx, builder);

    if (!R || !L) {
        return nullptr;
    }

    switch (this->LHS->type) {

        case Puxer::PuxerI32:
            return builder->
            break;
    }
}

llvm::Value* Pukser::BinaryExprAST::gen_code(
        llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) {

    switch (this->op) {

        case '+':
            return handle_add(ctx, builder);
            break;

        default:
            return log_error("invalid binary operator!!");
            break;
    }
}
