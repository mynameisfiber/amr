#include <stdlib.h>
#include <stdio.h>
#include "../triangle.h"

int main ( void )
{
  struct triangulateio in;
  struct triangulateio mid;
  struct triangulateio vorout;

  in.numberofpoints = 300;
  in.pointlist = (double *) malloc(in.numberofpoints * 2 * sizeof(double));
  in.numberofpointattributes = 1;
  in.pointattributelist = (double *) malloc(in.numberofpoints *
                                          in.numberofpointattributes *
                                          sizeof(double));
  in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));

  int i;
  srand(14053);
  for (i=0; i<in.numberofpoints; i++) {
    in.pointlist[2*i + 0] = 1.0 * rand() / RAND_MAX;
    in.pointlist[2*i + 1] = 1.0 * rand() / RAND_MAX;
    in.pointattributelist[i] = i;
    in.pointmarkerlist[i] = 0;
  }

  in.numberofsegments = 0;
  in.numberofholes = 0;
  in.numberofregions = 1;
  in.regionlist = (double *) malloc(in.numberofregions * 4 * sizeof(double));
  in.regionlist[0] = 0.5;
  in.regionlist[1] = 0.5;
  in.regionlist[2] = 7.0;           // Regional Attribute for whole mesh
  in.regionlist[3] = 0.1;           // Area Constraint

/* 
  Make necessary initializations so that Triangle can return a 
  triangulation in `mid' and a Voronoi diagram in `vorout'.  
*/
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
 */
  triangulate ( "aczvqQ", &in, &mid, &vorout );

  FILE *fd;

  fd = fopen("test.v.edges.dat","w+");
  for(i=0; i<vorout.numberofedges; i++)
    fprintf(fd,"%d %d %.6g %.6g\n",vorout.edgelist[2*i],vorout.edgelist[2*i+1], vorout.normlist[2*i], vorout.normlist[2*i+1]);
  fclose(fd);

  fd = fopen("test.v.points.dat","w+");
  for(i=0; i<vorout.numberofpoints; i++)
    fprintf(fd,"%.6g %.6g\n",vorout.pointlist[2*i],vorout.pointlist[2*i+1]);
  fclose(fd);

  fd = fopen("test.points.dat","w+");
  for(i=0; i<mid.numberofpoints; i++)
    fprintf(fd,"%.6g %.6g\n",mid.pointlist[2*i],mid.pointlist[2*i+1]);
  fclose(fd);

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

  return 0;
}
