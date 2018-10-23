#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

namespace {

class AddIfThenBranchPass: public FunctionPass{
private:

	Type* VoidTy = nullptr;
	IntegerType* Int32Ty = nullptr;

	Value* CONST_ONE = nullptr;
	Value* CONST_TWO = nullptr;
	Value* CONST_TEN = nullptr;

	Constant* m_Function = nullptr;

	void initializeVar(Function& function){

		LLVMContext& context = function.getContext();

		VoidTy = Type::getVoidTy(context);
		Int32Ty = IntegerType::get(context, 32);

		CONST_ONE = ConstantInt::get(Int32Ty, 1, true);
		CONST_TWO = ConstantInt::get(Int32Ty, 2, true);
		CONST_TEN = ConstantInt::get(Int32Ty, 10, true);

		m_Function = function.getParent()->getOrInsertFunction("abort", VoidTy, nullptr);
	}

public:

	static char ID;

	AddIfThenBranchPass(): FunctionPass(ID){};
	virtual bool runOnFunction(Function& function){
		
		bool modified = false;
		this->initializeVar(function);

		for (BasicBlock& basicBlock: function){
			for (Instruction& instruction: basicBlock){
				if (instruction.getOpcode() == BinaryOperator::Add){
					errs() << "An add instruction found, inserting branch.\n";

					Value* arg0 = instruction.getOperand(0);
					IRBuilder<> IRB(instruction.getNextNode());

					Value* icmpSGT = IRB.CreateICmpSGT(arg0, CONST_TEN);
					Instruction* insertGTPoint = dyn_cast<Instruction>(icmpSGT);
					Instruction* ifGTThenBranch = 
						SplitBlockAndInsertIfThen(icmpSGT, insertGTPoint->getNextNode(), false);
					IRBuilder<> gTBranchIRB(ifGTThenBranch);
					gTBranchIRB.CreateCall(m_Function, {});

					Value* icmpSLT = IRB.CreateICmpSLT(arg0, CONST_ONE);
					Instruction* insertLTPoint = dyn_cast<Instruction>(icmpSLT);
					Instruction* ifLTThenBranch =
						SplitBlockAndInsertIfThen(icmpSLT, insertLTPoint->getNextNode(), false);
					IRBuilder<> lTBranchIRB(ifLTThenBranch);
					lTBranchIRB.CreateCall(m_Function, {});

					modified = true;
				}
			}
		}
		return modified;
	}
}; // enf of AddIfThenBranchPass
}  // end of anonymous namespace 


char AddIfThenBranchPass::ID = 0;
static RegisterPass<AddIfThenBranchPass> X(
        "AddIfThenBranchPass",
        "A pass to add a if-then branch whenever an add operation is encountered.");

static void registerAddIfThenBranchPass(
        const PassManagerBuilder&, legacy::PassManagerBase &PM) {
    PM.add(new AddIfThenBranchPass());
}

static RegisterStandardPasses registerAddIfThenBranchPassVarOptLast(
        PassManagerBuilder::EP_OptimizerLast, registerAddIfThenBranchPass);
static RegisterStandardPasses registerAddIfThenBranchPassVarOptLevel0(
        PassManagerBuilder::EP_EnabledOnOptLevel0, registerAddIfThenBranchPass);
