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

	class OpInfo{
	private:
		unsigned m_opcode;
		char m_opChar;
		std::string m_opStr;
		unsigned m_occCut;

	public:
		OpInfo(){;}
		OpInfo(unsigned opcode, char opChar, std::string opStr): 
			m_opcode(opcode), m_opChar(opChar), m_opStr(opStr), m_occCut(0){;}

		static std::pair<unsigned, OpInfo> getUnsignedInstancePair(
			unsigned opcode, char opChar, std::string opStr){
			return std::pair<unsigned, OpInfo>(
				opcode, OpInfo(opcode, opChar, opStr));
		};
		// Lessons taken:
		//   Modifying a value with const is meaningless,
		//   only objects can be modified with const.
		unsigned getOpcode() const { return m_opcode; }
		char getOpChar() const { return m_opChar; }
		const std::string& getOpStr() const { return m_opStr; }
		unsigned getOccuranceCount() const { return m_occCut; }
		void inc() { m_occCut ++; }
	};
	
	typedef std::pair<unsigned, OpInfo> OpInfoPair;
	std::map<unsigned, OpInfo> m_opInfoMap;
	std::list<char> m_checkOpList;

public:
	static char ID;
	CmdLineControlPass(): ModulePass(ID){
		errs() << "-- CmdLineControlPass --\n";

		m_opInfoMap.insert(OpInfo::getUnsignedInstancePair(    Instruction::Mul, '*', " Mul"));
		m_opInfoMap.insert(OpInfo::getUnsignedInstancePair(    Instruction::Shl, '<', " Shl"));
		m_opInfoMap.insert(OpInfo::getUnsignedInstancePair(    Instruction::Add, '+', " Add"));
		m_opInfoMap.insert(OpInfo::getUnsignedInstancePair(    Instruction::Sub, '-', " Sub"));
		m_opInfoMap.insert(OpInfo::getUnsignedInstancePair(BinaryOperator::SDiv, '/', "SDiv"));
		m_opInfoMap.insert(OpInfo::getUnsignedInstancePair(BinaryOperator::SRem, '%', "SRem"));
		m_opInfoMap.insert(OpInfo::getUnsignedInstancePair(BinaryOperator::UDiv, '/', "UDiv"));
		m_opInfoMap.insert(OpInfo::getUnsignedInstancePair(BinaryOperator::URem, '%', "URem"));

		const char* checkOpEnv = getenv("SAN_OP");
		if (checkOpEnv && checkOpEnv[0] != 0) {
			std::string checkOpStr = checkOpEnv;
			for (char c: checkOpStr){
				if (c == '0') {
					m_checkOpList =  {'+', '-'} ;
					break;
				} else if (c == '1'){
					m_checkOpList = {'+', '-', '*', '/', '%'};
					break;
				} else if (c == 'a' || c == 'A' || c == '2') {
					m_checkOpList = {'+', '-', '*', '/', '%', '<'};		
					break;
				} else if (c == '+' || c == '-' || c == '*' || 
						   c == '/' || c == '<' || c == '%') {
					m_checkOpList.push_back(c);
				}
			}
		} else {
			m_checkOpList = {'+', '-', '*', '/', '%', '<'};
		}

		errs() << "Your are tracking the following operations: ";
		for (char c: m_checkOpList){
			errs() << c << " ";
		}
		errs() << "\n";
	};
	
	virtual bool runOnModule(Module& module){
		for (const Function& function: module){
			for (const BasicBlock& basicBlock: function){
				for (const Instruction& instruction: basicBlock){
					unsigned opcode = instruction.getOpcode();
					if (
					  m_opInfoMap.find(opcode) != m_opInfoMap.end() 
					&&
					  std::find(
							m_checkOpList.begin(), m_checkOpList.end(),
							m_opInfoMap[opcode].getOpChar()) 
					  != m_checkOpList.end()) {
						m_opInfoMap[opcode].inc();
					}
				}
			}
		}
		return false;
	}
	~CmdLineControlPass(){
		for (std::pair<const unsigned, OpInfo>& pair: m_opInfoMap){
			OpInfo& opInfo = pair.second;
			if (std::find(
					m_checkOpList.begin(), m_checkOpList.end(),
					opInfo.getOpChar())
			  != m_checkOpList.end()){
				errs() << opInfo.getOpStr() << ": " << opInfo.getOccuranceCount() << "\n";
			} else {
				errs() << opInfo.getOpStr() << ": " << "Uncounted.\n";
			}
		}
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
