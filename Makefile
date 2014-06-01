fortrock: fortrock.cpp
	g++ fortrock.cpp `llvm-config --cflags --ldflags --libs` -c -o fortrock.o

fortrock_lib: fortrock.o
	g++ -g -shared -o ./bin/fortrock.so fortrock.o
