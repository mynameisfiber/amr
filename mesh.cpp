#include "mesh.hpp"
#include <cstdio>

using namespace std;

#define REAL double
#define TRILIBRARY
#define ANSI_DECLARATORS
extern "C" {
  #include "triangle/triangle.h"
}

Boundary::Boundary(Mesh *mesh, vector<double> instart, vector<double> inend) : start(instart), end(inend)
{
  double const signeddistance = ( ( end[0] - start[0])*(mesh->center[1] - start[1]) - 
                                  ( end[1] - start[1])*(mesh->center[0] - start[0])   );
  length = sqrt( (end[0] - start[0])*(end[0] - start[0]) +
                 (end[1] - start[1])*(end[1] - start[1]));
  distance    = fabs(signeddistance);
  orientation = (short int) (distance / signeddistance);
}

bool Mesh::add_cell(Cell *item)
{
  // We test that the center of the cell is on the correct side of the boundary by taking the cross product
  for( auto b = boundaries.begin(); b != boundaries.end(); b++ ) {
    if ( ( ( b->end[0] - b->start[0])*(item->x[1] - b->start[1]) - 
           ( b->end[1] - b->start[1])*(item->x[0] - b->start[0])   )  
         <  b->orientation
       )
    {
      return false;
    }
  }

  item->nodes.push_back(meshID);
  cells.push_back(*item);
  return true;
}

bool Mesh::remove_cell(int idx)
{
  if (idx > 0 && idx < cells.size()) {
    cells.erase(cells.begin() + idx);
    return true;
  }
  return false;
}

bool Mesh::remove_cell(Cell *item)
{
  for( auto c = cells.begin(); c != cells.end(); c++) {
    if (item->x[0] == c->x[0] && 
        item->x[1] == c->x[1]    )
    {
      cells.erase(c);
      return true;
    }
  }
  return false;
}


bool Mesh::tesselate()
{
  struct triangulateio in;
  struct triangulateio mid;
  struct triangulateio vorout;

  in.numberofpoints = cells.size();
  in.pointlist = (double *) malloc(in.numberofpoints * 2 * sizeof(double));
  in.numberofpointattributes = 0;
  in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));

  for (int i=0; i<in.numberofpoints; i++) {
    in.pointlist[2*i + 0] = (double)cells[i].x[0];
    in.pointlist[2*i + 1] = (double)cells[i].x[1];
    in.pointmarkerlist[i] = 0;

    cells[i].neighboors.clear();
    cells[i].faces.clear();
  }
  
  in.numberofsegments = 0;
  in.numberofholes = 0;
  in.numberofregions = 1;
  in.regionlist = (double *) malloc(in.numberofregions * 4 * sizeof(double));
  in.regionlist[0] = 0.5;
  in.regionlist[1] = 0.5;
  in.regionlist[2] = 7.0;           // Regional Attribute for whole mesh
  in.regionlist[3] = 0.1;           // Area Constraint

  mid.pointlist = (double *) NULL;            
  mid.pointmarkerlist = (int *) NULL; 
  mid.trianglelist = (int *) NULL;
  mid.triangleattributelist = (double *) NULL;
  mid.neighborlist = (int *) NULL;
  mid.segmentlist = (int *) NULL;
  mid.segmentmarkerlist = (int *) NULL;
  mid.edgelist = (int *) NULL;
  mid.edgemarkerlist = (int *) NULL;

  vorout.pointlist = (double *) NULL;
  vorout.edgelist = (int *) NULL;
  vorout.normlist = (double *) NULL;

/*
 * -c  Encloses the convex hull with segments.
 * -e  Generates an edge list.
 * -v  Generates a Voronoi diagram.
 * -z  Numbers all items starting from zero (rather than one).
 * -C  Check consistency of final mesh.
 * -Q  Quiet:  No terminal output except errors.
 *
 */
  triangulate ((char*)"QCczve", &in, &mid, &vorout );

  printf("mid.numberofpoints = %d\n",mid.numberofpoints);
  for(int i=0; i<mid.numberofedges; i++) {
    int x = mid.edgelist[2*i+0];
    int y = mid.edgelist[2*i+1];
    if (x < cells.size() && y < cells.size() && x >=0 && y >= 0) {
      cells[x].neighboors.push_back(Neighboor(&cells[x], &cells[y]));
      cells[y].neighboors.push_back(Neighboor(&cells[y], &cells[x]));


      const int startnode = vorout.edgelist[2*i+0];
      const int endnode   = vorout.edgelist[2*i+1];

      vector<double> vedgestart = {vorout.pointlist[2*startnode+0],
                                   vorout.pointlist[2*startnode+1]};
      vector<double> vedgeend;
      // We now handle the case of a boundary face.
      if (endnode != -1) {
        vedgeend   = {vorout.pointlist[2*endnode+0],
                      vorout.pointlist[2*endnode+1]};
      } else {
        vedgeend   = {vedgestart[0] + vorout.normlist[2*startnode+0],
                      vedgestart[1] + vorout.normlist[2*startnode+1]};
      }

      cells[x].faces.push_back(Face(&cells[x], vedgestart, vedgeend));
      cells[y].faces.push_back(Face(&cells[y], vedgestart, vedgeend));
    } 
  }

  printf("mid.numberofsegments = %d\n",mid.numberofsegments);
  boundaries.clear();
  for(int i=0; i<mid.numberofsegments; i++) {
    int x = mid.segmentlist[2*i+0];
    int y = mid.segmentlist[2*i+1];
    if (x < cells.size() && y < cells.size() && x >=0 && y >= 0) {
      boundaries.push_back(Boundary(this, {mid.pointlist[2*x+0], mid.pointlist[2*x+1]},
                                          {mid.pointlist[2*y+0], mid.pointlist[2*y+1]}));
    } 
  }


  free(in.pointlist);
  free(in.pointmarkerlist);
  free(in.regionlist);

  free(mid.pointlist);
  free(mid.pointmarkerlist);
  free(mid.trianglelist);
  free(mid.triangleattributelist);
  free(mid.neighborlist);
  free(mid.segmentlist);
  free(mid.segmentmarkerlist);
  free(mid.edgelist);
  free(mid.edgemarkerlist);

  free(vorout.pointlist);
  free(vorout.edgelist);
  free(vorout.normlist);


  return true;
}

void Mesh::write(FILE *fd)
{
  fprintf(fd, "%f %f ",center[0], center[1]);
  for( auto b = boundaries.begin(); b != boundaries.end(); b++) {
    fprintf(fd, "%f %f %f %f %d ",b->start[0], b->start[1], b->end[0], b->end[1], b->orientation);
  }
  fprintf(fd, "\n");

  for( auto cell = cells.begin(); cell != cells.end(); cell++)
  {
    cell->write(fd);
  }
}

void Mesh::write(char *filename, char *mode)
{
  FILE *fd = fopen(filename, mode);
  write(fd);
  fclose(fd);
}

int main()
{
  Mesh mesh({0.0, 0.0}, 0);

  for(int i=0; i<400; i++) {
    Cell cell( {2.0*rand()/RAND_MAX-1.0,
                2.0*rand()/RAND_MAX-1.0},
               1);
    cell.data[0] = rand()/10.0;
    mesh.add_cell(&cell);
  }

  printf("Mesh has %zu cells\n",mesh.cells.size());

  mesh.tesselate();

  FILE *fd = fopen("testcell.dat", "w+");
  mesh.write(fd);
  fclose(fd);

  return 0;
}
