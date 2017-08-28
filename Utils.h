//
// Created by Benito Echarren on 17/07/2017.
//

#ifndef RUBIKSCUBE_UTILS_H
#define RUBIKSCUBE_UTILS_H

#include <iostream>

#define XOR(a, b) (a && (!b)) || ((!a) && b)

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

/**
 * Allocates a 3D array using only 3 memory allocations, one per dimension, and
 * returns a triple pointer to the array of elements of the specified type
 * @tparam T The type of the elements in the array
 * @param depth The number of faces in the array
 * @param height The number of rows in each face
 * @param width The number of columns in each face (the number of elements in
 * each row of the face)
 * @return
 */
template <class T>
T ***alloc3DArray(int depth, int height, int width) {
  //Allocates space for each face pointer
  T ***arr = new T **[depth];
  //Allocates space for the pointers to the row pointers in each face
  arr[0] = new T *[depth * height];
  //Allocate space for all the elements and assign the pointer to the first
  //pointer
  arr[0][0] = new T[depth * height * width];

  for (int i = 1; i < depth; ++i) {
    //Assigns each face pointer to the pointer to the first row of the face.
    //This is done by offsetting the previous face pointer by the number of row
    //pointers in that face, which is height
    arr[i] = arr[i - 1] + height;
    //Assigns the pointer to the first row of each face to the pointer to the
    //correct element. This is done by offsetting the previous first element
    //pointer by the number of elements in a face, (height * width)
    arr[i][0] = arr[i - 1][0] + (height * width);
  }
  for (int i = 0; i < depth; ++i) {
    for (int j = 1; j < height; ++j) {
      //Assigns each subsequent face row pointer to the pointer to the first
      //element in the next row, which is an offset of the number of elements
      //in each row with respect to the previous row pointer
      arr[i][j] = arr[i][j - 1] + width;
    }
  }
  return arr;
}

void assignCoord(int *coord, int skipIdx, int firstVal, int secondVal);

string::iterator skipSpaces(string s, string::iterator iter);

#endif //RUBIKSCUBE_UTILS_H
