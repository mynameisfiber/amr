#include "cell.hpp"

using namespace std;

Face::Face(Cell *incell, vector<double> instart, vector<double> inend) : start(instart), end(inend), cell(incell) {
  double const signeddistance = ( ( end[0] - start[0])*(cell->x[1] - start[1]) - 
                                  ( end[1] - start[1])*(cell->x[0] - start[0])   );
  length = sqrt( (end[0] - start[0])*(end[0] - start[0]) +
                 (end[1] - start[1])*(end[1] - start[1]));
  distance    = fabs(signeddistance);
  orientation = (short int) (distance / signeddistance);
}

Neighboor::Neighboor(Cell *me, Cell *them) : cell(them) {
  dx.push_back(me->x[0] - them->x[0]);
  dx.push_back(me->x[1] - them->x[1]);
  distance = sqrt(dx[0]*dx[0] + dx[1]*dx[1]);
}

void Cell::write(FILE *fd)
{
  fprintf(fd, "%f %f ",x[0], x[1]);
  
  for( auto b = faces.begin(); b != faces.end(); b++) {
    fprintf(fd, "%f %f %f %f %d ",b->start[0], b->start[1], b->end[0], b->end[1], b->orientation);
  }

  fprintf(fd, "\n");
}

void Cell::write(char *filename, char *mode)
{
  FILE *fd = fopen(filename, mode);
  write(fd);
  fclose(fd);
}
