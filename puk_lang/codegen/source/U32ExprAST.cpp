#include "../header/ASTNodes.hpp"
#include <llvm/ADT/APInt.h>

llvm::Value* Pukser::U32ExprAST::gen_code(llvm::LLVMContext* ctx) {
    return llvm::ConstantInt::get(*ctx, llvm::APInt(sizeof(uint32_t), this->val, false));
}
