sim : core.o sim.o
	g++ core.o sim.o -o sim -std=c++11
core.o : core.cpp core.h
	g++ -c core.cpp -std=c++11
sim.o : sim.cpp core.h
	g++ -c sim.cpp -std=c++11
