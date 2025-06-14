SOURCES = $(shell find ast kaleidoscope lexer logger parser -name '*.cpp')
HEADERS = $(shell find ast kaleidoscope lexer logger parser -name '*.h')
OBJ = ${SOURCES:.cpp=.o}

#CC = llvm-g++ -stdlib=libc++ -std=c++14
CC = clang++ -stdlib=libstdc++ -std=c++14
#CFLAGS = -g -O3 -I llvm/include -I llvm/build/include -I ./ 
CFLAGS = -g -I llvm/include -I llvm/build/include -I ./ 
LLVMFLAGS = `llvm-config --cxxflags --ldflags --system-libs --libs all`

LINKDIR= -L/mnt/e28833eb-0c99-4fe2-802a-09fa58d9c9f5/code/llvm-project/build/lib
LDLIBS = -lLLVMWindowsManifest \
-lLLVMXRay \
-lLLVMLibDriver \
-lLLVMDlltoolDriver \
-lLLVMCoverage \
-lLLVMLineEditor \
-lLLVMXCoreDisassembler \
-lLLVMXCoreCodeGen \
-lLLVMXCoreDesc \
-lLLVMXCoreInfo \
-lLLVMX86TargetMCA \
-lLLVMX86Disassembler \
-lLLVMX86AsmParser \
-lLLVMX86CodeGen \
-lLLVMX86Desc \
-lLLVMX86Info \
-lLLVMWebAssemblyDisassembler \
-lLLVMWebAssemblyAsmParser \
-lLLVMWebAssemblyCodeGen \
-lLLVMWebAssemblyDesc \
-lLLVMWebAssemblyUtils \
-lLLVMWebAssemblyInfo \
-lLLVMVEDisassembler \
-lLLVMVEAsmParser \
-lLLVMVECodeGen \
-lLLVMVEDesc \
-lLLVMVEInfo \
-lLLVMSystemZDisassembler \
-lLLVMSystemZAsmParser \
-lLLVMSystemZCodeGen \
-lLLVMSystemZDesc \
-lLLVMSystemZInfo \
-lLLVMSparcDisassembler \
-lLLVMSparcAsmParser \
-lLLVMSparcCodeGen \
-lLLVMSparcDesc \
-lLLVMSparcInfo \
-lLLVMRISCVTargetMCA \
-lLLVMRISCVDisassembler \
-lLLVMRISCVAsmParser \
-lLLVMRISCVCodeGen \
-lLLVMRISCVDesc \
-lLLVMRISCVInfo \
-lLLVMPowerPCDisassembler \
-lLLVMPowerPCAsmParser \
-lLLVMPowerPCCodeGen \
-lLLVMPowerPCDesc \
-lLLVMPowerPCInfo \
-lLLVMNVPTXCodeGen \
-lLLVMNVPTXDesc \
-lLLVMNVPTXInfo \
-lLLVMMSP430Disassembler \
-lLLVMMSP430AsmParser \
-lLLVMMSP430CodeGen \
-lLLVMMSP430Desc \
-lLLVMMSP430Info \
-lLLVMMipsDisassembler \
-lLLVMMipsAsmParser \
-lLLVMMipsCodeGen \
-lLLVMMipsDesc \
-lLLVMMipsInfo \
-lLLVMLoongArchDisassembler \
-lLLVMLoongArchAsmParser \
-lLLVMLoongArchCodeGen \
-lLLVMLoongArchDesc \
-lLLVMLoongArchInfo \
-lLLVMLanaiDisassembler \
-lLLVMLanaiCodeGen \
-lLLVMLanaiAsmParser \
-lLLVMLanaiDesc \
-lLLVMLanaiInfo \
-lLLVMHexagonDisassembler \
-lLLVMHexagonCodeGen \
-lLLVMHexagonAsmParser \
-lLLVMHexagonDesc \
-lLLVMHexagonInfo \
-lLLVMBPFDisassembler \
-lLLVMBPFAsmParser \
-lLLVMBPFCodeGen \
-lLLVMBPFDesc \
-lLLVMBPFInfo \
-lLLVMAVRDisassembler \
-lLLVMAVRAsmParser \
-lLLVMAVRCodeGen \
-lLLVMAVRDesc \
-lLLVMAVRInfo \
-lLLVMARMDisassembler \
-lLLVMARMAsmParser \
-lLLVMARMCodeGen \
-lLLVMARMDesc \
-lLLVMARMUtils \
-lLLVMARMInfo \
-lLLVMAMDGPUTargetMCA \
-lLLVMAMDGPUDisassembler \
-lLLVMAMDGPUAsmParser \
-lLLVMAMDGPUCodeGen \
-lLLVMAMDGPUDesc \
-lLLVMAMDGPUUtils \
-lLLVMAMDGPUInfo \
-lLLVMAArch64Disassembler \
-lLLVMAArch64AsmParser \
-lLLVMAArch64CodeGen \
-lLLVMAArch64Desc \
-lLLVMAArch64Utils \
-lLLVMAArch64Info \
-lLLVMOrcJIT \
-lLLVMWindowsDriver \
-lLLVMMCJIT \
-lLLVMJITLink \
-lLLVMInterpreter \
-lLLVMExecutionEngine \
-lLLVMRuntimeDyld \
-lLLVMOrcTargetProcess \
-lLLVMOrcShared \
-lLLVMDWP \
-lLLVMDebugInfoLogicalView \
-lLLVMDebugInfoGSYM \
-lLLVMOption \
-lLLVMObjectYAML \
-lLLVMObjCopy \
-lLLVMMCA \
-lLLVMMCDisassembler \
-lLLVMLTO \
-lLLVMPasses \
-lLLVMCFGuard \
-lLLVMCoroutines \
-lLLVMipo \
-lLLVMVectorize \
-lLLVMLinker \
-lLLVMInstrumentation \
-lLLVMFrontendOpenMP \
-lLLVMFrontendOpenACC \
-lLLVMFrontendHLSL \
-lLLVMExtensions \
-lLLVMDWARFLinker \
-lLLVMGlobalISel \
-lLLVMMIRParser \
-lLLVMAsmPrinter \
-lLLVMSelectionDAG \
-lLLVMCodeGen \
-lLLVMObjCARCOpts \
-lLLVMIRPrinter \
-lLLVMInterfaceStub \
-lLLVMFileCheck \
-lLLVMFuzzMutate \
-lLLVMTarget \
-lLLVMScalarOpts \
-lLLVMInstCombine \
-lLLVMAggressiveInstCombine \
-lLLVMTransformUtils \
-lLLVMBitWriter \
-lLLVMAnalysis \
-lLLVMProfileData \
-lLLVMSymbolize \
-lLLVMDebugInfoPDB \
-lLLVMDebugInfoMSF \
-lLLVMDebugInfoDWARF \
-lLLVMObject \
-lLLVMTextAPI \
-lLLVMMCParser \
-lLLVMIRReader \
-lLLVMAsmParser \
-lLLVMMC \
-lLLVMDebugInfoCodeView \
-lLLVMBitReader \
-lLLVMFuzzerCLI \
-lLLVMCore \
-lLLVMRemarks \
-lLLVMBitstreamReader \
-lLLVMBinaryFormat \
-lLLVMTargetParser \
-lLLVMTableGen \
-lLLVMSupport \
-lLLVMDemangle \
# -lrt \
# -ldl \
# -lpthread \
# -lm \
# -lz \
# -lzstd \
# -lxml2 

.PHONY: main

main: main.cpp ${OBJ}
	${CC} ${CFLAGS} ${LLVMFLAGS} ${OBJ} $< -o $@.o  ${LDLIBS}
#${LINKFLAG}
clean:
	rm -r ${OBJ}

%.o: %.cpp ${HEADERS}
	${CC} ${CFLAGS} ${LLVMFLAGS} -c $< $ -o $@ 
