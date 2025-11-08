# https://github.com/ISRC-CAS/tablegen-example/
# todo thereis a bug in let


LLVM_INCLUDE := /mnt/Data/software/llvm-22-x86/install/include # llvm include directory
LLVM_TBLGEN  := llvm-tblgen  # llvm-tblgen tool

# output files
RECORDS = class.inc let.inc template.inc multiclass.inc foreach.inc
FOOINC = FooGenRegisterInfo.inc FooGenRegisterInfoOrigin.inc

all: ${RECORDS} ${FOOINC}

${RECORDS}: %.inc: Records/%.td
	${LLVM_TBLGEN} $< -o outputs/$@

FooGenRegisterInfoOrigin.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -o outputs/$@

FooGenRegisterInfo.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-register-info -o outputs/$@

# FooGenInstrInfo.inc: Foo/*.td
# 	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Foo Foo/Foo.td -gen-instr-info -o $@

# FooGenMCCodeEmitter.inc: Foo/*.td
# 	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Foo Foo/Foo.td -gen-emitter -o $@

# FooGenMCPseudoLowering.inc: Foo/*.td
# 	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Foo Foo/Foo.td -gen-pseudo-lowering -o $@

# FooGenAsmMatcher.inc: Foo/*.td
# 	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Foo Foo/Foo.td -gen-asm-matcher -o $@

# FooGenAsmWriter.inc: Foo/*.td
# 	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Foo Foo/Foo.td -gen-asm-writer -o $@

# FooGenDAGISel.inc: Foo/*.td
# 	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Foo Foo/Foo.td -gen-dag-isel -o $@

# FooGenSubtargetInfo.inc: Foo/*.td
# 	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Foo Foo/Foo.td -gen-subtarget -o $@

# FooGenDisassemblerTables.inc: Foo/*.td
# 	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Foo Foo/Foo.td -gen-disassembler -o $@

# FooGenDFAPacketizer.inc: Foo/*.td
# 	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Foo Foo/Foo.td -gen-dfa-packetizer -o $@

clean:
	rm -f *.inc