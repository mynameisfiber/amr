#include <vector>
#include <cstring>
#include <cstdlib>
#include <cmath>
using namespace std;

class Cell;

struct Neighboor
{
  Cell *cell;
  double dx[2];
  double distance;
};

struct Face
{
  double start[2], end[2]; // Endpoints of the face
  short int orientation;   // Orientation of this face
  double length;           // Length of the face

  Face(double instart[2], double inend[2], short int inorientation) : orientation(inorientation) {
    memcpy(start, instart, 2*sizeof(double));
    memcpy(end, inend, 2*sizeof(double));
    length = sqrt( (end[0] - start[0])*(end[0] - start[0]) +
                   (end[1] - start[1])*(end[1] - start[1]));
  }

  ~Face() {
    free(&start);
    free(&end);
  }
};

class Cell
{
  public:
    double x[2];                  // The physical location of the center of this cell
    vector<double> data;          // The actual data this node contains

    vector<Neighboor*> neighboors; // Pointers to neighbooring cells as defined by the Delauny triangulation
    double area;                  // The area of this cell
    vector<Face*> faces;           // The faces that define this area

    bool active;                  // True if this node is active, false if it is a ghost zone
    vector<int> nodes;            // ID's of the MPI nodes this node is defined on.

    Cell(double ix[2], int ndata, bool iactive=true) : active(iactive) {
      memcpy(x, ix, 2 * sizeof(double));
      data       = vector<double>(ndata, 0.0);
      neighboors = vector<Neighboor*>();
      faces      = vector<Face*>();
      nodes      = vector<int>();
    }

    ~Cell() {
      delete[] &data;
      delete[] &neighboors;
      delete[] &faces;
      delete[] &nodes;
      free(&x);
    }

    double operator[](int idx) { return data[idx]; }
};

