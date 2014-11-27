#Makefile

all:	folders
	g++ -o obj/cost.o -c -Werror -std=c++0x -O2 -Iinclude src/cost.cc
	g++ -o obj/greedy.o -c -Werror -std=c++0x -O2 -Iinclude src/greedy.cc
	g++ -o obj/input.o -c -Werror -std=c++0x -O2 -Iinclude src/input.cc
	g++ -o obj/localSearch.o -c -Werror -std=c++0x -O2 -Iinclude src/localSearch.cc
	g++ -o obj/main.o -c -Werror -std=c++0x -O2 -Iinclude src/main.cc
	g++ -o obj/memetic.o -c -Werror -std=c++0x -O2 -Iinclude src/memetic.cc
	g++ -o obj/tabuSearch.o -c -Werror -std=c++0x -O2 -Iinclude src/tabuSearch.cc
	g++ -o bin/qap obj/cost.o obj/greedy.o obj/input.o obj/localSearch.o obj/main.o obj/memetic.o obj/tabuSearch.o

folders:
	mkdir -p bin obj

clear:
	rm bin/* obj/*
