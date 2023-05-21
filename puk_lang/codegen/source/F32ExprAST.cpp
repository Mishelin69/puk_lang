#include "../header/ASTNodes.hpp"
#include <llvm/ADT/APFloat.h>

llvm::Value* Pukser::F64ExprAST::gen_code(llvm::LLVMContext* ctx, llvm::IRBuilder<>*) {
    return llvm::ConstantFP::get(*ctx, llvm::APFloat(this->val));
}
