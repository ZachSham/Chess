#ifndef PieceMovement_H
#define PieceMovement_H
#include <iostream>

#include <vector>

#include <cmath>

using namespace std;

const int chessWidth = 8;
const int chessTotalBoard = 64;
const char rook = 'r'; // piece movement only knows pieces and their movemnets patterns not the color of the pieces
const char knight = 'h';
const char queen = 'q';
const char king = 'k';
const char bishop = 'b';
const char blkPawn = 'p';
const char whtPawn = 'P';

class pieceMovement {

private:
public:
    bool validateMove(int loc, int cursorLoc, char piece, bool attacking, vector<bool> obstaclePieces); // this function takes the piece and where you are trying to go and will move you there if you are allowed
    bool isKingInCheck(int loc, int kingLoc);
   
    bool pawnMove(int location, int selectedLocation, char piece, bool attacking, vector<bool> obstaclePieces); // movement sets for pawn
    bool kingMove(int location, int selectedLocation); // king movement
    bool bishopMove(int location, int selectedLocation, vector<bool> obstaclePieces); // bishop movement
    bool rookMove(int location, int selectedLocation, vector<bool> obstaclePieces); // rook movement, queen combines both rook and bishop
    bool knightMove(int location, int selectedLocation); // knight movemnet 

};


bool pieceMovement::validateMove(int loc, int selectedLoc, char piece, bool attacking, vector<bool> obstaclePieces) {
    bool valid;
    switch (tolower(piece)) { // goes through all pieces not color of piece, becauyse it only careas about obstacles and movement pattern

    case blkPawn:// black pawn used just for switch statement because it is 'lowercase'

        //can only move up/down and diagonal if there is a peice also two tiles in beginning of the game.
        valid = pawnMove(loc, selectedLoc, piece, attacking, obstaclePieces);
        break;

    case knight://the Knight
        //can only move in an L shape
        valid = knightMove(loc, selectedLoc);
        break;
    case rook://the Rook
        //can only move up and down/sideways.
        valid = rookMove(loc, selectedLoc, obstaclePieces);
        break;
    case bishop://the Bishop
        //can only move diagonal both sides.
        valid = bishopMove(loc, selectedLoc, obstaclePieces);
        break;
    case queen://the Queen
        //can move anywhere- has the bishop,Rooks abilities
        //so it just uses bishopMove and rookMove functions instead of having another function and repeating code
        valid = bishopMove(loc, selectedLoc, obstaclePieces);

        if (!valid) {

            valid = rookMove(loc, selectedLoc, obstaclePieces);

        }
        break;
    case king://the King
        //can only move to the adjacents.
        valid = kingMove(loc, selectedLoc);
        break;
    }
    return valid;
}

bool pieceMovement::pawnMove(int location, int selectedLocation, char piece, bool attacking, vector<bool> obstaclePieces) {

    //if reaches one of the ends of the board - depends whitch type of pawn- user inputs a character to make the peice not a pawn anymore.

    bool validMove = false;

    int firstMoveWhiteIndex = chessTotalBoard - (2 * chessWidth);
    switch (piece) {
    case whtPawn:
        if (attacking) {
        //once reaches the end of the board user has to input what peice it wants it to transform into.
            validMove = ((selectedLocation == (location - chessWidth - 1)) + (selectedLocation == (location - chessWidth + 1)));
        }
        else {
            if (location > chessTotalBoard - ((chessWidth * 2) + 1) && location < chessTotalBoard - (chessWidth)) {
            //can go up 2 also go up 1
                validMove = (selectedLocation == (location - (chessWidth * 2))) + (selectedLocation == (location - chessWidth));
                if (obstaclePieces.at(location - chessWidth)) {
                    validMove = false;
                }
            }
            else {
                validMove = (selectedLocation == (location - chessWidth));
            }

        }
        break;
    case blkPawn:
        if (attacking) {
            // if attacking can go down diagonal left or diagonal right
            validMove = ((selectedLocation == (location + chessWidth - 1)) + (selectedLocation == (location + chessWidth + 1)));
        }
        else {
 
            if (location > (chessWidth - 1) && location < (chessWidth + chessWidth)){
            //can go down 2
                validMove = (selectedLocation == (location + (chessWidth * 2))) + (selectedLocation == (location + chessWidth));
                if (obstaclePieces.at(location + chessWidth)) {
                    validMove = false;
                }
            }
            else {
                validMove = (selectedLocation == (location + chessWidth));
            }

        }
        break;
    }
    //switch stement takes in the the users input of what they want the pawn peice to be.

    return validMove;
}

bool pieceMovement::kingMove(int location, int selectedLocation) {
    // checks if cursorlocation is adjacent to location(king) using adjacents vector
    bool validMove = false;
    bool flag = false;

    vector<int> possibleMoves = { location - 1,
        location + 1,
        location - chessWidth ,
        location - chessWidth - 1,
        location - chessWidth + 1,
        location + chessWidth,
        location + chessWidth - 1,
        location + chessWidth + 1
    };
    for (int i = 0; i < possibleMoves.size() && !flag; i++) {
        if (selectedLocation == possibleMoves.at(i)) {
            // Cursor location is a valid move for the knight
            validMove = true;
            flag = true;
        }
    }
    return validMove;
}

bool pieceMovement::bishopMove(int location, int selectedLocation, vector<bool> obstaclePieces) {

    bool bishopMove = false;
    bool flag = false;
    int colDiff = ((location % chessWidth) - (selectedLocation % chessWidth)); //left to right
    int rowDiff = (location - selectedLocation - colDiff) / chessWidth;  // up and down

    if (abs(colDiff) == abs(rowDiff)) { // if the move is as far horizontal as it is vertical in rleation to bishop inital position it is a valid move

        bishopMove = true;

        int pathIndex = location;
        if (colDiff > 0 && rowDiff > 0) { // checks through the obtacle pieces in up left direction
            // up left
            for (int i = 0; i < (abs(colDiff) - 1) && !flag; i++) {// coldiff -1 because it does not check tile it is going to , that is checked within board
                pathIndex += (-chessWidth - 1);
                if (obstaclePieces.at(pathIndex)) {
                    bishopMove = false;
                    flag = true;
                }
            }
        }else if (colDiff < 0 && rowDiff >0) { // checks through the obtacle pieces in up right direction
            // up right
            for (int i = 0; i < (abs(colDiff) - 1) && !flag; i++) {
                pathIndex += (-chessWidth + 1);
                if (obstaclePieces.at(pathIndex)) {
                    bishopMove = false;
                    flag = true;
                }
            }
        }else if (colDiff > 0 && rowDiff < 0) { // checks through the obtacle pieces in down left direction
            // left down
            for (int i = 0; i < (abs(colDiff) - 1) && !flag; i++) {
                pathIndex += (chessWidth - 1);
                if (obstaclePieces.at(pathIndex)) {
                    bishopMove = false;
                    flag = true;
                }
            }
        }else { // this else would be if they are both negative less than 0, which would be obstackles pieces in right and down direction
            // right down
            for (int i = 0; i < (abs(colDiff) - 1) && !flag; i++) {
                pathIndex += (chessWidth + 1);
                if (obstaclePieces.at(pathIndex)) {
                    bishopMove = false;
                    flag = true;
                }
            }
        }
    }
    //have a vector of locations the Bishop can go
    // checks if cursorlocation is diagonal to location(bishop)

    return bishopMove;
}

bool pieceMovement::rookMove(int location, int selectedLocation, vector<bool> obstaclePieces) {
    // checks if cursorlocation is up/down or sideways from the location(rook)
    int diff = location - selectedLocation;
    int colDiff = (abs(diff) % chessWidth); //left- to right+
    int rowDiff = (diff) / chessWidth;  // up+ and down-
    int direction = 1;
    if (diff > 0) { //opposite if location is bigger because you will have to iterate backwards
        direction = -1;
    }
    //rest of code
    bool RookMove = false;
    bool flag = false;
    // If either the row difference or column difference is zero, it means the cursorLocation is in the same row or column as the rook's location
    if (rowDiff == 0 || colDiff == 0) {
        // Cursor location is either in the same row or same column as the rook's location, making the move valid
        RookMove = true;
        int pathIndex = location;
        if (rowDiff != 0) { // checks all obstackes piece vertically in direction stored
            for (int i = 0; i < (abs(rowDiff) -1) && !flag; i++) {
                pathIndex += (chessWidth * (direction));
                if (obstaclePieces.at(pathIndex)) {
                    flag = true;
                    RookMove = false;
                }
            }
        }
        else { // checks all obastcles horizontally in direction stored at the beginning
            for (int i = 0; i < (abs(colDiff) - 1) && !flag; i++) {
                pathIndex += direction;
                if (obstaclePieces.at(pathIndex)) {
                    flag = true;
                    RookMove = false;
                }
            }
        }
    }
    // If cursorLocation is neither in the same row nor in the same column, it's an invalid move for the rook
    return RookMove;
}

bool pieceMovement::knightMove(int location, int selectedLocation) {
    bool validMove = false;
    bool flag = false;
    // Define all possible moves for a knight
    vector<int> possibleMoves = {(location - (2 * chessWidth)) - 1,  // Up 2, Left 1
        (location - (2 * chessWidth)) + 1,  // Up 2, Right 1
        (location - chessWidth) - 2,      // Up 1, Left 
        (location - chessWidth) + 2,      // Up 1, Right 2
        (location + (2 * chessWidth)) - 1,  // Down 2, Left 1
        (location + (2 * chessWidth)) + 1,  // Down 2, Right 1
        (location + chessWidth) - 2,      // Down 1, Left 2
        (location + chessWidth) + 2       // Down 1, Right 2
    };
    //checks if selected locations is a valid move for the knight

    for (int i = 0; i < possibleMoves.size() && !flag; i++) {
        if (selectedLocation == possibleMoves.at(i)) {
            // Cursor location is a valid move for the knight
            validMove = true;
            flag = true;
        }
    }
    return validMove;
}


#endif // !ChessPieces_H