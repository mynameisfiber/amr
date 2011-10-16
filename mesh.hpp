#define REAL double
#include "triangle/triangle.h"
#include "cell.hpp"
#include <vector>

using namespace std;

struct Boundary
{
  double x[2], y[2]; // Endpoints of the boundary
  double length;     // Length of the boundary
  double distance;   // Distance to the center of the local mesh
  int neighboor;     // MPI id of the neighboor on this boundary
};

class Mesh
{
  public:
    double center[2];            // Physical location of the center of the mesh
    vector<Boundary> boundaries; // Vector of the bounding faces of this mesh
    vector<Cell> cells;          // Vector of the cells contained in this mesh
    int nghosts;                 // Number of ghosts needed.  This will change depth of cells outside boundary

    // Add a cell to the current mesh.  Return False if the cell is outside of boundary
    bool add_cell(Cell item);

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

