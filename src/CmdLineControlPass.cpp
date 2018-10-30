#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/Instruction.h"

#include <list>

using namespace llvm;

namespace {

class CmdLineControlPass: public ModulePass{
private:
	std::map<unsigned, unsigned> m_opCntMap;
	std::map<unsigned, char> m_opCharMap;
	std::list<char> m_checkOpList;
public:
	static char ID;
	CmdLineControlPass(): ModulePass(ID){
		errs() << "-- CmdLineControlPass --\n";

		m_opCharMap[Instruction::Mul] = '*';
		m_opCharMap[Instruction::Shl] = '<';
		m_opCharMap[Instruction::Add] = '+';
		m_opCharMap[Instruction::Sub] = '-';
		m_opCharMap[BinaryOperator::SDiv] = '/';
		m_opCharMap[BinaryOperator::SRem] = '%';
		m_opCharMap[BinaryOperator::UDiv] = '/';
		m_opCharMap[BinaryOperator::URem] = '%';

		m_opCntMap[Instruction::Mul] = 0;
		m_opCntMap[Instruction::Shl] = 0;
		m_opCntMap[Instruction::Add] = 0;
		m_opCntMap[Instruction::Sub] = 0;
		m_opCntMap[BinaryOperator::SDiv] = 0;
		m_opCntMap[BinaryOperator::SRem] = 0;
		m_opCntMap[BinaryOperator::UDiv] = 0;
		m_opCntMap[BinaryOperator::URem] = 0;

		const char* checkOpEnv = getenv("SAN_OP");
		if (checkOpEnv) {
			std::string checkOpStr = checkOpEnv;
			for (char c: checkOpStr){
				errs() << c ;
				if (c == 'a' || c == 'A') {
					m_checkOpList = {'+', '-', '*', '/', '<', '%'};		
					break;
				} else if (c == '+' || c == '-' || c == '*' || 
						   c == '/' || c == '<' || c == '%') {
					m_checkOpList.push_back(c);
				}
			}
		} else {
			m_checkOpList = {'+', '-', '*', '/', '<', '%'};
		}
		errs() << "\n";
	};
	
	virtual bool runOnModule(Module& module){
		for (const Function& function: module){
			for (const BasicBlock& basicBlock: function){
				for (const Instruction& instruction: basicBlock){
					if (
					  std::find(
							m_checkOpList.begin(), m_checkOpList.end(),
							m_opCharMap[instruction.getOpcode()]) 
					  != m_checkOpList.end()) {
						m_opCntMap[instruction.getOpcode()]++;
					}
				}
			}
		}
		return false;
	}
	~CmdLineControlPass(){
		errs() << " Mul: " << m_opCntMap[Instruction::Mul] << "\n";
		errs() << " Shl: " << m_opCntMap[Instruction::Shl] << "\n";
		errs() << " Add: " << m_opCntMap[Instruction::Add] << "\n";
		errs() << " Sub: " << m_opCntMap[Instruction::Sub] << "\n";
		errs() << "SDiv: " << m_opCntMap[BinaryOperator::SDiv] << "\n";
		errs() << "SRem: " << m_opCntMap[BinaryOperator::SRem] << "\n";
		errs() << "UDiv: " << m_opCntMap[BinaryOperator::UDiv] << "\n";
		errs() << "URem: " << m_opCntMap[BinaryOperator::URem] << "\n";
	}
}; // enf of CmdLineControlPass
}  // end of anonymous namespace 


char CmdLineControlPass::ID = 0;
static RegisterPass<CmdLineControlPass> X(
        "CmdLineControlPass",
        "A pass to examine all binary opearions");

static void registerCmdLineControlPass(
        const PassManagerBuilder&, legacy::PassManagerBase &PM) {
    PM.add(new CmdLineControlPass());
}

static RegisterStandardPasses registerCmdLineControlPassVarOptLast(
        PassManagerBuilder::EP_OptimizerLast, registerCmdLineControlPass);
static RegisterStandardPasses registerCmdLineControlPassVarOptLevel0(
        PassManagerBuilder::EP_EnabledOnOptLevel0, registerCmdLineControlPass);
