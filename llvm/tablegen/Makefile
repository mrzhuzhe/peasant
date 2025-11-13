# https://github.com/ISRC-CAS/tablegen-example/
# todo thereis a bug in let


LLVM_INCLUDE := /mnt/Data/software/llvm-22-x86/install/include # llvm include directory
LLVM_TBLGEN  := llvm-tblgen  # llvm-tblgen tool

# output files
RECORDS = class.inc let.inc template.inc multiclass.inc foreach.inc
FOOINC = FooGenRegisterInfo.inc FooGenRegisterInfoOrigin.inc FooGenInstrInfo.inc FooGenMCCodeEmitter.inc \
FooGenMCPseudoLowering.inc FooGenAsmMatcher.inc FooGenAsmWriter.inc FooGenAsmWriter.inc FooGenDAGISel.inc \
FooGenSubtargetInfo.inc FooGenDisassemblerTables.inc FooGenDFAPacketizer.inc

all: ${RECORDS} ${FOOINC}

${RECORDS}: %.inc: Records/%.td
	${LLVM_TBLGEN} $< -o outputs/$@

FooGenRegisterInfoOrigin.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -o outputs/$@

FooGenRegisterInfo.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-register-info -o outputs/$@

FooGenInstrInfo.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-instr-info -o outputs/$@

FooGenMCCodeEmitter.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-emitter -o outputs/$@

FooGenMCPseudoLowering.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-pseudo-lowering -o outputs/$@

FooGenAsmMatcher.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-asm-matcher -o outputs/$@

FooGenAsmWriter.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-asm-writer -o outputs/$@

FooGenDAGISel.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-dag-isel -o outputs/$@

FooGenSubtargetInfo.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-subtarget -o outputs/$@

FooGenDisassemblerTables.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-disassembler -o outputs/$@

FooGenDFAPacketizer.inc: Instr/*.td
	${LLVM_TBLGEN} -I ${LLVM_INCLUDE} -I Instr Instr/Foo.td -gen-dfa-packetizer -o outputs/$@

clean:
	rm -f *.inc