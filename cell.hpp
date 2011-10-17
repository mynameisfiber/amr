#include <vector>
#include <cstring>
#include <cstdlib>
#include <cmath>
using namespace std;

class Cell;

struct Neighboor
{
  Cell *cell;
  vector<double> dx;
  double distance;

  Neighboor(Cell *me, Cell *them);
};

struct Face
{
  Cell *cell;
  vector<double> start, end; // Endpoints of the face
  short int orientation;     // Orientation of this face
  double length;             // Length of the face

  Face(Cell *incell, vector<double> instart, vector<double> inend);
};

class Cell
{
  public:
    vector<double> x;             // The physical location of the center of this cell
    vector<double> data;          // The actual data this node contains

    vector<Neighboor> neighboors; // Pointers to neighbooring cells as defined by the Delauny triangulation
    double area;                  // The area of this cell
    vector<Face> faces;           // The faces that define this area

    bool active;                  // True if this node is active, false if it is a ghost zone
    vector<int> nodes;            // ID's of the MPI nodes this node is defined on.

    Cell(vector<double> ix, int ndata, bool iactive=true) : x(ix), active(iactive) {
      data       = vector<double>(ndata, 0.0);
      neighboors = vector<Neighboor>();
      faces      = vector<Face>();
      nodes      = vector<int>();
    }

    double operator[](int idx) { return data[idx]; }
};

