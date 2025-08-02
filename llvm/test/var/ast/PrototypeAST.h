#ifndef __PROTOTYPE_AST_H__
#define __PROTOTYPE_AST_H__

#include "ast/ExprAST.h"
#include "llvm/IR/IRBuilder.h"
#include "kaleidoscope/kaleidoscope.h"
#include <map>

// Represents the "prototype" for a function,
// which captures its name, and its argument names
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &name, std::vector<std::string> Args) : Name(name), Args(std::move(Args)) {}

  bool isUnaryOp() const { return IsOperator && Args.size() == 1; }
  bool isBinaryOp() const { return IsOperator && Args.size() == 2; }

  char getOperatorName() const {
    assert(isUnaryOp() || isBinaryOp());
    return Name[Name.size() - 1];
  }

  unsigned getBinaryPrecedence() const { return Precedence; }
  
  llvm::Function *codegen();
  const std::string &getName() const { return Name; }
};

extern std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

#endif
