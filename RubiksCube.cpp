//
// Created by Benito Echarren on 29/06/2017.
//

#include "RubiksCube.h"

/**
 * Rubik's cube constructor
 * Allocates memory for the cube and sets it to its solved state
 * @param side The number of pieces in a cube's edge
 */
RubiksCube::RubiksCube(int side) {
  //Allocate space for the cube
  this->side = side;
  cube = allocateCube(side);
  //TODO: Initialise to solved state
  initSolvedCube();
}

/**
 * Rotates the specified slice in the rotation plane of the cube by 90
 * degrees clockwise. All rotation slices are taken with the top left corner of
 * the cube as the origin
 * @param rotPlane The plane of the face/slice to rotate
 * @param slice The index of the slice being rotated
 * @param direction The direction in which to turn the face, either clockwise or
 * anti-clockwise
 */
void RubiksCube::rotateFace(plane rotPlane, int slice, rotationDir direction) {
  //A function pointer to the function which will set up the indices for the
  //swaps to carry out the rotation
  void (*getRotationIndices) (int **, plane, int, int, int);
  //An array which holds the coordinates of the four pieces to rotate on each
  //iteration
  int **swapIndices = alloc2DArray<int>(SIDES_IN_SQUARE, 3);
  //Fix the coordinate which defines the plane of the cube to rotate
  for (int i = 0; i < 4; ++i) {
    swapIndices[i][rotPlane] = slice;
  }
  //Assign the function pointer which will assign the correct values to the
  //indices
  switch (direction) {
    case CLOCKWISE:
      getRotationIndices = &getClockwiseRotationIndices;
      break;
    default:
      //ANTI_CLOCKWISE case
      getRotationIndices = &getAntiClockwiseRotationIndices;
  }
  //Iterates through the rows of the top half of the slice of the rubik's cube
  for (int i = 0; i < side / 2; ++i) {
    //Iterates through the columns of each inner layer of the cube's slice
    for (int j = i; j < side - i - 1; ++j) {
      //Assigns the indices of the four pieces which are to be rotated
      getRotationIndices(swapIndices, rotPlane, i, j, side);
      //Fix the orientation of the pieces which will be rotated
      for (int k = 0; k < SIDES_IN_SQUARE; k++) {
        cube[swapIndices[k][0]][swapIndices[k][1]][swapIndices[k][2]]
            .rotateColours(rotPlane);
      }
      //DEBUG
//      cout << "\n";
//      for (int l = 0; l < 4; ++l) {
//        for (int m = 0; m < 3; ++m) {
//          cout << swapIndices[l][m] << " ";
//        }
//        cout << "\n";
//      }
//      cout << "\n\n";
      //DEBUG
      //Performs the three swaps to rotate the 4 pieces
      swapPieces(swapIndices[0], swapIndices[3]);
      swapPieces(swapIndices[1], swapIndices[3]);
      swapPieces(swapIndices[2], swapIndices[0]);
    }
  }
  //Free the allocated array
  delete swapIndices[0];
  delete swapIndices;
}

/**
 * Assigns the indices of the four positions which the piece which is at
 * position (r, c) in the rotation plane can pass through when rotated. These
 * four pieces are the ones which will be rotated clockwise by 90 degrees.
 * @param indicesArr The array which will hold the indices of four pieces. It
 * should have four rows of three columns, each representing a 3D coordinate
 * @param rotPlane The plane parallel to the slice of the cube which will be
 * rotated. It is used to determine what index is fixed and has to be skipped
 * when assigning the values
 * @param r The index of the row of the piece which we want to rotate within the
 * slice of the cube
 * @param c The index of the column of the piece which we want to rotate within
 * the slice of the cube
 * @param side
 */
void RubiksCube::getClockwiseRotationIndices(int **indicesArr, plane rotPlane,
                                             int r, int c, int side) {
  assignCoord(indicesArr[0], rotPlane, r, c);
  assignCoord(indicesArr[1], rotPlane, c, side - r - 1);
  assignCoord(indicesArr[2], rotPlane, side - c - 1, r);
  assignCoord(indicesArr[3], rotPlane, side - r - 1, side - c - 1);
}

/**
 * Assigns the indices of the four positions which the piece which is at
 * position (r, c) in the rotation plane can pass through when rotated. These
 * four pieces are the ones which will be rotated anti-clockwise by 90 degrees.
 * @param indicesArr The array which will hold the indices of four pieces. It
 * should have four rows of three columns, each representing a 3D coordinate
 * @param rotPlane The plane parallel to the slice of the cube which will be
 * rotated. It is used to determine what index is fixed and has to be skipped
 * when assigning the values
 * @param r The index of the row of the piece which we want to rotate within the
 * slice of the cube
 * @param c The index of the column of the piece which we want to rotate within
 * the slice of the cube
 * @param side
 */
void RubiksCube::getAntiClockwiseRotationIndices(int **indicesArr,
                                                 plane rotPlane, int r, int c,
                                                 int side) {
  assignCoord(indicesArr[0], rotPlane, r, c);
  assignCoord(indicesArr[1], rotPlane, side - c - 1, r);
  assignCoord(indicesArr[2], rotPlane, c, side - r - 1);
  assignCoord(indicesArr[3], rotPlane, side - r - 1, side - c - 1);
}

/**
 * Allocates memory for a side x side x side cube. It creates side pointers
 * which will point to a different slice of the cube, and then assign each
 * pointer to an allocated slice
 * @param side the number of pieces in each of the cubes edges
 * @return a pointer to the cube represented as a piece 3D array
 */
Piece ***RubiksCube::allocateCube(int side) {
  //Allocates space for the face pointers
  Piece ***cube = new Piece **[side];
  for (int i = 0; i < side; ++i) {
    //Assign each face pointer to an allocated face/slice of the cube
    cube[i] = alloc2DArray<Piece>(side, side);
  }
  return cube;
}

void RubiksCube::swapPieces(int *coord1, int *coord2) {
  Piece tempPiece = cube[coord1[0]][coord1[1]][coord1[2]];
  cube[coord1[0]][coord1[1]][coord1[2]] = cube[coord2[0]][coord2[1]][coord2[2]];
  cube[coord2[0]][coord2[1]][coord2[2]] = tempPiece;
}

/**
 * Returns the number of pieces in a cube's edge
 * @return the number of pieces in a cube's edge
 */
int RubiksCube::getSide() {
  return side;
}

/**
 * Solves the Rubik's cube if possible
 * @return true if it was possible to solve the cube, false if it wasn't
 */
bool RubiksCube::solve() {
  //TODO
  return false;
}

/**
 * Initialises the cube to its solved state. It iterates through the coordinates
 * of a side x side array. Every coordinate is used to set a colour on each side
 * of the cube. An array of 6 coordinates is used to store the coordinates of
 * the six pieces whose colours will be set on each iteration. Each coordinate
 * has a fixed index which is determines the plane through which the function
 * iterates. The other coordinates are set on each iteration of the loop to
 * go through all the pieces in the face.
 */
void RubiksCube::initSolvedCube() {
  //Coordinate array
  int coord[6][3];
  //Fix indices
  coord[0][1] = 0;
  coord[1][2] = 0;
  coord[2][0] = side - 1;
  coord[3][1] = side - 1;
  coord[4][2] = side - 1;
  coord[5][0] = 0;
  //Iterate through the 6 different faces
  for (int k = 0; k < 6; ++k) {
    //Iterate through rows of face
    for (int i = 0; i < side; ++i) {
      //Iterate through columns of face
      for (int j = 0; j < side; ++j) {
        //Set the appropriate indices in the kth face to (i, j)
        assignCoord(coord[k], (k + 1) % 3, i, j);
        //Set the of in each piece in the face to its correct colour
        cube[coord[k][0]][coord[k][1]][coord[k][2]]
            .setColour((k + 1) % 3, (colour) k);
      }
    }
  }
}

/**
 * Prints the state of the Rubik's cube to Terminal. It prints the Rubik's cube
 * by iterating through each of its faces in turn in a simillar fashion to the
 * initialisation code: it has an array of coordinates, each of which has a
 * fixed index, and the other indices are reassigned to move through the
 * different indices of the face
 */
void RubiksCube::print() {
  //Coordinate array
  int coord[6][3];
  //Fix indices
  coord[0][1] = 0;
  coord[1][2] = 0;
  coord[2][0] = side - 1;
  coord[3][1] = side - 1;
  coord[4][2] = side - 1;
  coord[5][0] = 0;
  int iInc;
  int jInc;
  int rLBound;
  int rUBound;
  int cLBound;
  int cUBound;
  //Iterate through the 6 different faces
  for (int k = 0; k < 6; ++k) {
    printFaceName((faceName) k);

    //Iterate through rows of face
//    for (int i = 0; i < side; ++i) {
//      //Iterate through columns of face
//      for (int j = 0; j < side; ++j) {
//        //Set the appropriate indices in the kth face to (i, j)
//        assignCoord(coord[k], (k + 1) % 3, i, j);
//        //Set the of in each piece in the face to its correct colour
//        Piece::printColour(cube[coord[k][0]][coord[k][1]][coord[k][2]]
//            .getColour((plane) ((k + 1) % 3)));
//      }
//      cout << '\n';
//    }
    bool swapIndices = k % 3 == 1;
    iInc = (k == DOWN) ? -1 : 1;
    jInc = (k >= RIGHT) ? -1 : 1;
    for (int i = ((k == DOWN) ? side - 1 : 0);
         ((k == DOWN) ? (side - i - 1) : i) < side; i += iInc) {
      //Iterate through columns of face
      for (int j = ((k >= RIGHT) ? side - 1 : 0);
           ((k >= RIGHT) ? (side - j - 1) : j) < side; j += jInc) {
        //Set the appropriate indices in the kth face to (i, j)

        assignCoord(coord[k], (k + 1) % 3, (swapIndices) ? j : i, (swapIndices) ? i : j);
        //Set the of in each piece in the face to its correct colour
        Piece::printColour(cube[coord[k][0]][coord[k][1]][coord[k][2]]
                               .getColour((plane) ((k + 1) % 3)));
      }
      cout << '\n';
    }
    cout << '\n';
  }
}

/**
 * Converts the given enum parameter representing the name of a face to its
 * corresponding string and prints it out
 * @param face An enum representing the name of a face
 */
void RubiksCube::printFaceName(faceName face) {
  //Depending on what the input enum is it will print the corresponding string
  switch (face) {
    case UP: {
      cout << "UP:\n";
      break;
    }
    case LEFT: {
      cout << "LEFT:\n";
      break;
    }
    case FRONT: {
      cout << "FRONT:\n";
      break;
    }
    case DOWN: {
      cout << "DOWN:\n";
      break;
    }
    case RIGHT: {
      cout << "RIGHT:\n";
      break;
    }
    case BACK: {
      cout << "BACK:\n";
      break;
    }
    default:
      //Invalid case
      cout << "NULL:\n";
  }
}

void RubiksCube::move(plane rotPlane, int slice, rotationDir direction) {
  rotateFace(rotPlane, slice, direction);
}
