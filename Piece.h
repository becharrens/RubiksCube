//
// Created by Benito Echarren on 28/06/2017.
//

#ifndef RUBIKSCUBE_PIECE_H
#define RUBIKSCUBE_PIECE_H

#include "Utils.h"

typedef enum colour {
    WHITE, GREEN, RED, YELLOW, BLUE, ORANGE, NO_COLOUR
} colour;

typedef enum plane {
    YZ, XZ, XY
} plane;

class Piece {
  public:
    Piece();
    void rotateColours(plane p);
    void setColour(int coord, colour c);
    void setColours(colour *colours);
    colour getColour(plane colourPlane);
    static void printColour(colour c);
  private:
    colour colours[3];
    static const int MAX_N_COLOURS = 3;
    void swapColours(int idx1, int idx2);

};


#endif //RUBIKSCUBE_PIECE_H
