#include "../header/ASTNodes.hpp"
#include <llvm/ADT/APInt.h>

llvm::Value* Pukser::I64ExprAST::gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>*) {
    return llvm::ConstantInt::get(*ctx, llvm::APInt(sizeof(int64_t), this->val, true));
}
