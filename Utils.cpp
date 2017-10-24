//
// Created by Benito Echarren on 17/07/2017.
//
#include <cstring>
#include "Utils.h"

/**
 * Checks if a given value is within a range, that is, if it is greater than or
 * equal to the lower bound and strictly less than the upper bound
 * @param val - The value to check if it is within the given range
 * @param lBound -  The lower bound of the range
 * @param uBound - The upper bound of the range
 * @return Whether the value is greater than or equal to the lower bound and
 * strictly less than the upper bound
 */
bool isInRange(int val, int lBound, int uBound) {
  return (val >= lBound && val < uBound);
}

/**
 * Writes 3 given values into 3D coordinate such that one of the first and
 * second values have to be in relative order, but out of the three indices,
 * one of them (skipIdx) must have a certain value assigned, so the other two
 * values must be placed taking that into account. Cases:
 * skipIdx | firstIdx | secondIdx
 * --------+----------+----------
 *    0    |     1    |     2
 *    1    |     0    |     2
 *    2    |     0    |     1
 * @param coord A pointer to an array with space for at least 3 elements
 * @param skipIdx The index which the function will skip. It should be 0, 1 or 2
 * @param firstVal The first value to be assigned
 * @param secondVal The second value to be assigned
 */
void assignCoord(int *coord, int skipIdx, int firstVal, int secondVal) {
  coord[(skipIdx == 0) ? 1 : 0] = firstVal;
  coord[(skipIdx == 2) ? 1 : 2] = secondVal;
}

/**
 * Given a string and a non-negative integer, it returns the string resulting
 * from concatenating the string with itself as many time as specified (repeat
 * the string as many times as required)
 * @param s The string to repeat
 * @param times The number of times the string is to be repeated
 * @return The string resulting from repeating s times times
 */
string repeat(string s, int times) {
  string result;
  for (int i = 0; i < times; i++) {
    result += s;
  }
  return result;
}

/**
 * Given a valid move sequence, it returns an equivalent move sequence where all
 * the bracketed sequences have been expanded, so it is only a sequence moves,
 * with no brackets
 * @param moves A pointer to the move string
 * @param disp A reference to the displacement with respect to the first
 * character of the string. After the function call, the displacement will point
 * to the first character after the current sequence has ended
 * @return The equivalent move string to the given one, with all the brackets
 * expanded and removed
 */
string expandMoveSequence(const char *moves, int &disp) {
  string moveSeq;
  while (disp < strlen(moves)) {
    if (*(moves + disp) == '(') {
      disp++;
      moveSeq += expandMoveSequence(moves, disp);
    } else if (*(moves + disp) == ')') {
      disp++;
      char *savePtr;
      int nRepetitions = (int) strtol(moves + disp, &savePtr, 10);
      disp = (int) (savePtr - moves);
      string repeatedStr;
      for (int i = 0; i < nRepetitions; ++i) {
        repeatedStr += moveSeq;
        repeatedStr += ' ';
      }
      return repeatedStr;
    } else {
      if (!(*(moves + disp) == ' ' && moveSeq[moveSeq.length() - 1] == ' ')) {
        moveSeq += *(moves + disp);
      }
      disp++;
    }
  }
  return moveSeq;
}