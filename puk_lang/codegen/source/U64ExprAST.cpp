#include "../header/ASTNodes.hpp"
#include <llvm/ADT/APInt.h>

llvm::Value* Pukser::U64ExprAST::gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>*) {
    return llvm::ConstantInt::get(*ctx, llvm::APInt(sizeof(uint64_t), this->val, false));
}
