# Example

This folder contains some source code I used to do the testing.
They are compiled with the following:

```makefile
PASSPATH="/home/peter/llvm-toy-pass/lib/lib<CustomePassName>Pass.so"
CC="clang -Xclang -load -Xclang $PASSPATH"
```

This way we can add extra pass while compiling.

## Dependency

For _git_, _gnu\_nano_, _gnu\_bash_, you need some dependencies to compile them.
Please do the following:

```shell
sudo apt-get install libcurl4-openssl-dev 
sudo apt install gawk
sudo apt-get install autoconf 
sudo apt install autopoint
sudo apt install libncurses5-dev 
```

## Results

This folder maintains the output of the analysis we've done on all the submodules. 
Current, only git and bash are tested.
However, the data we found is satisfying and we decided to continue our work.

We added OverflowingBinaryPass to compile these two tools and found the following:

|   git   |  true  |  false  |    total    | percentage |
| ------- | ------ | ------- | ----------- | ---------- |
|   O0    |  5454  |  20496  |  12981(*2)  |   21.01%   |
|   O2    |  8292  |  24898  |  16595(*2)  |   24.98%   |

|  gnu_bash   |  true  |  false  |    total    | percentage |
| ----------- | ------ | ------- | ----------- | ---------- |
|     O0      |  2039  |   3475  |   2757(*2)  |   36.98%   |
|     O2      |  2501  |   4955  |   3727(*2)  |   33.55%   |

|  gnu_nano   |  true  |  false  |    total    | percentage |
| ----------- | ------ | ------- | ----------- | ---------- |
|     O0      |    20  |    206  |    113(*2)  |    8.85%   |
|     O2      |   420  |   2204  |   1312(*2)  |   16.01%   |

We are happy to find that LLVM is able to cut many of overflowing binary operation checking by statically analysising them and report that they will not overflow.