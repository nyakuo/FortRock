CFLAGS         = -std=c++11
CXX            = clang++-3.6
INPUT_FORTRAN  = ../fortran/lcm/lcm.f
INPUT_LLVM     = ../fortran/lcm/lcm_opt.s
OUTPUT_VERILOG = output.v

fortrock: ./core/fortrock.cpp
	$(CXX) -g ./core/fortrock.cpp `llvm-config --cflags --ldflags --libs` -c -o fortrock.o $(CFLAGS)
	$(CXX) -g -shared -o fortrock.so fortrock.o $(CFLAGS)

use: ./core/fortrock.so
	opt -load=./core/fortrock.so -instnamer -fortrock $(INPUT_LLVM) -S 1> /dev/null 2> $(OUTPUT_VERILOG)

llvm:
	gfortran-4.7 -O3 $(INPUT_FORTRAN) -fplugin=/usr/lib/gcc/x86_64-linux-gnu/4.7/plugin/dragonegg.so -fplugin-arg-dragonegg-emit-ir -S

clean:
	rm *.o *.so 2> /dev/null
