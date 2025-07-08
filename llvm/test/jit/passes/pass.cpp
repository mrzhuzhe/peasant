#include "pass.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar.h"

//===----------------------------------------------------------------------===//
// Top-Level parsing and JIT Driver
//===----------------------------------------------------------------------===//

void InitializeModuleAndPassManagers() {
  
  TheJIT = ExitOnErr(KaleidoscopeJIT::Create());

  // Open a new context and module.
  TheContext = std::make_unique<llvm::LLVMContext>();
  // TheModule = std::make_unique<llvm::Module>("My awesome JIT", *TheContext);
  TheModule = std::make_unique<llvm::Module>("KaleidoscopeJIT", *TheContext);
  TheModule->setDataLayout(TheJIT->getDataLayout());

  // Create a new builder for the module.
  Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);

  // Create new pass and analysis managers.
  TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
  // TheLAM = std::make_unique<LoopAnalysisManager>();
  // TheFAM = std::make_unique<FunctionAnalysisManager>();
  // TheCGAM = std::make_unique<CGSCCAnalysisManager>();
  // TheMAM = std::make_unique<ModuleAnalysisManager>();
  // ThePIC = std::make_unique<PassInstrumentationCallbacks>();
  // TheSI = std::make_unique<StandardInstrumentations>(*TheContext,
  //                                                    /*DebugLogging*/ true);
  // TheSI->registerCallbacks(*ThePIC, TheMAM.get());

  // Add transform passes.
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  TheFPM->add(llvm::createInstructionCombiningPass());
  // Reassociate expressions.
  TheFPM->add(llvm::createReassociatePass());
  // Eliminate Common SubExpressions.
  TheFPM->add(llvm::createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  TheFPM->add(llvm::createCFGSimplificationPass());

  // Register analysis passes used in these transform passes.
  // PassBuilder PB;
  // PB.registerModuleAnalyses(*TheMAM);
  // PB.registerFunctionAnalyses(*TheFAM);
  // PB.crossRegisterProxies(*TheLAM, *TheFAM, *TheCGAM, *TheMAM);
  TheFPM->doInitialization();
}