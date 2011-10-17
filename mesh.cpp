#include "mesh.hpp"
#include <cstdio>

#define REAL double
#define TRILIBRARY
#define ANSI_DECLARATORS
extern "C" {
  #include "triangle/triangle.h"
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
  in.numberofpointattributes = 1;
  in.pointattributelist = (double *) malloc(in.numberofpoints *
                                          in.numberofpointattributes *
                                          sizeof(double));
  in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));

  for (int i=0; i<in.numberofpoints; i++) {
    in.pointlist[2*i + 0] = (double)cells[i].x[0];
    in.pointlist[2*i + 1] = (double)cells[i].x[1];
    in.pointmarkerlist[i] = 0;
    in.pointattributelist[i] = i;

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
  mid.pointattributelist = (double *) NULL;
  mid.pointmarkerlist = (int *) NULL; 
  mid.trianglelist = (int *) NULL;
  mid.triangleattributelist = (double *) NULL;
  mid.neighborlist = (int *) NULL;
  mid.segmentlist = (int *) NULL;
  mid.segmentmarkerlist = (int *) NULL;
  mid.edgelist = (int *) NULL;
  mid.edgemarkerlist = (int *) NULL;

  vorout.pointlist = (double *) NULL;
  vorout.pointattributelist = (double *) NULL;
  vorout.edgelist = (int *) NULL;
  vorout.normlist = (double *) NULL;

/*
 *  -q  Quality mesh generation.  A minimum angle may be specified.
 *  -a  Applies a maximum triangle area constraint.
 *  -c  Encloses the convex hull with segments.
 *  -v  Generates a Voronoi diagram.
 *  -z  Numbers all items starting from zero (rather than one).
 *  -Q  Quiet:  No terminal output except errors.
 *  -e  Generates an edge list.
 */
  triangulate ((char*)"Qaczvqe", &in, &mid, &vorout );

  for(int i=0; i<mid.numberofedges; i++) {
    int x = mid.edgelist[i+0];
    int y = mid.edgelist[i+1];
    if (x < cells.size() && y < cells.size()) {
      cells[x].neighboors.push_back(Neighboor(&cells[x], &cells[y]));
      cells[y].neighboors.push_back(Neighboor(&cells[y], &cells[x]));

      vector<double> vedgestart = {vorout.pointlist[vorout.edgelist[i+0]+0],vorout.pointlist[vorout.edgelist[i+0]+1]};
      vector<double> vedgeend   = {vorout.pointlist[vorout.edgelist[i+1]+0],vorout.pointlist[vorout.edgelist[i+1]+1]};

      cells[x].faces.push_back(Face(&cells[x], vedgestart, vedgeend));
      cells[y].faces.push_back(Face(&cells[y], vedgestart, vedgeend));
    }

  }

  free(in.pointlist);
  free(in.pointattributelist);
  free(in.pointmarkerlist);
  free(in.regionlist);

  free(mid.pointlist);
  free(mid.pointattributelist);
  free(mid.pointmarkerlist);
  free(mid.trianglelist);
  free(mid.triangleattributelist);
  free(mid.neighborlist);
  free(mid.segmentlist);
  free(mid.segmentmarkerlist);
  free(mid.edgelist);
  free(mid.edgemarkerlist);

  free(vorout.pointlist);
  free(vorout.pointattributelist);
  free(vorout.edgelist);
  free(vorout.normlist);


  return true;
}

int main()
{
  Mesh mesh({0.0, 0.0}, 0);

  for(int i=0; i<200; i++) {
    Cell cell( {2.0*rand()/RAND_MAX-1.0,
                2.0*rand()/RAND_MAX-1.0},
               1);
    cell.data[0] = rand()/10.0;
    mesh.add_cell(&cell);
  }

  printf("Mesh has %zu cells\n",mesh.cells.size());

  mesh.tesselate();

  return 0;
}
