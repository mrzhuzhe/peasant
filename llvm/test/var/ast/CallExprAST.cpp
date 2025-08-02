#include "ast/CallExprAST.h"


llvm::Function *getFunction(std::string Name) {
  // First, see if the function has already been added to the current module.
  if (auto *F = TheModule->getFunction(Name))
    return F;

  // If not, check whether we can codegen the declaration from some existing
  // prototype.
  auto FI = FunctionProtos.find(Name);
  if (FI != FunctionProtos.end())
    return FI->second->codegen();

  // If no existing prototype exists, return null.
  return nullptr;
}

// Generate LLVM code for function calls
llvm::Value *CallExprAST::codegen() {
  // llvm::Function *CalleeF = TheModule->getFunction(Callee);
  llvm::Function *CalleeF = getFunction(Callee);
  if (!CalleeF) {
    return LogErrorV("Unknown function referenced");
  }

  if (CalleeF->arg_size() != Args.size()) {
    return LogErrorV("Incorrect # arguments passed");
  }

  std::vector<llvm::Value *> ArgsV;
  for (unsigned i = 0, e = Args.size(); i != e; i++) {
    ArgsV.push_back(Args[i]->codegen());

    if (!ArgsV.back()) {
      return nullptr;
    }
  }

  return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}
