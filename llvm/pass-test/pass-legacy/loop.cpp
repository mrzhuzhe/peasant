// opt -load-pass-plugin=build/libmodules.so -passes="goodbye" outputs/aa.ll -S
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

namespace {

bool runBye(Loop *L) {
  
  errs() << "Bye: ";
  errs().write_escaped(L->getName()) << '\n';
  return false;
}

struct LegacyBye : public LoopPass {
  static char ID;
  LegacyBye() : LoopPass(ID) {}
  bool runOnLoop(Loop *L, LPPassManager &LPM) override { return runBye(L); }
};

// struct Bye : PassInfoMixin<Bye> {
//   PreservedAnalyses run(Loop *L, loopAnalysisManager &) {
//     if (!runBye(M))
//       return PreservedAnalyses::all();
//     return PreservedAnalyses::none();
//   }
// };

} // namespace

// char LegacyBye::ID = 0;

// static RegisterPass<LegacyBye> X("goodbye", "Good Bye World Pass",
//                                  false /* Only looks at CFG */,
//                                  false /* Analysis Pass */);

// /* New PM Registration */
// llvm::PassPluginLibraryInfo getByePluginInfo() {
//   return {LLVM_PLUGIN_API_VERSION, "Bye", LLVM_VERSION_STRING,
//           [](PassBuilder &PB) {
//             PB.registerPipelineParsingCallback(
//                 [](StringRef Name, llvm::LoopPassManager &PM,
//                    ArrayRef<llvm::PassBuilder::PipelineElement>) {
//                   if (Name == "goodbye") {
//                     PM.addPass(Bye());
//                     return true;
//                   }
//                   return false;
//                 });
//           }};
// }

// extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
// llvmGetPassPluginInfo() {
//   return getByePluginInfo();
// }
