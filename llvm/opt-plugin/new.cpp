//  opt -load-pass-plugin=build/libnew.so -passes="Helloworld2" outputs/aa.ll  -S

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

namespace llvm {

    class HelloWorldPass2 : public PassInfoMixin<HelloWorldPass2> {
    public:
        PreservedAnalyses run(Function &F,
                                        FunctionAnalysisManager &AM) {
            errs() << "function name is \n" ;
            errs() << F.getName() << "\n";
            return PreservedAnalyses::all();
        }
    };    
} // namespace llvm

/* New PM Registration */
llvm::PassPluginLibraryInfo getHelloworldPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "Helloworld2", LLVM_VERSION_STRING,
          [](llvm::PassBuilder &PB) {
            PB.registerVectorizerStartEPCallback(
                [](llvm::FunctionPassManager &PM, llvm::OptimizationLevel Level) {
                  PM.addPass(llvm::HelloWorldPass2());
                });
            PB.registerPipelineParsingCallback(
                [](llvm::StringRef Name, llvm::FunctionPassManager &PM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                  if (Name == "Helloworld2") {
                    PM.addPass(llvm::HelloWorldPass2());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getHelloworldPluginInfo();
}