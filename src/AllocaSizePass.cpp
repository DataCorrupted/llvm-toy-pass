#include "llvm/Pass.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Constants.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Casting.h"

#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {

class AllocaSizePass: public FunctionPass{

public:
	static char ID;
	AllocaSizePass(): FunctionPass(ID){; }
	virtual bool runOnFunction(Function& function){
        errs() << "Passing Function: " << function.getName() << "\n";
        for (const BasicBlock& basicBlock: function){
            for (const Instruction& instruction: basicBlock){
                errs() << instruction.getOpcodeName() << " ";
                // Not null instruction, this dynamic cast is successful.
                if (const AllocaInst* allocaInst = dyn_cast<AllocaInst>(&instruction)) {
                    errs() << "This is a AllocaInst. ";
                    if (allocaInst->isArrayAllocation()){
                        const Value* value =  allocaInst->getArraySize();
                        errs() << "It's an array allocation which asked "
                            << *value
                            << " elements with type: "
                            << *allocaInst->getType()->getElementType();
                    } else if (auto sequentialType = dyn_cast<SequentialType>(allocaInst->getType()->getElementType())) {
                        errs() << "It's a const sized array which has "
                            << sequentialType->getNumElements()
                            << " elements with type: "
                            << *sequentialType->getElementType();
                    } else {
                        errs() << "But it didn't allocate an array, "
                            << "it asked for a type of "
                            << *allocaInst->getType()->getElementType();
                    }
                }
                errs() << "\n";
            }
        }
        return false;
	}

}; // end of AllocSizePass
}  // end of anonymous namespace

char AllocaSizePass::ID = 0;
static RegisterPass<AllocaSizePass> X(
        "AllocaSizePass",
        "A pass used to examine how much size is allocated for each AllocaInst.");

static void registerCountOpcodePass(
        const PassManagerBuilder&, legacy::PassManagerBase &PM) {
    PM.add(new AllocaSizePass());
}
static RegisterStandardPasses registerCountOpcodePassVarOptLast(
        PassManagerBuilder::EP_OptimizerLast, registerCountOpcodePass);
static RegisterStandardPasses registerCountOpcodePassVarOptLevel0(
        PassManagerBuilder::EP_EnabledOnOptLevel0, registerCountOpcodePass);