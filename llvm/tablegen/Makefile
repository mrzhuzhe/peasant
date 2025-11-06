# https://github.com/ISRC-CAS/tablegen-example/
# todo thereis a bug in let

# LLVM_INCLUDE := llvm/include # llvm include directory
LLVM_TBLGEN  := llvm-tblgen  # llvm-tblgen tool

# output files
RECORDS = class.inc let.inc template.inc multiclass.inc foreach.inc

all: ${RECORDS}

${RECORDS}: %.inc: Records/%.td
	${LLVM_TBLGEN} $< -o outputs/$@

clean:
	rm -f *.inc