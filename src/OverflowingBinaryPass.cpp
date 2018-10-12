#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {

class OverflowingBinaryPass: public FunctionPass{
public:
	static char ID;
	OverflowingBinaryPass(): FunctionPass(ID){};
	virtual bool runOnFunction(Function& function){
		errs() << "Passing Function: " << function.getName() << "\n";
		errs() << "  Opearion | hasNoUnsignedWrap | hasNoSignedWrap  \n";
		for (const BasicBlock& basicBlock: function){
			for (const Instruction& instruction: basicBlock){
				if (const OverflowingBinaryOperator* overflowBinOp 
                  = dyn_cast<OverflowingBinaryOperator>(&instruction)){
                  	errs() << "|   ";
					errs() << instruction.getOpcodeName() << "    |       ";
					errs() << (overflowBinOp->hasNoUnsignedWrap()? "true " : "false") << "       |      ";
					errs() << (overflowBinOp->hasNoSignedWrap()? "true " : "false") << "      |\n";
				}
			}
		}
		errs() << "\n";
		return false;
	}
}; // enf of OverflowingBinaryPass
}  // end of anonymous namespace 


char OverflowingBinaryPass::ID = 0;
static RegisterPass<OverflowingBinaryPass> X(
        "OverflowingBinaryPass",
        "A pass to examine all binary opearions");

static void registerOverflowingBinaryPass(
        const PassManagerBuilder&, legacy::PassManagerBase &PM) {
    PM.add(new OverflowingBinaryPass());
}

static RegisterStandardPasses registerOverflowingBinaryPassVarOptLast(
        PassManagerBuilder::EP_OptimizerLast, registerOverflowingBinaryPass);
static RegisterStandardPasses registerOverflowingBinaryPassVarOptLevel0(
        PassManagerBuilder::EP_EnabledOnOptLevel0, registerOverflowingBinaryPass);
