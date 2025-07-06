#ifndef __KALEIDOSCOPE_H__
#define __KALEIDOSCOPE_H__

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

#include "ast/PrototypeAST.h"
#include "llvm/IR/LegacyPassManager.h"
#include <map>

// This is an object that owns LLVM core data structures
extern std::unique_ptr<llvm::LLVMContext> TheContext;

// This is a helper object that makes easy to generate LLVM instructions
extern std::unique_ptr<llvm::IRBuilder<>> Builder;

// This is an LLVM construct that contains functions and global variables
extern std::unique_ptr<llvm::Module> TheModule;

// This map keeps track of which values are defined in the current scope
extern std::map<std::string, llvm::Value *> NamedValues;

// extern std::unique_ptr<KaleidoscopeJIT> TheJIT;
extern std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;
// extern std::unique_ptr<LoopAnalysisManager> TheLAM;
// extern std::unique_ptr<FunctionAnalysisManager> TheFAM;
// extern std::unique_ptr<CGSCCAnalysisManager> TheCGAM;
// extern std::unique_ptr<ModuleAnalysisManager> TheMAM;
// extern std::unique_ptr<PassInstrumentationCallbacks> ThePIC;
// extern std::unique_ptr<StandardInstrumentations> TheSI;
extern llvm::ExitOnError ExitOnErr;

#endif
