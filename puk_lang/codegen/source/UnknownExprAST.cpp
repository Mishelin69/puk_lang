#include "../header/ASTNodes.hpp"

llvm::Value* Pukser::UknownExprAST::gen_code() {
    fprintf(stderr, "Error: couln't infer type ... exitng");
    exit(0);
    return nullptr;
}
