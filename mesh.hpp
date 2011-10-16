#define REAL double
#include "triangle/triangle.h"
#include "cell.hpp"
#include <vector>
#include <cstring>
#include <cstdlib>

using namespace std;

struct Boundary
{
  double start[2], end[2]; // Endpoints of the boundary
  short int orientation;   // Orientation of boundary
  double length;           // Length of the boundary
  double distance;         // Distance to the center of the local mesh
  int neighboor;           // MPI id of the neighboor on this boundary

  ~Boundary() {
    free(&start);
    free(&end);
  }
};

class Mesh
{
  public:
    double center[2];             // Physical location of the center of the mesh
    vector<Boundary*> boundaries; // Vector of the bounding faces of this mesh
    vector<Cell*> cells;          // Vector of the cells contained in this mesh
    int nghosts;                  // Number of ghosts needed.  This will change depth of cells outside boundary
    int meshID;                   // MPI ID of this Mesh

    Mesh(double icenter[2], vector<Boundary*> inboundaries, int innghosts) : nghosts(innghosts), boundaries(inboundaries) {
      memcpy(center, icenter, 2 * sizeof(double));
      meshID     = 0;
      cells      = vector<Cell*>();
      boundaries = vector<Boundary*>();
    }

    ~Mesh() {
      delete[] &boundaries;
      delete[] &cells;
      free(&center);
    }

    // Add a cell to the current mesh.  Return False if the cell is outside of boundary
    bool add_cell(Cell *item);

    // Remove a cell from the current mesh.  Return false if the cell is not valid
    bool remove_cell(Cell *item);
    bool remove_cell(int idx);

    // Sets new boundaries for the current mesh.  Returns False if new boundary fails
    //   sanity tests.  Will also update this->center.
    bool new_boundaries(vector<Boundary> newboundaries);

    // Retesselate the mesh and update cell quantities.
    bool tesselate();

    // Syncronizes mesh with other meshes on MPI topography.  This will exchange ghost zones if
    //   necessary
    bool sync_mesh();
};

