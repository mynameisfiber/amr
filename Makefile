
CFLAG=-g

mesh: triangle.o cell.o mesh.o
	g++ -o mesh triangle.o mesh.o cell.o

mesh.o: mesh.cpp mesh.hpp
	g++ -c -std=gnu++0x mesh.cpp  $(CFLAG)

cell.o: cell.cpp cell.hpp
	g++ -c -std=gnu++0x cell.cpp $(CFLAG)

triangle.o: triangle/triangle.c triangle/triangle.h
	gcc -c triangle/triangle.c -lm -DTRILIBRARY $(CFLAG)

clean:
	rm -rf mesh *.o
