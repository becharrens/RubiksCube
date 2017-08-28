#include <iostream>
#include <regex>
#include "RubiksCube.h"

#define side 3
#define length 4
#define map(x) (x * x) % 3
#define map2(x) 1 + map((x + 1) % 3)

#include "LinkedListTests.h"
#include "SwapIndicesTuple.h"

using namespace std;

string::iterator applyNextMove(string moves, string::iterator iter);
string::iterator applyNextMove(string moves, string::iterator iter);

static const regex faceMove("[ULFDRB](2| |$)", regex_constants::icase);
static const regex internalMove("([1-9]([0-9]*)?)?[ULFDRB][2]?( |$)", regex_constants::icase);

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


Piece ***alloc3D(int s) {
  Piece ***arr = new Piece **[s];
  arr[0] = new Piece *[s * s];
  arr[0][0] = new Piece[s * s * s];
  for (int i = 1; i < s; ++i) {
    arr[i] = arr[i - 1] + s;
    arr[i][0] = arr[i - 1][0] + (s * s);
  }
  for (int i = 0; i < s; ++i) {
    for (int j = 1; j < s; ++j) {
      arr[i][j] = arr[i][j - 1] + s;
    }
  }
  return arr;
}

string::iterator getNext(string s, string::iterator iter) {
  cout << *iter;
  return ++iter;
}

void iteratorTest() {
  string s = "H";
  string::iterator iter = s.begin();
  while (iter < s.end()) {
    iter = getNext(s, iter);
  }
}


faceName charToFace(char face) {
  switch (face) {
    case 'U':
      return UP;
    case 'L':
      return LEFT;
    case 'F':
      return FRONT;
    case 'D':
      return DOWN;
    case 'R':
      return RIGHT;
    case 'B':
      return BACK;
    default:
      return VOID;
  }
}

bool isValidSlice(int slice, faceName face) {
  return ((slice >= 0) && (slice < side / 2)) ||
      (((face + 1) / 3 == 1) && slice == side / 2);
}

bool validateMoveSequence(const char *moves) {
  int nOpenBraces = 0;
  regex invalidChar("([^(ULFDRB0-9\\(\\))]).*", regex_constants::icase);
  regex moveRegex("([1-9]([0-9]*)?)?[ULFDRB][2]?( |$|((\\)[1-9]([0-9]*)?)+)).*", regex_constants::icase);
  regex separator("( |$).*");
  int slice;
  char *savePtr;
  for (int i = 0; i < strlen(moves); i++) {
//    cout << *(moves + i) << endl;
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
        //moved, it means that the move was invalid
        if ((slice <= 1 && savePtr >  moves + i)) {
          return false;
        }
        //If the pointer didn't move, it means that the slice to move is an
        //outer slice, it is already 0, otherwise, it is one more than the index
        //of the slice referenced
        if (slice != 0) {
          slice--;
        }
        //Update the offset so that the pointer will point to the next character
        i = (int) (savePtr - moves);
        //Check if the slice referenced is within the bounds for the given face
        if (!isValidSlice(slice, charToFace(*(moves + i)))) {
          return false;
        }
        //Skip all subsequent characters (there might be an optional 2 after
        //the move's letter), but we know from the regex that the move is valid.
        //Hence it is safe to skip all subsequent characters, while parsing
        //closing brackets and their respective numbers
        while (!regex_match((moves + i), separator)) {
          if (*(moves + i) == ')') {
            nOpenBraces--;
            if (nOpenBraces < 0) {
              //Too many closing braces
              return false;
            }
          }
          i++;
        }
      } else {
        return false;
      }
    }
  }
  return (nOpenBraces <= 0);
}

//bool validateMoveSequence(string moves) {
//  int nOpenBraces = 0;
//  regex invalidChar("([^(ULFDRB0-9\\(\\))])", regex_constants::icase);
//  regex moveRegex("([1-9]([0-9]*)?)?[ULFDRB][2]?( |$|\\))", regex_constants::icase);
//  string::iterator strIter = moves.begin();
//  while (strIter != moves.end()) {
//    cout << to_string(*strIter) << endl;
//    if (!isspace(*strIter)) {
//      if (regex_match(string(1, *strIter), invalidChar)) {
//        return false;
//      }
//      if (*strIter == '(') {
//        nOpenBraces++;
//      }
////      else if (regex_match()) {
////
////      }
//    }
//    strIter++;
//  }
//  return false;
//}

moveName getMoveName(char c) {
  switch (c) {
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
      return INVALID;
  }
}



/**
 * PRE: *iter == '('
 * @param moves A string containing the move sequence, following the syntax
 * rules for Rubic's cube moves
 * @param iter string iterator for moves, placed at an opening bracket in the
 * string
 * @return An iterator pointing to the next character after the parsed move
 * sequence
 */
//string::iterator parseMoveSequence(string moves, string::iterator startIter,
//                                   int count) {
//  if (*startIter != '(') {
//    return startIter;
//  }
//  startIter = skipSpaces(moves, startIter);
//  string::iterator iter;
//  bool repeat;
//  do {
//    iter = applyNextMove(moves, iter);
//    iter = skipSpaces(moves, iter);
//  } while (*iter != ')');
//  iter++;
//  int nRepetitions = *iter - '0';
////  if (isInRange(nR));
//  return nullptr;
//}

string::iterator applyNextMove(string moves, string::iterator iter) {
  while (*iter == ' ' && iter < moves.end()) {
    iter++;
  }
  int displaceMent;
  if (*iter == '(') {
//    iter = parseMoveSequence(moves, iter);
  } else if (!isalnum(*iter)) {
    while (*iter != ' ' && iter < moves.end()) {
      iter++;
    }
    return iter;
//    moveName move = getMoveName(*iter);
//    iter++;
  } else if (isnumber(*iter)){

  }
}


bool applyMoves(string moves) {
  string::iterator iter = moves.begin();
  while (*iter == ' ') {
    iter++;
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
//  c.move(YZ, 1, CLOCKWISE);
//  c.print();
//  for (int l = 0; l < 1; ++l) {
//    for (int i = 0; i < 3; ++i) {
//      c.move(YZ, i, CLOCKWISE);
//      cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//    }
//    c.print();
//    c.move(YZ, 1, CLOCKWISE);
//  }
  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//  c.print();
  //F
  c.move(YZ, 2, CLOCKWISE);
  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
  c.print();
  //R
  c.move(XY, 2, CLOCKWISE);
  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
  c.print();
  //b
  c.move(YZ, 0, ANTI_CLOCKWISE);
  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
  c.print();
  //l
  c.move(XY, 0, ANTI_CLOCKWISE);
  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
  c.print();
//  iteratorTest();
//  string regex_str = "([1-9]([0-9]*)?)?[ULFDRB][2]?( |$)";
//  string regex_str = "[^\\s]+";
//  regex r1(regex_str, regex_constants::icase);
//  string s = "U u2 (2u 3F F2)4 F$ F4 b2 L 2R ";
//  sregex_iterator iter(s.begin(), s.end(), r1);
//  sregex_iterator rEnd;
//  while (iter != rEnd) {
//    cout << iter->str() << endl;
//    iter++;
//  }
  string s = " /%[f(pL";
//  regex invalidChar("([^(ULFDRB0-9\\(\\))]).*", regex_constants::icase);
//  regex moveRegex("([1-9]([0-9]*)?)?[ULFDRB][2]?( |$|\\))", regex_constants::icase);
  const char arrS[] = ". 2/%[f(pL";
  const char arrMoves[][30] =
      {"((((2F2)3)4)5)6",           //Valid: Nested Brackets
       "(2F) R2 2d",                //Invalid: Brackets without number
       "((F r2)3 L2)2 U)4",         //Invalid: Too many closing brackets
       "(F (R2 (F (20b2 D)3 D)4 R", //Invalid: Too many opening brackets
       "(R2 2F)3 (R U2)300 l2",     //Valid: Brackets and moves
       "(D2 G2 R2 L u d)",          //Invalid: Invalid moves
       "D3 d r2 b3",                //Invalid: Non-2 number after move
       "2(R2 L2 2R2 U2)2 D L U2"};  //Invalid: Number before opening braces
//  for (int i = 0; i < strlen(arrS); i++) {
//    if (regex_match((arrS + i), invalidChar)) {
//      cout << false << endl;
//    } else {
//      cout << true << endl;
//    }
//  }
  for (int l = 0; l < 8; ++l) {
    cout << ((validateMoveSequence(arrMoves[l]))?"Valid":"Invalid") << endl;
  }
  char *test;
  long a = strtol(arrS + 1, &test, 10);
  cout << a << "------>" << *test << endl;
  return 0;
}

