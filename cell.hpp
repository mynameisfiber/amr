#include <vector>
#include <string.h>
using namespace std;

class Cell;

struct Neighboor
{
  Cell *cell;
  double distance;
};

struct Face
{
  double start[2], end[2]; // Endpoints of the face
  double length;           // Length of the face
  double distance;         // Distance to the center of the cell
};

class Cell
{
  public:
    double x[2];                  // The physical location of the center of this cell
    vector<double> data;          // The actual data this node contains

    vector<Neighboor> neighboors; // Pointers to neighbooring cells as defined by the Delauny triangulation
    double area;                  // The area of this cell
    vector<Face> faces;           // The faces that define this area

    bool active;                  // True if this node is active, false if it is a ghost zone
    vector<int> nodes;            // ID's of the MPI nodes this node is defined on.

    Cell(double ix[2], int ndata, bool iactive=true) : active(iactive) {
      memcpy(x, ix, 2 * sizeof(double));
      data = vector<double>(ndata, 0.0);
    }

    ~Cell() {
      delete[] &data;
      delete[] &neighboors;
      delete[] &faces;
      delete[] &nodes;
    }

    double operator[](int idx) { return data[idx]; }
};

