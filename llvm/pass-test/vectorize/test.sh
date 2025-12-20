export bin_path=/mnt/build/llvm-project/build/bin
gdb --ex "b zzLoopVectorizePass::run" --args ${bin_path}/opt -load-pass-plugin=build/libnew.so -passes="inferattrs,\
cgscc(devirt<4>(inline,\
function-attrs<skip-non-recursive-function-attrs>,\
function<eager-inv;no-rerun>(sroa<modify-cfg>,\
simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,\
instcombine<max-iterations=1;no-verify-fixpoint>,\
loop(loop-idiom,\
indvars),\
sroa<modify-cfg>,\
mldst-motion<no-split-footer-bb>,\
correlated-propagation,\
simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,\
instcombine<max-iterations=1;no-verify-fixpoint>),\
function-attrs,\
function(require<should-not-run-function-passes>),\
coro-split,\
coro-annotation-elide)),\
globalopt,\
globaldce,\
function<eager-inv>(drop-unnecessary-assumes,\
lower-constant-intrinsics,\
loop(loop-rotate<header-duplication;no-prepare-for-lto>),\
inject-tli-mappings,\
zztest,\
infer-alignment,\
instcombine<max-iterations=1;no-verify-fixpoint>,\
simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,\
transform-warning,\
sroa<preserve-cfg>,\
infer-alignment,\
instcombine<max-iterations=1;no-verify-fixpoint>,\
tailcallelim)" outputs/simd.ll -S 

# b zzLoopVectorizePass::run
# b LoopVectorize.cpp:10304
# https://llvm.org/docs/SourceLevelDebugging.html
# -stats Statistics are disabled.  Build with asserts or with -DLLVM_FORCE_ENABLE_STATS
# https://llvm.org/docs/VectorizationPlan.html