LLVM_CONFIG ?= llvm-config

CFLAGS      ?= -O3 -funroll-loops -g
CFLAGS      += -Wall -D_FORTIFY_SOURCE=2 -g -Wno-pointer-sign \
               -DVERSION=\"$(VERSION)\"

CXXFLAGS    ?= -O3 -funroll-loops -g
CXXFLAGS    += -Wall -D_FORTIFY_SOURCE=2 -g -Wno-pointer-sign \
               -DVERSION=\"$(VERSION)\" -Wno-variadic-macros

CLANG_CFL    = `$(LLVM_CONFIG) --cxxflags` -fno-rtti -fpic $(CXXFLAGS)
CLANG_LFL    = `$(LLVM_CONFIG) --ldflags` $(LDFLAGS)

ifeq "$(origin CC)" "default"
  CC         = clang
  CXX        = clang++
endif

all: libCountOpcodePass.so libAllocaSizePass.so libOverflowingBinaryPass.so libAddIfThenBranchPass.so clean

libCountOpcodePass.so: CountOpcodePass.cpp
	$(CXX) $(CLANG_CFL) -shared $< -o $@ $(CLANG_LFL)

libAllocaSizePass.so: AllocaSizePass.cpp
	$(CXX) $(CLANG_CFL) -shared $< -o $@ $(CLANG_LFL)

libOverflowingBinaryPass.so: OverflowingBinaryPass.cpp
	$(CXX) $(CLANG_CFL) -shared $< -o $@ $(CLANG_LFL)

libAddIfThenBranchPass.so: AddIfThenBranchPass.cpp
	$(CXX) $(CLANG_CFL) -shared $< -o $@ $(CLANG_LFL)
	
clean:
	mv *.so ../lib/