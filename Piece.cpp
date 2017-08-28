#include <stdexcept>
#include <cstring>
#include "Piece.h"

using namespace std;

Piece::Piece() {
  //TODO: verify
  memset(colours, NO_COLOUR, MAX_N_COLOURS);
}

/**
 * PRE: colours parameter points to an array of at least 3 elements
 * Changes the oriention of the pieces' colours according to the plane on which
 * the rotation is taking place. The colour on the plane which is being rotated
 * remains constant. Taking the array of colours as a 3D vector with x, y, z
 * "coordinates", a rotation swaps the colours in the two components which
 * define the plane being rotated. The mapping of the rotations is as follows:
 * U/D -> Rotation of the XY plane. YZ (0) -> 1, 2
 * F/B -> Rotation of the XZ plane. XZ (1) -> 2, 0
 * R/L -> Rotation of the YZ plane. XY (2) -> 0, 1
 * @param p An enum representing the plane which is being rotated
 */
void Piece::rotateColours(plane p) {
  swapColours((p + 1) % MAX_N_COLOURS, (p + 2) % MAX_N_COLOURS);
}


/**
 * Given an array of colours, this function sets the object's colours to be the
 * same as the parameter's, by copying its contents.
 * @param colours the array of colours to be assigned
 */
void Piece::setColours(colour *colours) {
  memcpy(this->colours, colours, MAX_N_COLOURS * sizeof(colour));
}


/**
 * Sets a colour component to the given value provided that the component's
 * coordinate is within range
 * @param coord the component's index
 * @param c the colour the component will be set to
 */
void Piece::setColour(int coord, colour c) {
  if (isInRange(coord, 0, 3)) {
    colours[coord] = c;
  }
}

/**
 * Swaps two colours in the colours array of the piece, provided that the
 * indices are within bounds
 * @param idx1 - The index of the first element to be swapped
 * @param idx2 - The index of the second element to be swapped
 */
void Piece::swapColours(int idx1, int idx2) {
  //Checks if the indices are within bounds before performing the swapColours
  if (!(isInRange(idx1, 0, MAX_N_COLOURS) &&
      isInRange(idx2, 0, MAX_N_COLOURS))) {
    throw std::out_of_range("Trying to swapColours colour out of range");
  }
  //Swap the elements at the indices
  colour tempCol = colours[idx1];
  colours[idx1] = colours[idx2];
  colours[idx2] = tempCol;
}

/**
 * Returns the colour of the piece in a spec
 * @param idx
 * @return
 */
colour Piece::getColour(plane colourPlane) {
  return colours[colourPlane];
}

void Piece::printColour(colour c) {
  switch (c) {
    case ORANGE:
      cout << "O ";
      break;
    case RED:
      cout << "R ";
      break;
    case WHITE:
      cout << "W ";
      break;
    case YELLOW:
      cout << "Y ";
      break;
    case GREEN:
      cout << "G ";
      break;
    case BLUE:
      cout << "B ";
      break;
    default:
      cout << "X ";
  }
}
