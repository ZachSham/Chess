#ifndef ChessTiles_H
#define ChessTiles_H

#include <iostream>
#include <cctype>
#include "ChessCursor.h"
#include "PieceMovement.h"
#include "Windows.h"
#include <io.h>
#include <fcntl.h>

using namespace std;

const char blackRook = 'r'; // the actual pieces and how are they stored when they are moving and performing attributed/methods
const char whiteRook = 'R';
const char whiteKnight = 'H';
const char blackKnight = 'h';
const char whiteQueen = 'Q';
const char blackQueen = 'q';
const char blackKing = 'k';
const char whiteKing = 'K';
const char whiteBishop = 'B';
const char blackBishop = 'b';
const char whitePawn = 'P';
const char blackPawn = 'p';
const char emptyTile = ' ';


class tile : public ChessCursor , public pieceMovement { // tile can take form of a piece, empty tile, or a cursor

private:
	// the ouput pieces that will print out using a switch statement
	const wchar_t outputWhiteKing = L'\u2654';   // ♔
	const wchar_t outputWhiteQueen = L'\u2655';  // ♕
	const wchar_t outputWhiteRook = L'\u2656';   // ♖
	const wchar_t outputWhiteBishop = L'\u2657'; // ♗
	const wchar_t outputWhiteKnight = L'\u2658'; // ♘
	const wchar_t outputWhitePawn = L'\u2659';   // ♙

	const wchar_t outputBlackKing = L'\u265A';   // ♚
	const wchar_t outputBlackQueen = L'\u265B';  // ♛
	const wchar_t outputBlackRook = L'\u265C';   // ♜
	const wchar_t outputBlackBishop = L'\u265D'; // ♝
	const wchar_t outputBlackKnight = L'\u265E'; // ♞
	const wchar_t outputBlackPawn = L'\u265F';   // ♟


	//set the peices and the types - since there are two people playing the game.
	// uses lowercase and uoppercase letters because they are easy to pass by and compare


	int location;
	bool check = false;
	char myCharacter = emptyTile;
	bool occupied = false;


public:


	void setCharacter();
	int getLocation() const;
	void setLocation(int loc);
	char getEmptyChar() const; // returns a space for empty tile


	void setMyCharacter(char piece);
	void setEmpty(); //sets tiles to a empty tile when chess peice had left that tile 
	char getMyCharacter(); // returns your identity/char
	void printMyCharacter(bool whiteKingIsInCheck, bool blackKingIsInCheck); // prints the piece out
	void printCursor();

	void setConsoleColor(int color) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}

	// moving pattern also checks if it your turn to move
	bool isOccupied() const;

};


//create a funtion like shoot that takes in the vector assigned to the pawn selected and basically makes it allowed to move 
//either until it is on top of a peice, reaches end of the map



void tile::setLocation(int loc) { // to set the location so the tile knows where it is at the beggining of game
	location = loc;

}

char tile::getMyCharacter() { // gets the piece to make sure black pieces dont take other black piece in board.h
	return myCharacter;
}

char tile::getEmptyChar()const { // gets the empty char ' ' just a space for checking if there is nothing at that place
	return emptyTile;
}
bool tile::isOccupied()const { // checks if the tile is occupied currently
	return occupied;
}

void tile::printCursor(){ // prints the cursor out if the tile takes form of one
	wcout << showCursor();
	 
}

void tile::printMyCharacter(bool whiteKingIsInCheck, bool blackKingIsInCheck) { // takes the piece 'char', and whatvere letter it is prints out the ms gothic unicode character

	// 15 and 8

	_setmode(_fileno(stdout), _O_U16TEXT);


	if (location == CURSOR_LOCATION) {
		printCursor();
	}
	else {
		switch (myCharacter) { // these are just for ouput and have nothing to do with game 
		case whiteRook:
			setConsoleColor(15); // changes color for different piece (bright white for white pieces) and grey for black to make it more visible
			wcout << outputWhiteRook;
			break;
		case blackRook:
			setConsoleColor(8);
			wcout << outputBlackRook;
			break;
		case whiteKnight:
			setConsoleColor(15);
			wcout << outputWhiteKnight;
			break;
		case blackKnight:
			setConsoleColor(8);
			wcout << outputBlackKnight;
			break;
		case whiteQueen:
			setConsoleColor(15);
			wcout << outputWhiteQueen;
			break;
		case blackQueen:
			setConsoleColor(8);
			wcout << outputBlackQueen;
			break;
		case whiteKing:
			if (whiteKingIsInCheck) { // if king is in check and tries to move it becomes red
				setConsoleColor(4);
				wcout << outputWhiteKing;
			}
			else {
				setConsoleColor(15);
				wcout << outputWhiteKing;
			}
			break;
		case blackKing:
			if (blackKingIsInCheck) { // king becomes red if checked and tries to move without getting out of check
				setConsoleColor(4);
				wcout << outputBlackKing;
			}
			else {
				setConsoleColor(8);
				wcout << outputBlackKing;
			}
			break;
		case whiteBishop:
			setConsoleColor(15);
			wcout << outputWhiteBishop;
			break;
		case blackBishop:
			setConsoleColor(8);
			wcout << outputBlackBishop;
			break;
		case whitePawn:
			setConsoleColor(15);
			wcout << outputWhitePawn;
			break;
		case blackPawn:
			setConsoleColor(8);
			wcout << outputWhitePawn;
			break;
		}

	}



}

// sets the tiles up at the beggining also changes the tiles to the pieces if they have been legally moved

void tile::setMyCharacter(char piece) { // can set your tile as any piece when they move around

	myCharacter = piece;
	occupied = true;
}
// sets the tiles to empty if a piece is moved
void tile::setEmpty() { // sets tile as empty when piece leaves the last tile

	myCharacter = ' ';
	occupied = false;
}



#endif // !ChessTiles_H