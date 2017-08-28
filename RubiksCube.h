//
// Created by Benito Echarren on 29/06/2017.
//

#ifndef RUBIKSCUBE_RUBIKSCUBE_H
#define RUBIKSCUBE_RUBIKSCUBE_H

#include <regex>

#include "Piece.h"
#include "LinkedList.h"
#include "Utils.h"

#define SIDES_IN_SQUARE 4

typedef enum roationDir {
    CLOCKWISE, ANTI_CLOCKWISE
} rotationDir;

typedef enum moveName {
    U, L, F, D, R, B, INVALID
} moveName;

typedef enum faceName {
    UP, LEFT, FRONT, DOWN, RIGHT, BACK, VOID
} faceName;

typedef struct rubiksMove {
    bool isHalfTurn;
    rotationDir dir;
    plane rotationPlane;
    int slice;
    rubiksMove(rotationDir dir1, plane rotPlane, int cubeSlice) {
      rubiksMove(false, dir1, rotPlane, cubeSlice);
    }
    rubiksMove(bool halfTurn, rotationDir dir1, plane rotPlane, int cubeSlice) :
        isHalfTurn(halfTurn), dir(dir1), rotationPlane(rotPlane),
        slice(cubeSlice) {}
} rubiksMove;

/**
 * A Rubik's cube implementation where the cube is represented as 3D,
 * side x side x side array of pieces, where the origin is taken to be the top
 * left corner of the cube and the axis of the cube go as follows:
 *          O ------> z
 *          |\
 *          | \
 *          |  v
 *          v   x
 *          y
 * In this program, references to coordinates are taken to be a vector with
 * three components (x, y, z), which refer to the axis shown above. Pieces in
 * the cube have an orientation represented by a coordinate, although the
 * significance of each index is slightly different. The colour at each
 * index is the colour which is present at the plane formed by the two axis
 * excluding the one represented by that index.
 */
class RubiksCube {
  public:
    RubiksCube(int side);

    bool solve();
    void move(plane rotPlane, int slice, rotationDir direction);
    int getSide();
    void print();
  private:
    bool isValidSlice(int slice, moveName move);
    static moveName charToMove(char move);
    bool validateMoveSequence(const char *moves);
    bool applyMoveSequence(const char *moves);
    rubiksMove *parseMove(const char *moves, int &disp);
    void parseRepeatedSequence(const char *moves, int &disp);
    int side;
    Piece ***cube;
    void rotateFace(plane rotPlane, int slice, rotationDir direction);
    Piece ***allocateCube(int side);
    void swapPieces(int *coord1, int *coord2);
    void initSolvedCube();
    void getRotationIndices(int **indicesArr, plane rotPlane, int slice,
                            rotationDir direction, int r, int c);
    static void printFaceName(faceName face);
//    Piece **allocateFace(int side);
};


#endif //RUBIKSCUBE_RUBIKSCUBE_H
