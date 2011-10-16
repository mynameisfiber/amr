#include "mesh.hpp"
#include <cstdio>

bool Mesh::add_cell(Cell *item)
{
  // We test that the center of the cell is on the correct side of the boundary by taking the cross product
  for( auto b = boundaries.begin(); b != boundaries.end(); b++ ) {
    if ( ( ( (*b)->end[0] - (*b)->start[0])*(item->x[1] - (*b)->start[1]) - 
           ( (*b)->end[1] - (*b)->start[1])*(item->x[0] - (*b)->start[0])   )  
         <  (*b)->orientation
       )
    {
      return false;
    }
  }

  item->nodes.push_back(meshID);
  cells.push_back(item);
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
    if (item->x[0] == (*c)->x[0] && 
        item->x[1] == (*c)->x[1]    )
    {
      cells.erase(c);
      return true;
    }
  }
  return false;
}
