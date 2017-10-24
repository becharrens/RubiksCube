#include <iostream>
#include <regex>
#include "RubiksCube.h"

using namespace std;

//Good sequence to test all possible moves of the cube
//char seq[] = "U L F D R B u l f d r b U2 L2 F2 D2 R2 B2 u2 l2 f2 d2 r2 b2 2F "
//    "2D 2R 2F2 2D2 2R2";
//if (c.applyMoveSequence(seq)) {
//  cout << "true" << endl;
//  c.print();
//}
//char seq2[] = "u l U L U F u f";
//cout << "--------------------------" << endl;
//c.applyMoveSequence(seq2);
//c.print();


int main() {
  cout << "Welcome to the Terminal-based Rubik's cube" << endl << endl;
  int side = -1;
  while (side < 2 || side > 10) {
    cout << "Enter the size of the cube: ";
    cin >> side;
    cout << endl;
  }
  std::cin.ignore();
  RubiksCube rubiksCube(side);
  string moveSeq;
  cout << "Cube state:" << endl;
  rubiksCube.print();
  cout << "------------------------------------------------------" << endl;
  while (moveSeq != "quit") {
    cout << "Enter a move or move sequence: " << endl;
    if (!getline(cin, moveSeq)) {
      break;
    }
    if (rubiksCube.applyMoveSequence(moveSeq.c_str())) {
      cout << "Cube state:" << endl;
      rubiksCube.print();
      cout << "------------------------------------------------------" << endl;
    } else {
      cout << "Your input was an invalid move sequence" << endl;
    }
  }

  return 0;
}
