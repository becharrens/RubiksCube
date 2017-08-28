//
// Created by Benito Echarren on 17/07/2017.
//

#include "Utils.h"

using namespace std;

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
 * This function skips all the spaces starting from the element the iterator
 * points to and returns an iterator pointing to the first non-space character
 * in the string or, if there isn't one, s.end()
 * @param s the string which is being iterated through
 * @param iter the iterator pointing to the starting element of the iteration
 * @return An iterator to the first non-space character in the string or s.end()
 * if there are none
 */
string::iterator skipSpaces(string s, string::iterator iter) {
  while (isspace(*iter) && iter < s.end()) {
    iter++;
  }
  return iter;
}