CFLAGS = -std=c++11

fortrock: fortrock.cpp
	g++ -g fortrock.cpp `llvm-config --cflags --ldflags --libs` -c -o fortrock.o $(CFLAGS)
	g++ -g -shared -o fortrock.so fortrock.o $(CFLAGS)

# lib: fortrock.o
# 	g++ -g -shared -o fortrock.so fortrock.o $(CFLAGS)

use: fortrock.so
	opt -load=./fortrock.so -fortrock lcm_opt.s -S > /dev/null

clean:
	rm *.o *.so

