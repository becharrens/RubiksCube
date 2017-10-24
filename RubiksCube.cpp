#include "RubiksCube.h"

/**
 * Rubik's cube constructor
 * Allocates memory for the cube and sets it to its solved state
 * @param side The number of pieces in a cube's edge
 */
RubiksCube::RubiksCube(int side) {
  //Allocate space for the cube
  this->side = side;
  cube = alloc3DArray<Piece>(side, side, side);
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
  if (!isInRange(slice, 0, side)) {
    throw out_of_range("Trying to rotate a slice out of bounds");
  }

  //An array which holds the coordinates of the four pieces to rotate on each
  //iteration
  int **swapIndices = alloc2DArray<int>(SIDES_IN_SQUARE, 3);

  //Fix the coordinate which defines the plane of the cube to rotate
  for (int i = 0; i < 4; ++i) {
    swapIndices[i][rotPlane] = slice;
  }

  //Iterates through the rows of the top half of the slice of the rubik's cube
  for (int i = 0; i < side / 2; ++i) {
    //Iterates through the columns of each inner layer of the cube's slice
    for (int j = i; j < side - i - 1; ++j) {
      //Assigns the indices of the four pieces which are to be rotated
      getRotationIndices(swapIndices, rotPlane, slice, direction, i, j);
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
  //Iterate through the 6 different faces
  for (int k = 0; k < 6; ++k) {
    printFaceName((faceName) k);
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

/**
 * Public interface for the rotateFace method. It simply calls the private
 * method with the same arguments. It moves the given slice of the cube in the
 * desired direction
 * @param rotPlane The plane which the desired cube slice lies
 * @param slice The index of the slice to be turned in the cube
 * @param direction The direction of the turn: Clockwise or Anti-clockwise
 */
void RubiksCube::move(plane rotPlane, int slice, rotationDir direction) {
  rotateFace(rotPlane, slice, direction);
}

/**
 * Writes the rotation indices for the four pieces corresponding to the (r, c)
 * coordinate of the face in the cube to be rotated in the desired direction.
 * Turning a face clockwise/anti-clockwise in a cube means turning it in the
 * direction which would be clockwise/anti-clockwise when looking at it from the
 * front, like in the standard rubik's cube moves. However, for the inner layers
 * of the cube, the turning direction is the same as the outermost face in the
 * same plane which is closest to it. In case of doubt (when the distant to
 * either one is the same), the face will be rotated in the same direction as
 * the face which has the highest slice index, that is, the outer face to which
 * the top left back corner doesn't belong.
 * The indices themselves are the same for all rotations, it is just the order
 * which changes. In fact, it is the order of the second and third indices which
 * determine the direction of the turn. However, because the direction of the
 * rotation depends on the user's viewpoint and not the coordinates with respect
 * to the cube's coordinate's origin, the indices get muddled, and a non-obvious
 * logical condition is required to determine which of the two orderings pick in
 * order to turn each face in the desired direction.
 * Brief explanation of the condition for the outer faces (it extends naturally
 * to the inner layers):
 * Ordering needed to turn each face clockwise
 * -----------------------------------
 * | Face | Plane | Slice | Ordering |
 * -----------------------------------
 * |  B   |   YZ  |   0   |   Anti   |
 * |  D   |   XZ  |   2   |   Anti   |
 * |  L   |   XY  |   0   |   Anti   |
 * |  F   |   YZ  |   2   | Clockwise|
 * |  U   |   XZ  |   0   | Clockwise|
 * |  R   |   XY  |   2   | Clockwise|
 * -----------------------------------
 * Grouped by ordering, you can notice that only the ones with plane XZ differ
 * in the slice. Taking the booleans:
 *  - Is the plane XZ? as XZ
 *  - Is the slice < side / 2? as SLICE
 *  - Is the direction clockwise? as CLOCK
 * Hence you can find the condition for a clockwise rotation as:
 * (XZ && SLICE && CLOCK) || (!XZ && !SLICE && CLOCK) ||
 * (XZ && !SLICE && !CLOCK) || (!XZ && SLICE && !CLOCK)
 *
 * (CLOCK && ((XZ && SLICE) || (!XZ && !SLICE))) ||
 * (!CLOCK && ((XZ && !SLICE) || (!XZ && SLICE)))
 *
 * (CLOCK && !XOR(XZ, SLICE)) || (!CLOCK && XOR(XZ, SLICE)))
 *
 * XOR(CLOCK, XOR(XZ, SLICE))
 *
 * @param indicesArr The int** which points to the 2D-array to which the indices
 * will be written. It should have space for at least 4 rows of 3 columns each
 * @param rotPlane The plane in which the rotation will be carried out
 * @param slice The slice of the cube to be rotated
 * @param direction The direction for the rotation: CLOCKWISE/ ANTI_CLOCKWISE
 * @param r The row index within the slice of the cube of the reference element
 * of the four to be swapped
 * @param c The column index within the slice of the cube of the reference
 * element of the four to be swapped
 */
void RubiksCube::getRotationIndices(int **indicesArr, plane rotPlane, int slice,
                                    rotationDir direction, int r, int c) {
  //Assign the first and last coordinates, which are common to both cases
  assignCoord(indicesArr[0], rotPlane, r, c);
  assignCoord(indicesArr[3], rotPlane, side - r - 1, side - c - 1);
  //Get the three booleans for the condition
  bool isClockwise = direction == CLOCKWISE;
  bool sliceInFirstHalf = slice < side / 2;
  bool isPlaneXZ = rotPlane == XZ;
  if (XOR(isClockwise, (XOR(sliceInFirstHalf, isPlaneXZ)))) {
    //Clockwise rotation assignments
    //DEBUG
//    cout << "0 1 2 3" << endl;
    //DEBUG
    assignCoord(indicesArr[1], rotPlane, c, side - r - 1);
    assignCoord(indicesArr[2], rotPlane, side - c - 1, r);
  } else {
    //Anti-clockwise rotation assignments
    //DEBUG
//    cout << "0 2 1 3" << endl;
    //DEBUG
    assignCoord(indicesArr[1], rotPlane, side - c - 1, r);
    assignCoord(indicesArr[2], rotPlane, c, side - r - 1);
  }
}

/**
 * Checks if the number for a given slice is correct or not. It verifies that
 * The slice is non negative and refers to a slice in the half of the cube
 * which the face represented in the move enum belongs to. If there is a middle
 * slice, it will be referenced only by the faces F, D or R
 * @param slice The index of the slice referenced by the move
 * @param move The move enum representing the face referenced by the move
 * @return true if the slice is valid or false if it isn't
 */
bool RubiksCube::isValidSlice(int slice, moveName move) {
  return (isInRange(slice, 0, side / 2) ||
         (((move + 1) / 3 == 1) && slice == side / 2));
}

/**
 * Returns either the move enum corresponding to the character or an INVALID
 * enum
 * @param move The character representing the enum
 * @return A move enum corresponding to the character or an INVALID move enum
 * if the character doesn't represent a valid move
 */
moveName RubiksCube::charToMove(char move) {
  switch (move) {
    case 'U':
    case 'u':
      return U;
    case 'L':
    case 'l':
      return L;
    case 'F':
    case 'f':
      return F;
    case 'D':
    case 'd':
      return D;
    case 'R':
    case 'r':
      return R;
    case 'B':
    case 'b':
      return B;
    default:
      //Invalid move
      return INVALID;
  }
}

/**
 * Given a move sequence represented as a char array (c-style string), it goes
 * through the string, verifying that all the moves, if any, in it are valid,
 * and that the syntax is correct. It uses regex to identify the different parts
 * of the syntax.
 * The syntax for moves is as follows:
 *  - An individual move is a letter: ULFDRB, representing a 90 degrees move of
 *  a face clockwise if it is uppercase or anti-clockwise if it is lowercase
 *  - Optionally, for a 180 degrees turn, a '2' may be added after the letter,
 *  as in F2, which would move the front face 180 degrees
 *  - Movements of inner layers of the cube are designated by adding a number
 *  before the letter, which represents how many layers into the cube the face
 *  you want to turn is. For example: 2F2. Slices can only be referenced through
 *  the closest outer face parallel to the slice. If both sides are equally
 *  close, then it will be referenced by faces F/D/R
 *  - Repeated move sequences can be grouped through the use of brackets (),
 *  with the number of times the sequence should be repeated indicated after
 *  that
 *  - A bracketed move sequence starts either at the beginning of the string,
 *  after a space or after another bracket, with the first move(s) after the
 *  opening brace starting immediately after the opening bracket
 *  - The closing bracket should come immediately after the last move in the
 *  sequence, and the number of repetitions > 1 should come straight after that
 *  - Different moves and move sequences should be separated by spaces (ideally
 *  one)
 *  Example of a move sequence: "l (U2 (U 2d D 2R2)2 U2)2 R B"
 * @param moves A character pointer to a string holding the moves which need to
 * be verified
 * @return true if the move sequence is valid, false otherwise
 */
bool RubiksCube::validateMoveSequence(const char *moves) {
  int nOpenBraces = 0;
  int nRepetitions;
  regex invalidChar("([^(ULFDRB0-9\\(\\))]).*", regex_constants::icase);
  regex moveRegex("([1-9]([0-9]*)?)?[ULFDRB][2]?( |$|((\\)[1-9]([0-9]*)?)+)).*",
                  regex_constants::icase);
  regex afterMoveRegex("( |$|\\)).*");
  int slice;
  char *savePtr;
  for (int i = 0; i < strlen(moves); i++) {
    if (!isspace(*(moves + i))) {
      //If invalid character then the whole move sequence is invalid
      if (regex_match((moves + i), invalidChar)) {
        return false;
      }
      //Take into account all consecutive opening braces, keeping track of them
      //as it skips over them
      while (*(moves + i) == '(') {
        nOpenBraces++;
        i++;
      }

      //If the string matches the move regex then skip the the move and update
      //the iterator (i)
      if (regex_match((moves + i), moveRegex)) {
        //The previous match guarantees that there is a valid move, followed
        //a space, end of line character or any number of nested closing
        //brackets followed by numbers indicating how many times the moves
        //inside the brackets should be repeated. Hence the move finishes when
        //the iteration meets a space or EOL character (the closing brace(s) is
        //taken to be part of the move). The loop terminates at a non-move
        //character, which means that increasing the iterator won't skip over an
        //important character

        slice = (int) strtol(moves + i, &savePtr, 10);
        //If a slice has been assigned with value <= 1 with the pointer having
        //moved, it means that the move was invalid because the leading number
        //was invalid
        if ((slice <= 1 && savePtr >  moves + i)) {
          return false;
        }
        //If the pointer didn't move, it means that the slice to move is an
        //outer slice, it is already 0. Otherwise, it is one more than the index
        //of the slice referenced
        if (slice != 0) {
          slice--;
        }
        //Update the offset so that the pointer will point to the next character
        i = (int) (savePtr - moves);
        //Check if the slice referenced is within the bounds for the given face
        if (!isValidSlice(slice, charToMove(*(moves + i)))) {
          return false;
        }

        //Skip the move letter and the optional 2 after it. After the move there
        //can only be three characters: EOF, ' ' or ')'. Skip all the characters
        //until you get to it.
        while (!regex_match((moves + i), afterMoveRegex)) {
          i++;
        }

        //If the character after the move is a closing brace, verify the number
        //of closing and opening braces and check the validity of the number of
        //repetitions
        if (*(moves + i) == ')') {
          nOpenBraces--;
          if (nOpenBraces < 0) {
            //Too many closing braces
            return false;
          }
          //Parse number of repetitions. Add extra 1 because *(moves + i) points
          //to the closing brace
          nRepetitions = (int) strtol(moves + i + 1, &savePtr, 10);
          if (nRepetitions <= 1) {
            return false;
          }
          //update the displacement
          i = (int) (savePtr - moves);
          break;
        }
      } else {
        //Invalid move syntax
        return false;
      }
    }
  }
  //Check if too many opening braces
  return (nOpenBraces <= 0);
}

/**
 * Given a move sequence, it tries to apply all the moves in order. If the
 * sequence has a syntax error, the function fails to apply any moves. The
 * function reports the success or failure of applying the moves. The move
 * sequence is first expanded before iterating through it an applying the moves
 * in order
 * @param moves The string containing the moves to be applied
 * @return true if the function has successfully applied the function, false
 * otherwise
 */
bool RubiksCube::applyMoveSequence(const char *moves) {
  //If the move sequence is invalid exit with failure
  if (!validateMoveSequence(moves)) {
    return false;
  }

  int disp = 0;
  int &dispRef = disp;
  //Expand all the repeated moves
  string expandedMoves = expandMoveSequence(moves, dispRef);

  //Convert the resulting string to a cstring
  const char *expandedMovesCStr = expandedMoves.c_str();
  disp = 0;
  rubiksMove *nextMove;

  //Iterate through the moves, applying each one in turn
  while (disp < strlen(expandedMovesCStr)) {
    if (!isspace(*(expandedMovesCStr + disp))) {
      nextMove = parseMove(expandedMovesCStr, dispRef);
      move(nextMove->rotationPlane, nextMove->slice, nextMove->dir);
      if (nextMove->isHalfTurn) {
        move(nextMove->rotationPlane, nextMove->slice, nextMove->dir);
      }
      print();
      delete nextMove;
    }
    disp++;
  }
  return true;
}

/**
 * PRE: The the the pointer (moves + disp) points to the beginning of a valid
 * move.
 * If the precondition is met, this function parses the string starting at
 * disp from the start and breaks down the move into its different components,
 * which are stored in a struct which is returned (a pointer to it)
 * @param moves The string were all the moves are stored.
 * @param disp The displacement from the beginning of the string, which should
 * point to the first character which belongs to the move. When the function
 * terminates, disp will be the offset from the beginning such that it will
 * point to the first character in the string which isn't part of the move.
 * @return A pointer to the struct containing all the different parts of
 * move
 */
rubiksMove *RubiksCube::parseMove(const char *moves, int &disp) {
  int sliceDisp;
  int slice;
  bool isHalfTurn;
  moveName movName;
  rotationDir direction;

  //Pointer to move letter in the string, the character which will be after the
  //number (if any) in the move string
  char *savePtr;
  //Parse the slice displacement
  sliceDisp = (int) strtol(moves + disp, &savePtr, 10);
  if (sliceDisp >= 1) {
    //Displacement will be 1 more than the index of the slice
    sliceDisp--;
  }
  //Update the displacement
  disp = (int) (savePtr - moves);
  //Upper case letters indicate clockwise rotations, lower case letters indicate
  //anti-clockwise rotations
  direction = (isupper(*(moves + disp))) ? CLOCKWISE : ANTI_CLOCKWISE;
  //Parse the face indicating the movement
  movName = charToMove(*(moves + disp));
  disp++;
  //If there is a 2 after the letter, then the rotation is 180º
  isHalfTurn = *(moves + disp) == '2';
  if (isHalfTurn) {
    disp++;
  }
  //Use the face of the movement to get the rotation plane and decide how to
  //modify the slice index with the slice displacement
  /*
   *  Move | Plane
   * ------+-------
   *    U  |  XZ(1)
   *    L  |  XY(2)
   *    F  |  YZ(0)
   *    D  |  XZ(1)
   *    R  |  XY(2)
   *    B  |  YZ(0)
   */
  plane rotPlane = (plane) ((movName + 1) % 3);
  /*
   *                  | U | L |    F     |    D     |    R     | B
   * -----------------+---+---+----------+----------+----------+---
   *       enum       | 0 | 1 |    2     |    3     |    4     | 5
   *  starting index  | 0 | 0 | side - 1 | side - 1 | side - 1 | 0
   * +/- displacement | + | + |    -     |    -     |    -     | +
   */
  if ((movName + 1) / 3 == 1) {
    slice = side - 1 - sliceDisp;
  } else {
    slice = sliceDisp;
  }
  rubiksMove *move = new rubiksMove(isHalfTurn, direction, rotPlane, slice);
  return move;
}

bool RubiksCube::validateMoveSequenceTests() {
  const char arrMoves[][30] =
      {"((((2F2)3)4)5)6",           //Valid: Nested Brackets
       "(2F) R2 2d",                //Invalid: Brackets without number
       "((F r2)3 L2)2 U)4",         //Invalid: Too many closing brackets
       "(F (R2 (F (20b2 D)3 D)4 R", //Invalid: Too many opening brackets
       "(R2 2F)3 (R U2)300 l2",     //Valid: Brackets and moves
       "(D2 G2 R2 L u d)",          //Invalid: Invalid moves
       "D3 d r2 b3",                //Invalid: Non-2 number after move
       "2(R2 L2 2R2 U2)2 D L U2"};  //Invalid: Number before opening braces
  return validateMoveSequence(arrMoves[0]) &&
      !validateMoveSequence(arrMoves[1]) &&
      !validateMoveSequence(arrMoves[2]) &&
      !validateMoveSequence(arrMoves[3]) &&
      validateMoveSequence(arrMoves[4]) &&
      !validateMoveSequence(arrMoves[5]) &&
      !validateMoveSequence(arrMoves[6]) &&
      !validateMoveSequence(arrMoves[7]);
}