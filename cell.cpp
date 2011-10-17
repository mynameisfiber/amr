#include "cell.hpp"

Face::Face(Cell *incell, vector<double> instart, vector<double> inend) : start(instart), end(inend), cell(incell) {
  length = sqrt( (end[0] - start[0])*(end[0] - start[0]) +
                 (end[1] - start[1])*(end[1] - start[1]));
  orientation = ( ( end[0] - start[0])*(cell->x[1] - start[1]) - 
                  ( end[1] - start[1])*(cell->x[0] - start[0])   );
}

Neighboor::Neighboor(Cell *me, Cell *them) : cell(them) {
  dx.push_back(me->x[0] - them->x[0]);
  dx.push_back(me->x[1] - them->x[1]);
  distance = sqrt(dx[0]*dx[0] + dx[1]*dx[1]);
}
