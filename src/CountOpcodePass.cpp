#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <map>
#include <string>

using namespace llvm;

namespace {

class CountOpcodePass: public FunctionPass{
private:
    std::map<std::string, int> opcodeCnt;
public:
    static char ID;
    CountOpcodePass():FunctionPass(ID){ };
    virtual bool runOnFunction(Function& function){
        errs() << "Passing Function: " << function.getName() << "\n";
        for (const BasicBlock& basicBlock: function){
            for (const Instruction& instruction: basicBlock){
                const char * opcode = instruction.getOpcodeName();
                if (opcodeCnt.find(opcode) == opcodeCnt.end()){
                    opcodeCnt[opcode] = 1;
                } else {
                    opcodeCnt[opcode] +=1;
                }
            }
        }
        for (auto entry: opcodeCnt){
            errs() << "opcode: " << entry.first << "\t occurrence: " << entry.second << "\n";
        }

        errs() << "\n";
        return false;
    }
}; // end of CountOpcodePass
}  // end of anonymous namespace

char CountOpcodePass::ID = 0;
static RegisterPass<CountOpcodePass> X(
        "CountOpcodePass",
        "A pass to count the occurrence of all opcodes");

static void registerCountOpcodePass(
        const PassManagerBuilder&, legacy::PassManagerBase &PM) {
    PM.add(new CountOpcodePass());
}

/*
 * The following note is copied from
 *   * https://github.com/rdadolf/clangtool/blob/master/clangtool.cpp
 * , which helped a lot.
 *
 * Note: The location EP_OptimizerLast places this pass at the end of the list
 * of *optimizations*. That means on -O0, it does not get run.

 * In general, adding your pass twice will cause it to run twice, but in this
 * particular case, the two are disjoint (EP_EnabledOnOptLevel0 only runs if
 * you're in -O0, and EP_OptimizerLast only runs if you're not). You can check
 *  * include/llvm/Transforms/IPO/PassManagerBuilder.h
 *  * lib/Transforms/IPO/PassManagerBuilder.cpp
 * for the exact behavior.
 */
static RegisterStandardPasses registerCountOpcodePassVarOptLast(
        PassManagerBuilder::EP_OptimizerLast, registerCountOpcodePass);
static RegisterStandardPasses registerCountOpcodePassVarOptLevel0(
        PassManagerBuilder::EP_EnabledOnOptLevel0, registerCountOpcodePass);
