a.out: 001.o 003.o 004.o 006.o 200.o 300.o Interprete.o main.o
	g++ 001.o 003.o 004.o 006.o 200.o 300.o Interprete.o main.o

001.o: 001.cpp 001.h
	g++ -c 001.cpp

003.o: 003.cpp 003.h
	g++ -c 003.cpp

004.o: 004.cpp 001.h 003.h 004.h
	g++ -c 004.cpp

006.o: 006.cpp 004.h 006.h 200.h 300.h
	g++ -c 006.cpp

Interprete.o: Interprete.cpp Interprete.h 003.h 004.h
	g++ -c Interprete.cpp

200.o: 200.cpp 003.h 004.h 200.h
	g++ -c 200.cpp

300.o: 300.cpp 300.h 004.h
	g++ -c 300.cpp

main.o: main.cpp Interprete.h 200.h
	g++ -c main.cpp


