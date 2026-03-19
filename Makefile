a.out: Mensajes.o Fecha.o Cuac.o DiccionarioCuacs.o TablaHash.o ArbolAVL.o Interprete.o main.o
	g++ Mensajes.o Fecha.o Cuac.o DiccionarioCuacs.o TablaHash.o ArbolAVL.o Interprete.o main.o

Mensajes.o: Mensajes.cpp Mensajes.h
	g++ -c Mensajes.cpp

Fecha.o: Fecha.cpp Fecha.h
	g++ -c Fecha.cpp

Cuac.o: Cuac.cpp Mensajes.h Fecha.h Cuac.h
	g++ -c Cuac.cpp

DiccionarioCuacs.o: DiccionarioCuacs.cpp Cuac.h DiccionarioCuacs.h TablaHash.h ArbolAVL.h
	g++ -c DiccionarioCuacs.cpp

Interprete.o: Interprete.cpp Interprete.h Fecha.h Cuac.h
	g++ -c Interprete.cpp

TablaHash.o: TablaHash.cpp Fecha.h Cuac.h TablaHash.h
	g++ -c TablaHash.cpp

ArbolAVL.o: ArbolAVL.cpp ArbolAVL.h Cuac.h
	g++ -c ArbolAVL.cpp

main.o: main.cpp Interprete.h TablaHash.h
	g++ -c main.cpp

clean:
	rm -f *.o a.out cuacker.exe
