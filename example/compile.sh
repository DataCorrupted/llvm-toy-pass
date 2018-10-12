cd $1
PASSPATH="/home/peter/llvm-toy-pass/lib/libOverflowingBinaryPass.so"
CFLAGS="-g -Wno-parentheses -Wno-format-security"

make CC="clang -Xclang -load -Xclang $PASSPATH" CFLAGS="$CFLAGS -O2" |& tee "$1-O2.out"
# force re-compile
rm lib/*.o *.a *.so

make CC="clang -Xclang -load -Xclang $PASSPATH" CFLAGS="$CFLAGS -O0" |& tee "$1-O0.out"
# force re-compile
rm src/*.o lib/*.o *.o *.a *.so

mv $1-O?.out ../logging/