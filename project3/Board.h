#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Square.h"

class Board : public GeneralMV
{
 public:
  Board( vec3 location, float width, float thickness );
  virtual ~Board();

  void getWCBoundingBox( double* xyzLimitsF ) const;
  void render();

 private:
  std::vector<Square*> _squares;
};

#endif
