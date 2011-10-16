
CFLAG=

mesh: mesh.cpp mesh.hpp cell.cpp cell.hpp
	g++ -o mesh -std=gnu++0x mesh.cpp $(CFLAG)
