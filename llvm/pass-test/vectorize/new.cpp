//  opt -load-pass-plugin=build/libnew.so -passes="Helloworld2" outputs/aa.ll  -S
//  https://github.com/10x-Engineers/tutorial-llvm-pass/blob/main/HelloWorld/HelloWorld.cpp

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "inc/LoopVectorize.h"

namespace llvm {

} // namespace llvm

/* New PM Registration */
llvm::PassPluginLibraryInfo getHelloworldPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "zzVectorize", LLVM_VERSION_STRING,
          [](llvm::PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](llvm::StringRef Name, llvm::FunctionPassManager &PM,
                   llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
                  if (Name == "zzVectorize") {
                    PM.addPass(llvm::LoopVectorizePass());
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