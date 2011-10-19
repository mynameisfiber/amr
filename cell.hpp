#include <vector>
#include <cstdlib>
#include <cmath>
#include <stdio.h>

class Cell;

struct Neighboor
{
  Cell *cell;
  std::vector<double> dx;
  double distance;

  Neighboor(Cell *me, Cell *them);
};

struct Face
{
  Cell *cell;
  std::vector<double> start, end; // Endpoints of the face
  short int orientation;     // Orientation of this face
  double length;             // Length of the face
  double distance;           // distance to center

  Face(Cell *incell, std::vector<double> instart, std::vector<double> inend);
};

class Cell
{
  public:
    std::vector<double> x;             // The physical location of the center of this cell
    std::vector<double> data;          // The actual data this node contains

    std::vector<Neighboor> neighboors; // Pointers to neighbooring cells as defined by the Delauny triangulation
    double area;                  // The area of this cell
    std::vector<Face> faces;           // The faces that define this area

    bool active;                  // True if this node is active, false if it is a ghost zone
    std::vector<int> nodes;            // ID's of the MPI nodes this node is defined on.

    Cell(std::vector<double> ix, int ndata, bool iactive=true) : x(ix), active(iactive) {
      data       = std::vector<double>(ndata, 0.0);
      neighboors = std::vector<Neighboor>();
      faces      = std::vector<Face>();
      nodes      = std::vector<int>();
    }

    double operator[](int idx) { return data[idx]; }

    void write(char *filename, char *mode);
    void write(FILE *fd);
};

