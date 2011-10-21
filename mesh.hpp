#include "cell.hpp"
#include <vector>
#include <cstring>
#include <cstdlib>

class Mesh;

struct Boundary
{
  std::vector<double> start, end; // Endpoints of the boundary
  int orientation;                // Orientation of boundary
  double length;                  // Length of the boundary
  double distance;                // Distance to the center of the local mesh
  int neighboor;                  // MPI id of the neighboor on this boundary

  Boundary(Mesh *mesh, std::vector<double> instart, std::vector<double> inend);
};

class Mesh
{
  public:
    std::vector<double> center;       // Physical location of the center of the mesh
    std::vector<Boundary> boundaries; // std::vector of the bounding faces of this mesh
    std::vector<Cell> cells;          // std::vector of the cells contained in this mesh
    int nghosts;                      // Number of ghosts needed.  This will change 
                                      //  depth of cells outside boundary
    int meshID;                       // MPI ID of this Mesh

    Mesh(std::vector<double> icenter, int innghosts) : center(icenter), nghosts(innghosts) {
      meshID     = 0;
      cells      = std::vector<Cell>();
      boundaries = std::vector<Boundary>();
    }

    // Add a cell to the current mesh.  Return False if the cell is outside of boundary
    bool add_cell(Cell *item);

    // Remove a cell from the current mesh.  Return false if the cell is not valid
    bool remove_cell(Cell *item);
    bool remove_cell(int idx);

    // Sets new boundaries for the current mesh.  Returns False if new boundary fails
    //   sanity tests.  Will also update this->center.
    bool new_boundaries(std::vector<Boundary> newboundaries);

    // Retesselate the mesh and update cell quantities.
    bool tesselate();

    // Syncronizes mesh with other meshes on MPI topography.  This will exchange ghost zones if
    //   necessary
    bool sync_mesh();

    // Output routines
    void write(char *filename, char *mode);
    void write(FILE *fd);
};

