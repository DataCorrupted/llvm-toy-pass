# llvm-toy-pass

This repo is used to learn llvm by writing some toy passes myself.

20180930 Update.   
This repo will be paused for sometime as I will turn to integer over flow problems for now.
I will get back and start working on malloc later.

## Quick start

### How to compile
http://llvm.org/docs/WritingAnLLVMPass.html#setting-up-the-build-environment  
[Makefile](src/Makefile)

### How to write a pass
http://llvm.org/docs/WritingAnLLVMPass.html#basic-code-required
http://laure.gonnord.org/pro/research/ER03_2015/lab3_intro.pdf

### How to run a pass with clang
http://www.cs.cornell.edu/~asampson/blog/clangpass.html
https://github.com/rdadolf/clangtool/blob/master/clangtool.cpp

## Reference

Some useful papers are added here.
I might have cited them... or not. 
But I will put them here for now.

[CCured: Type-Safe Retrofitting of Legacy Software](http://people.eecs.berkeley.edu/~necula/Papers/ccured_toplas.pdf)
[Guarder: A Tunable Secure Allocator](https://www.usenix.org/system/files/conference/usenixsecurity18/sec18-silvestro.pdf)

  
## Folders description

### src

This folder contains all the passes I wrote to practice. 
Including some experiments I did to test how llvm compiles C code to better understand buffer overflow.

### example

Some example code wrote to be the test subjects.

### lib

All the passes are stored here.
A _.keep_ file is stored inside to make sure it's alive, but no binary code have the privilege to go into git.

# Findings

## passes

You have to register so it can be loaded by clang.
Generally you have to register twice, one for optimized compiling and one for non-optimized compiling.

```cpp
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

static void register_YOURPASS_(
        const PassManagerBuilder&, legacy::PassManagerBase &PM) {
    PM.add(new _YOURPASS_());
}
// This is for optimized case.
static RegisterStandardPasses register_YOURPASS_VarOptLast(
        PassManagerBuilder::EP_OptimizerLast, register_YOURPASS_Pass);
// This is for un-optimized case.
static RegisterStandardPasses register_YOURPASS_VarOptLevel0(
        PassManagerBuilder::EP_EnabledOnOptLevel0, register_YOURPASS_Pass);
```

## AllocaSizePass

_alloca_ is just an instruction used to do stack space allocation.
Many of them, especially static ones will be optimized out in O1 but kept in O0.

It is not trivial to find the length of the array using [AllocaInst](http://llvm.org/doxygen/classllvm_1_1AllocaInst.html)

```cpp
// For arrays whose size cannot be statically determined, it will return a variable.
const Value * 	AllocaInst::getArraySize () const 

// For arrays with size that can be determined at compile time, this function should be used.
// As such array generally has type [size * type] and you can get size from that type.
PointerType * 	AllocaInst::getType () const
```
