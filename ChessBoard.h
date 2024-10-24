#ifndef ChessBoard_H
#define ChessBoard_H

#include <iostream>
#include <string>
#include <vector>
#include "windows.h"
#include <thread>
#include <mutex>
#include <io.h>
#include <fcntl.h>
#include "ChessTiles.h"
#include <cctype>

using namespace std;

const wchar_t WHITE = L'\u2588';  //White unicode tiles
const wchar_t BLACK = L'\u2591'; // Black unicode tiles



class ChessBoard {

private:

	int whiteKingLocation;
	int blackKingLocation;

	bool movePiece = false; // first time you hit space this is false because you are selecting piece to move
	int pieceLocation = 0; // keeps starung location statically for when space is hit again

	vector<tile> chessBoard; // the actual game of tiles

	vector<int> outputLocations; // the locations in which actual pieces and tiles will be printed out 


	//witdth and length is 24- 8x3  
	const int outputBoardWidth = 24; //for output
	const int ouputTotalBoard = 576;
	const int chessWidth = 8; //for traditional board
	const int chessTotalBoard = 64;


	// pieces to be set at beggining
	const char blackRook = 'r';
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


	bool whiteMove; // games goes between white and black move
	bool blackKingIsChecked; // true if king is checked
	bool whiteKingIsChecked;

	vector <tile> deadWhitePieces; //both vectors are for ouput and just store the pieces that have been taken
	vector <tile> deadBlackPieces;

public:

	mutex mtx;


	void ShowConsoleCursor(bool showFlag)
	{
		//-------do not change-------
		//hides the cursor, use only once at program start
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(out, &cursorInfo);
		cursorInfo.bVisible = showFlag; // set the cursor visibility
		SetConsoleCursorInfo(out, &cursorInfo);
	}
	bool gotoxy(const WORD x, const WORD y) {
		//-------do not change-------
		COORD xy;
		xy.X = x;
		xy.Y = y;
		return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
	}
	void c_update(int time) {
		while (true) {
			this_thread::sleep_for(chrono::milliseconds(time));
			Draw(false);
		}
	}
	void setConsoleColor(int color) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}

	void move_down();
	void move_right();
	void move_left();
	void move_up();
	void select();
	bool checkMovingPiece(int location, int selectedLocation);

	void Draw(bool gameOver);
	void outputColor(bool printingWhite);
	vector <bool> occupiedLocations(); // checks all the locations with a true if they are being occupied to make sure pieces don't go over other pieces when moving
	bool selectedLocationCheck(char currentChar, char selectedChar); // checks the tile you are trying to go to 
	bool isKinginCheck(int loc, int selectedLoc); // checks all pieces of opposing color to see if in check, if in check can't move till out

	ChessBoard();
};


ChessBoard::ChessBoard() { // constructor for setting all the board and variables up ion beginning

	whiteMove = true;
	blackKingIsChecked = false;
	whiteKingIsChecked = false;

	chessBoard.resize(64);


	int location = outputBoardWidth + 1; // index of very first interactive chess tile


	for (int i = 0; i < chessTotalBoard; i++) { // loop that goes through chessBoard and location vector

		chessBoard.at(i).setLocation(i); // tells each tile within chessboard what location it is at in vector

		outputLocations.push_back(location); // pushes back value in which you will print out the chesstile

		if ((i + 1) % 8 == 0) {
			location += (outputBoardWidth * 2); // skips the two blank lines
		}
		location += 3;
	}


	//sets all the pieces up on the board
	chessBoard.at(0).setMyCharacter(blackRook);
	chessBoard.at(1).setMyCharacter(blackKnight);
	chessBoard.at(2).setMyCharacter(blackBishop);
	chessBoard.at(4).setMyCharacter(blackKing);
	blackKingLocation = 4; // stores king location for checking aspect of game
	chessBoard.at(3).setMyCharacter(blackQueen);
	chessBoard.at(5).setMyCharacter(blackBishop);
	chessBoard.at(6).setMyCharacter(blackKnight);
	chessBoard.at(7).setMyCharacter(blackRook);


	for (int i = chessWidth; i < chessWidth * 2; i++) {

		chessBoard.at(i).setMyCharacter(blackPawn); // sets all black pawns on 2nd row

	}


	for (int i = chessTotalBoard - (chessWidth * 2); i < chessTotalBoard - chessWidth; i++) {

		chessBoard.at(i).setMyCharacter(whitePawn); //sets all the white paws on 2nd to last row

	}

	chessBoard.at(chessTotalBoard - 8).setMyCharacter(whiteRook); //sets all the white pieces on the last row
	chessBoard.at(chessTotalBoard - 7).setMyCharacter(whiteKnight);
	chessBoard.at(chessTotalBoard - 6).setMyCharacter(whiteBishop);
	chessBoard.at(chessTotalBoard - 5).setMyCharacter(whiteQueen);
	chessBoard.at(chessTotalBoard - 4).setMyCharacter(whiteKing);
	whiteKingLocation = chessTotalBoard - 4; // stores king location for checking aspect of game
	chessBoard.at(chessTotalBoard - 3).setMyCharacter(whiteBishop);
	chessBoard.at(chessTotalBoard - 2).setMyCharacter(whiteKnight);
	chessBoard.at(chessTotalBoard - 1).setMyCharacter(whiteRook);
}




void ChessBoard::Draw(bool gameOver) {

	mtx.lock();
	gotoxy(0, 0);
	ShowConsoleCursor(false);
	setConsoleColor(15);

	//basically the tile. make sure to have the chess- checkered patteren
	// printing at the beggining starts with white

	int locationsIndex = 0;

	bool printingWhite = true;

	// for loop that goes through whole board 24 x 24
	for (int i = 0; i < ouputTotalBoard; i++) {


		// if statement that checks if its a location where piece will be ouput
		// also checks if emopty because if its empty you should just print a tile
		if (locationsIndex < chessTotalBoard && outputLocations.at(locationsIndex) == i) {

			if (chessBoard.at(locationsIndex).getMyCharacter() != chessBoard.at(0).getEmptyChar()) {
				wcout << ' ';
				chessBoard.at(locationsIndex).printMyCharacter(whiteKingIsChecked, blackKingIsChecked);  //prints the piece plu spaces on both sides
				wcout << ' ';
				locationsIndex += 1; //adds one to now go to next index in vector of locations for pieces
			}
			else {
				if (locationsIndex == chessBoard.at(0).getCLocation()) {
					wcout << ' ';
					chessBoard.at(locationsIndex).printCursor(); //prints two spaces + cursor 
					wcout << ' ';
					locationsIndex += 1;
				}
				else {
					locationsIndex += 1;
					outputColor(printingWhite); //prints out a tile on the real interactive tiles
				}
			}

		}
		else {

			outputColor(printingWhite); //prints out the tile  on the aestehtic tiles ( the tiles of white and black that are just there for looks)

		}

		if ((i + 1) % 3 == 0 && (i + 1) % 72 != 0) {  // switches color every 3 tiles, but every 72 does not switch because at the end of 3 lines it goes from color to the same color. 
			printingWhite = !printingWhite;
		}

		if ((i + 1) % 24 == 0) { // adds a end line every 24 prints to make it a 24 x 24 
			wcout << endl;
		}

	}
	if (whiteMove) {
		wcout << "CURRENTLY WHITE MOVE " << endl; //prints out this at end of board as a reminder 
	}
	else {
		wcout << "CURRENTLY BLACK MOVE " << endl;
	}
	
	wcout << endl << "White has taken: "; 
	for (int i = 0; i < deadBlackPieces.size(); i++) { //displays all the pieces that have bee taken by other color 
		deadBlackPieces.at(i).printMyCharacter(false, false);
		wcout << ' ';
	}
	setConsoleColor(8);
	wcout << endl << "Black has taken: "; 
	for (int i = 0; i < deadWhitePieces.size(); i++) { // displays all the pieces that have been taken
		deadWhitePieces.at(i).printMyCharacter(false, false);
		wcout << ' ';
	}

	mtx.unlock();
}


void ChessBoard::outputColor(bool printingWhite) {

	_setmode(_fileno(stdout), _O_U16TEXT); // for printing out wchars


	if (printingWhite) {
		setConsoleColor(15); //sets it to bright white
		wcout << WHITE << WHITE << WHITE; // 3 tile prints for wider board

	}
	else {
		setConsoleColor(2); // sets black shaded tiles to green for chess.com look
		wcout << BLACK << BLACK << BLACK;
		setConsoleColor(7); // sets it back
	}
}

vector <bool> ChessBoard::occupiedLocations() { // marks all occupied locations with true and non-occupied with false, for making sure pieces dont pass up others

	vector<bool> occupiedLocations(chessTotalBoard, false);

	for (int i = 0; i < chessTotalBoard; i++) {

		if (chessBoard.at(i).isOccupied()) {
			occupiedLocations.at(i) = true;
		}
	}
	return occupiedLocations;
}



void ChessBoard::move_down() { // basic moevemnt with arrow keys from minesweeper

	if ((chessBoard.at(0).getCLocation() < chessBoard.size() - chessWidth)) {

		chessBoard.at(0).setCLocation(chessBoard.at(0).getCLocation() + chessWidth);
	}
}
void ChessBoard::move_up() {
	if ((chessBoard.at(0).getCLocation() >= chessWidth)) {
		chessBoard.at(0).setCLocation(chessBoard.at(0).getCLocation() - chessWidth);
	}
}
void ChessBoard::move_right() {
	if ((chessBoard.at(0).getCLocation() < chessBoard.size() - 1)) {
		chessBoard.at(0).setCLocation(chessBoard.at(0).getCLocation() + 1);
	}
}
void ChessBoard::move_left() {
	if ((chessBoard.at(0).getCLocation() > 0)) {
		chessBoard.at(0).setCLocation(chessBoard.at(0).getCLocation() - 1);
	}
}

void ChessBoard::select() {

	//press space to select then press space again to move it if its a valid move.
	bool attacking = false; // checks if you arer going to an unoccupied legal space

	if (movePiece) {//second time you hit space and are tryng to move a piece somwehwre
		int selectedLocation = chessBoard.at(0).getCLocation();  // stores location you are sleceting to check for it 

		if (whiteMove && isupper(chessBoard.at(pieceLocation).getMyCharacter())) { // for white


			if (checkMovingPiece(pieceLocation, selectedLocation)) { // function that checks if move is legal, if there are no obstacles, also if your king is in check 
				// return true and iniate if you are allowed to move there
				whiteMove = false; // switches move 
				chessBoard.at(0).setCLocation(blackKingLocation); // moves you cursor to other side of board on king so you don't have to keep scrolling up and down
				
			}

		}
		else if (!whiteMove && islower(chessBoard.at(pieceLocation).getMyCharacter())) { // for black same as white above

			if (checkMovingPiece(pieceLocation, selectedLocation)) { // returns true if you are allowed to move
				whiteMove = true;  // switches turn
				chessBoard.at(0).setCLocation(whiteKingLocation); //moves your cursor to other side
			}
		}

		movePiece = false;
		
	}
	else if (chessBoard.at(chessBoard.at(0).getCLocation()).getMyCharacter() != chessBoard.at(0).getEmptyChar()) { // makes sure you are not just slecting spaces
		pieceLocation = chessBoard.at(0).getCLocation();//first time you hit space it stores your location 
		movePiece = true; // now next time you press, first part of if else will work
	}
}
bool ChessBoard::isKinginCheck(int location, int selectedLocation) {

	bool flag = false;
	char piece = chessBoard.at(selectedLocation).getMyCharacter(); // saves piece for selected location

	chessBoard.at(selectedLocation).setMyCharacter(chessBoard.at(location).getMyCharacter()); // goes through with move to make sure that king is not in check with move
	chessBoard.at(location).setEmpty(); // sets last location back to empty

	vector <bool> occupiedLocs = occupiedLocations(); // stores occupied locs to know what location to choose when checking if is king is being checked

	if (whiteMove) { 

		for (int i = 0; i < chessTotalBoard && !flag; i++) { // goes through a loop if white move that checks all black piece's moves

			if (islower(chessBoard.at(i).getMyCharacter())) { // send black pieces + whitekingLocation to see if piece can move there
				whiteKingIsChecked = chessBoard.at(0).validateMove(i, whiteKingLocation, chessBoard.at(i).getMyCharacter(), true, occupiedLocs);
				if (whiteKingIsChecked) {
					flag = true;
				}
			}
		}
	}
	else {
		for (int i = 0; i < chessTotalBoard && !flag; i++) { // loop that goes through all white pieces

			if (isupper(chessBoard.at(i).getMyCharacter())) { // checks if white piece can get to the king
				blackKingIsChecked = chessBoard.at(0).validateMove(i, blackKingLocation, chessBoard.at(i).getMyCharacter(), true, occupiedLocs);
				if (blackKingIsChecked) {
					flag = true;
				}
			}
		}
	}
	
	chessBoard.at(location).setMyCharacter(chessBoard.at(selectedLocation).getMyCharacter()); // sets pieces back
	chessBoard.at(selectedLocation).setMyCharacter(piece);  

	return blackKingIsChecked + whiteKingIsChecked; // returns if either king is checked
}

bool ChessBoard::checkMovingPiece(int location, int selectedLocation) { // checks all the rules for making the move

	bool attacking = false;
	bool didMoveGoThrough = false;
	whiteKingIsChecked = false;
	blackKingIsChecked = false;
	
		if (selectedLocationCheck(chessBoard.at(location).getMyCharacter(), chessBoard.at(selectedLocation).getMyCharacter())) { //checks place you are trying to move piece and returns true if you are allowed


			if (chessBoard.at(selectedLocation).getMyCharacter() != chessBoard.at(0).getEmptyChar()) { // if you are allowed to move and selectedLocation is not empty, then you are attcking opposite color, which matter for the pawn's movement
				attacking = true;
			}

			if (chessBoard.at(0).validateMove(location, selectedLocation, chessBoard.at(location).getMyCharacter(), attacking, occupiedLocations())) { // send it to function from chess tiles that inherits from pieceMoveemnt that checks your piece and if you are allowed to move to that place by game rules

				char piece = chessBoard.at(selectedLocation).getMyCharacter(); //holds onto the char of the place you are moving 

				if (chessBoard.at(location).getMyCharacter() == whiteKing) { //makes sure to change location of either king if that is the piece you are movng
					whiteKingLocation = selectedLocation;
				}
				else if (chessBoard.at(location).getMyCharacter() == blackKing) {
					blackKingLocation = selectedLocation;
				}
				if (!isKinginCheck(location, selectedLocation)) { // makes sure your king is not in check with move
					chessBoard.at(selectedLocation).setMyCharacter(chessBoard.at(location).getMyCharacter()); //moves the piece on the ChessBoard vector of tile objects
					chessBoard.at(location).setEmpty(); // leaves last tile it was on empty
					didMoveGoThrough = true;
					if (attacking && islower(chessBoard.at(selectedLocation).getMyCharacter())) { // if piece has been taken store it in dead white pieces
						tile t1;
						t1.setMyCharacter(piece);
						deadWhitePieces.push_back(t1);
					}
					else if (attacking && isupper(chessBoard.at(selectedLocation).getMyCharacter())) { // if piece has been taken store it in dead black pieces
						tile t1;
						t1.setMyCharacter(piece);
						deadBlackPieces.push_back(t1);
					}
				}

			}
		}
	
	return didMoveGoThrough;
}


bool ChessBoard::selectedLocationCheck(char currentChar, char selectedChar) { // checks if the place you are going is legal for you to move. 

	bool validMove = false;

	if (selectedChar == ' ') { // if empty always allowed
		validMove = true;
	}
	else {
		bool isCurrentUpperCase = isupper(currentChar); // if is opposite upper/lower then you are allowed to move because you can attack piece that are of opposite colors
		bool isSelectedUpperCase = isupper(selectedChar);

		
		validMove = (isCurrentUpperCase != isSelectedUpperCase);
	}

	return validMove;
}

#endif // !ChessBoard_H

