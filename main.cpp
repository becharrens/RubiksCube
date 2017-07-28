#include <iostream>
#include "RubiksCube.h"

using namespace std;
#define side 3
#define length 4
#define map(x) (x * x) % 3
#define map2(x) 1 + map((x + 1) % 3)

#include "LinkedListTests.h"
#include "SwapIndicesTuple.h"

string mapIdxToCoords(int idx) {
  return to_string(idx / side) + ", " + to_string(idx % side);
}

void swap(int *arr, int idx1, int idx2) {
  int tempCol = arr[idx1];
  arr[idx1] = arr[idx2];
  arr[idx2] = tempCol;
}

bool match(int *arr, int *target, int len) {
  for (int i = 0; i < len; i++) {
    if (arr[i] != target[i]) {
      return false;
    }
  }
  return true;
}

int **allocFace(int s) {
  int **face = new int *[s];
  face[0] = new int[s * s];
  for (int i = 1; i < s; ++i) {
    face[i] = face[i - 1] + s;
  }
  return face;
}

int ***allocCube(int s) {
  int ***cube = new int **[s];
  for (int i = 0; i < s; ++i) {
    cube[i] = allocFace(s);
  }
  return cube;
}

void printSol(string sol[], int len) {
  for (int i = 0; i < len; i++) {
    std::cout << sol[i] << std::endl;
  }
}

void printArr(int *arr) {
  for (int i = 0; i < length; ++i) {
    cout << arr[i];
  }
  cout << endl;
}

bool getSol(int *arr, int *target, string *solArr, int swaps) {
  if (match(arr, target, length)) {
    return true;
  }
  if (swaps >= 3) {
    return false;
  }
  for (int i = 0; i < length; ++i) {
//    if (i != swaps) {
    swap(arr, swaps, i);
    solArr[swaps] = mapIdxToCoords(swaps) + " <-> " + mapIdxToCoords(i);
    if (getSol(arr, target, solArr, swaps + 1)) {
      cout << "Solution found. Number of moves: " << swaps + 1 << endl;
      printSol(solArr, swaps + 1);
      printArr(arr);
    }
    swap(arr, swaps, i);
    solArr[swaps] = "";
//    }
  }
  return false;
}

void printSwap(string coord1, string coord2) {
  cout << coord1 + " <-> " + coord2 << endl;
}

string toCoord(int r, int c) {
  return to_string(r) + ", " + to_string(c);
}

void printCube(int ***cube, int _side) {
  for (int i = 0; i < _side; ++i) {
    for (int j = 0; j < _side; ++j) {
      for (int k = 0; k < _side; ++k) {
        cout << cube[i][j][k] << " ";
      }
      cout << '\n';
    }
    cout << '\n';
  }
}



int main() {
//  cout << "Hello, World!" << endl;
//  int target[length] = {6, 3, 0, 7, 4, 1, 8, 5, 2};
//  int initial[length] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
//  int target[length] = {2, 0, 3, 1};
//  int initial[length] = {0, 1, 2, 3 };
//  int target[length] = {12, 8, 4, 0, 13, 9, 5, 1, 14, 10, 6, 2, 15, 11, 7, 3};
//  int initial[length] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
//  string sol[length];
//  cout << "Starting infinite loop" << endl;
//  getSol(initial, target, sol, 0);
//  printSol(sol, length);
  //Iterates through the rows of the top half of the slice of the rubik's cube
  for (int i = 0; i < side / 2; ++i) {
    //Iterates through the columns of each inner layer of the cube
    for (int j = i; j < side - i - 1; ++j) {
      //Calculates the coordinates of the three swaps to rotate clockwise the
      //four pieces around the slice which correspond to the (i, j) position
      printSwap(toCoord(i, j), toCoord(side - i - 1, side - 1));
      printSwap(toCoord(j, side - i - 1), toCoord(side - i - 1, side - j - 1));
      printSwap(toCoord(side - j - 1, i), toCoord(i, j));
    }
  }
  int ***cube = allocCube(side);
  for (int i = 0; i < side; ++i) {
    for (int j = 0; j < side; ++j) {
      for (int k = 0; k < side; ++k) {
        cube[i][j][k] = i * side * side +  side * j + k;
      }
    }
  }
  for (int i = 0; i < side; ++i) {
    for (int j = 0; j < side; ++j) {
      for (int k = 0; k < side; ++k) {
        cout << "( " << i << ", " << j << ", " << k << " )  " ;
      }
      cout << '\n';
    }
    cout << '\n';
  }
  printCube(cube, side);
  linkedListTests();
  SwapIndicesTuple t = SwapIndicesTuple(2, 3);
  t.add(0, 0);
  t.add(0, 1);
  t.add(0, 2);
  t.add(1, 3);
  t.add(1, 4);
  t.add(1, 5);
  t.print();
  cout << 1 - map(0) <<  " " << 1 - map(1) << " " << 1 - map(2) << " \n";
  cout << map2(0) <<  " " << map2(1) << " " << map2(2) << " \n\n\n";
  RubiksCube c(3);
  c.print();
  for (int l = 0; l < 4; ++l) {
    c.move(XY, 1, ANTI_CLOCKWISE);
    c.print();
//    c.move(YZ, 1, CLOCKWISE);
  }
  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//  c.print();
//  c.move(YZ, 2, CLOCKWISE);
//  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//  c.print();
//  c.move(XY, 2, CLOCKWISE);
//  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//  c.print();
//  c.move(YZ, 0, ANTI_CLOCKWISE);
//  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//  c.print();
//  c.move(XY, 0, ANTI_CLOCKWISE);
//  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
  c.print();
  return 0;
}

