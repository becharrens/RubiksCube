#include <iostream>
#include <regex>
#include "RubiksCube.h"

#define side 3
#define length 4
#define map(x) (x * x) % 3
#define map2(x) 1 + map((x + 1) % 3)

#include "LinkedListTests.h"
#include "SwapIndicesTuple.h"

#include <forward_list>

using namespace std;

struct C {
};

struct A : C {
    int x;
public:
    A(int x1) : x(x1) {};
};

struct B : C {
    int y;
public:
    B(int y1) : y(y1) {};
};

string::iterator applyNextMove(string moves, string::iterator iter);

string::iterator applyNextMove(string moves, string::iterator iter);

static const regex faceMove("[ULFDRB](2| |$)", regex_constants::icase);
static const regex internalMove("([1-9]([0-9]*)?)?[ULFDRB][2]?( |$)",
                                regex_constants::icase);

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
  regex moveRegex("([1-9]([0-9]*)?)?[ULFDRB][2]?( |$|((\\)[1-9]([0-9]*)?)+)).*",
                  regex_constants::icase);
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
        if ((slice <= 1 && savePtr > moves + i)) {
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
  } else if (isnumber(*iter)) {

  }
  return nullptr;
}


bool applyMoves(string moves) {
  string::iterator iter = moves.begin();
  while (*iter == ' ') {
    iter++;
  }
  return true;
}

/**
 * PRE: The move sequence has been validated beforehand (the move sequence is
 * valid)
 * Assuming that the precondition holds and that the character pointed to by
 * displacement in moves is the beginning of a bracketed sequence, the function
 * will parse the move sequence, isolating all the moves and then applying the
 * move sequence the required number of times. When the function terminates,
 * the character at displacement is the first character after the move sequence.
 * @param moves A pointer to the beginning of the moves string
 * @param disp The character displacement from the start of the string
 */
forward_list<rubiksMove *>::const_iterator parseSequence(const char *moves, int &disp,
                   forward_list<rubiksMove *> moveList,
              forward_list<rubiksMove *>::const_iterator iter) {
  //Check if the given starting point is in fact the start of a repeated move
  //sequence
  while (disp < strlen(moves)) {

  }
  if (*(moves + disp) == '(') {
    //Skip the '('
    disp++;
    forward_list<rubiksMove *>::const_iterator a;
    int nRepetitions = 0;
    char *savePtr;
    bool closingBraceFound = false;
    //Declare the vector to store the moves in the repeated sequence and reserve
    //space to store a small number of them
    vector<rubiksMove *> moveVec;
    moveVec.reserve(5);
    while (!closingBraceFound) {
      //Iterate through the the string, looking for the start of a move or the
      //end of the sequence. Spaces are skipped
      if (!isspace(*(moves + disp))) {
        if (*(moves + disp) == ')') {
          //If a closing brace is found then the move sequence must end, so
          //parse the number of repetitions and update the disp so it points to
          //to the first character after the number
          closingBraceFound = true;
          nRepetitions = (int) strtol(moves + disp + 1, &savePtr, 10);
          disp = (int) (savePtr - moves);
        } else if (*(moves + disp) == '(') {
          //Start of the move. Parse it and add the parsed information to the
          //vector
//          moveVec.push_back(parseMove(moves, disp));
        }
      }
    }
    //Repeat the move sequence nRepetition times
    for (int i = 0; i < nRepetitions; i++) {
      for (int j = 0; j < moveVec.size(); ++j) {
        //Apply each move (90 degree turns in the appropriate direction)
//        move(moveVec[j]->rotationPlane, moveVec[j]->slice, moveVec[j]->dir);
        if (moveVec[j]->isHalfTurn) {
          //Double turn: apply the same turn again
//          move(moveVec[j]->rotationPlane, moveVec[j]->slice, moveVec[j]->dir);
        }
      }
    }
    moveVec.clear();
  }
  return nullptr;
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
        cube[i][j][k] = i * side * side + side * j + k;
      }
    }
  }
  for (int i = 0; i < side; ++i) {
    for (int j = 0; j < side; ++j) {
      for (int k = 0; k < side; ++k) {
        cout << "( " << i << ", " << j << ", " << k << " )  ";
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
  cout << 1 - map(0) << " " << 1 - map(1) << " " << 1 - map(2) << " \n";
  cout << map2(0) << " " << map2(1) << " " << map2(2) << " \n\n\n";
  RubiksCube c(4);
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
//  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//  c.print();
  //F
//  c.move(YZ, 2, CLOCKWISE);
//  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//  c.print();
  //R
//  c.move(XY, 2, CLOCKWISE);
//  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//  c.print();
  //b
//  c.move(YZ, 0, ANTI_CLOCKWISE);
//  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//  c.print();
  //l
//  c.move(XY, 0, ANTI_CLOCKWISE);
//  cout << "\n\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
//  c.print();
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
    cout << ((validateMoveSequence(arrMoves[l])) ? "Valid" : "Invalid") << endl;
  }
//  struct A yes;
//  struct B no;
//  cout << typeid(no).name() << " " << typeid(yes).name() << endl;
//  if (typeid(no) == typeid(struct B)) {
//    cout << "Upcasting works" << endl;
//  }
  forward_list<struct C *> l;
  struct B *q;
  struct A *w;
  for (int i = 0; i < 3; i++) {
    q = new struct B(2 * i);
    w = new struct A(2 * i + 1);
    l.push_front(q);
    l.push_front(w);
  }
  char *test;
  long a = strtol(arrS + 1, &test, 10);
  cout << a << "------>" << *test << endl;
  cout << repeat("U2 2F (R2 U2) ", 3) << endl;
  const char ww[] = "U2 2F (R2 U2 (B2 D2)2)2";
  int disp = 0;
  int &ref = disp;
  cout << expandMoveSequence(ww, ref) << endl;
  char seq[] = "U L F D R B u l f d r b U2 L2 F2 D2 R2 B2 u2 l2 f2 d2 r2 b2 2F 2D 2R 2F2 2D2 2R2";
  if (c.applyMoveSequence(seq)) {
    cout << "true" << endl;
    c.print();
  }
  char seq2[] = "u l U L U F u f";
  cout << "--------------------------" << endl;
  c.applyMoveSequence(seq2);
  c.print();
  return 0;
}
