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
            return builder->CreateAdd(L, R, "i32_add");
            break;

        case Puxer::PuxerI64:
            return builder->CreateAdd(L, R, "i64_add");
            break;

        case Puxer::PuxerU32:
            return builder->CreateAdd(L, R, "u32_add");
            break;

        case Puxer::PuxerU64:
            return builder->CreateAdd(L, R, "u64_add");
            break;

        case Puxer::PuxerF32:
            return builder->CreateFAdd(L, R, "f32_add");
            break;

        case Puxer::PuxerF64:
            return builder->CreateFAdd(L, R, "f64_add");
            break;

        default:
            return log_error("Invalid puk type");
            break;
    }
}

llvm::Value* Pukser::BinaryExprAST::handle_sub(
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
            return builder->CreateSub(L, R, "i32_sub");
            break;

        case Puxer::PuxerI64:
            return builder->CreateSub(L, R, "i64_sub");
            break;

        case Puxer::PuxerU32:
            return builder->CreateSub(L, R, "u32_sub");
            break;

        case Puxer::PuxerU64:
            return builder->CreateSub(L, R, "u64_sub");
            break;

        case Puxer::PuxerF32:
            return builder->CreateFSub(L, R, "f32_sub");
            break;

        case Puxer::PuxerF64:
            return builder->CreateFSub(L, R, "f64_sub");
            break;

        default:
            return log_error("Invalid puk type");
            break;
    }
}

llvm::Value* Pukser::BinaryExprAST::handle_mul(
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
            return builder->CreateMul(L, R, "i32_mul");
            break;

        case Puxer::PuxerI64:
            return builder->CreateMul(L, R, "i64_mul");
            break;

        case Puxer::PuxerU32:
            return builder->CreateMul(L, R, "u32_mul");
            break;

        case Puxer::PuxerU64:
            return builder->CreateMul(L, R, "u64_mul");
            break;

        case Puxer::PuxerF32:
            return builder->CreateFMul(L, R, "f32_mul");
            break;

        case Puxer::PuxerF64:
            return builder->CreateFMul(L, R, "f64_mul");
            break;

        default:
            return log_error("Invalid puk type");
            break;
    }
}

llvm::Value* Pukser::BinaryExprAST::handle_div(
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
            return builder->CreateSDiv(L, R, "i32_div");
            break;

        case Puxer::PuxerI64:
            return builder->CreateSDiv(L, R, "i64_div");
            break;

        case Puxer::PuxerU32:
            return builder->CreateUDiv(L, R, "u32_div");
            break;

        case Puxer::PuxerU64:
            return builder->CreateUDiv(L, R, "u64_div");
            break;

        case Puxer::PuxerF32:
            return builder->CreateFDiv(L, R, "f32_div");
            break;

        case Puxer::PuxerF64:
            return builder->CreateFDiv(L, R, "f64_div");
            break;

        default:
            return log_error("Invalid puk type");
            break;
    }
}

llvm::Value* Pukser::BinaryExprAST::gen_code(
        llvm::LLVMContext* ctx, llvm::IRBuilder<>* builder) {

    switch (this->op) {

        case '+':
            return handle_add(ctx, builder);
            break;

        case '-':
            return handle_sub(ctx, builder);
            break;

        case '*':
            return handle_mul(ctx, builder);
            break;

        case '/':
            return handle_div(ctx, builder);
            break;

        default:
            return log_error("invalid binary operator!!");
            break;
    }
}
