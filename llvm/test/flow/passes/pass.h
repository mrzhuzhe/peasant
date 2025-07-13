#pragma once

#include "kaleidoscope/kaleidoscope.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"

void InitializeModuleAndPassManagers();