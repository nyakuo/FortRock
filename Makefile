CFLAGS         = -std=c++11
INPUT_FORTRAN  = ./fortran/lcm/lcm.f
INPUT_LLVM     = ./fortran/lcm/lcm_opt.s
OUTPUT_VERILOG = output.v

fortrock: fortrock.cpp
	g++ -g fortrock.cpp `llvm-config --cflags --ldflags --libs` -c -o fortrock.o $(CFLAGS)
	g++ -g -shared -o fortrock.so fortrock.o $(CFLAGS)

use: fortrock.so
	opt -load=./fortrock.so -instnamer -fortrock $(INPUT_LLVM) -S 1> /dev/null 2> $(OUTPUT_VERILOG)

llvm:
	gfortran-4.7 -O3 $(INPUT_FORTRAN) -fplugin=/usr/lib/gcc/x86_64-linux-gnu/4.7/plugin/dragonegg.so -fplugin-arg-dragonegg-emit-ir -S

clean:
	rm *.o *.so 2> /dev/null
