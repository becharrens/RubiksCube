//
// Created by Benito Echarren on 17/07/2017.
//

#ifndef RUBIKSCUBE_UTILS_H
#define RUBIKSCUBE_UTILS_H

#include <iostream>

using namespace std;

bool isInRange(int val, int uBound, int lBound);

/**
 * Allocates memory for a 2D array of a given type and the specified dimensions
 * @tparam T The type of the elements in the array
 * @param rows The number of rows in the array
 * @param cols The number of columns in the array
 * @return A pointer to a 2D array of the specified type and dimensions
 */
template <class T>
T **alloc2DArray(int rows, int cols) {
  //Allocates space for each row pointer
  T **arr = new T *[rows];
  //Allocate space for all the elements and assign the pointer to the first
  //pointer
  arr[0] = new T[rows * cols];
  for (int i = 1; i < rows; ++i) {
    //Make each subsequent pointer an offset of the number of columns with
    //respect to the previous one
    arr[i] = arr[i - 1] + cols;
  }
  return arr;
}

void assignCoord(int *coord, int skipIdx, int firstVal, int secondVal);

#endif //RUBIKSCUBE_UTILS_H
