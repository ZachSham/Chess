#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <random>
#include <mutex>
#include <fstream>
#include <iostream>
#include <windows.h>
#include "ChessCursor.h"
#include "ChessBoard.h"

using namespace std;

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color); //for colors
}
void ShowConsoleCursor(bool showFlag)
{
    
    //hides the cursor, use only once at program start
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

bool gotoxy(const WORD x, const WORD y) {
    COORD xy;
    xy.X = x;
    xy.Y = y;
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}

void outputMenu(int menuLocation) {
    gotoxy(0, 0);
    setConsoleColor(2);

    cout << "  ______  __    __   _______     _______.     _______." << endl;
    cout << " /      ||  |  |  | |   ____|   /       |    /       |" << endl; // ASCII art 
    cout << "|  ,----'|  |__|  | |  |__     |   (----`   |   (----`" << endl;
    cout << "|  |     |   __   | |   __|     \\   \\        \\   \\    " << endl;
    cout << "|  `----.|  |  |  | |  |____.----)   |   .----)   |   " << endl;
    cout << " \\______||__|  |__| |_______|_______/    |_______/    " << endl;
    cout << "                                                      " << endl;

    switch (menuLocation) {
    case 0:
        setConsoleColor(1);
        cout << " ___ _            _   " << endl;
        cout << "/ __| |_ __ _ _ _| |_ " << endl;
        cout << "\\__ \\  _/ _` | '_|  _|" << endl; // switches between which one is grey and blue when you go up and down on arrows
        cout << "|___/\\__\\__,_|_|  \\__|" << endl;
        setConsoleColor(8);
        cout << "  ___     _ _   " << endl;
        cout << " | ____ _(_| |_ " << endl;
        cout << " | _|\\ \\ | |  _|" << endl;
        cout << " |___/_\\_|_|\\__|" << endl;
        break;
    case 1:
        setConsoleColor(8);
        cout << " ___ _            _   " << endl;
        cout << "/ __| |_ __ _ _ _| |_ " << endl;
        cout << "\\__ \\  _/ _` | '_|  _|" << endl;
        cout << "|___/\\__\\__,_|_|  \\__|" << endl;
        setConsoleColor(1);
        cout << "  ___     _ _   " << endl;
        cout << " | ____ _(_| |_ " << endl;
        cout << " | _|\\ \\ | |  _|" << endl;
        cout << " |___/_\\_|_|\\__|" << endl;
        break;
    }
    setConsoleColor(7);
}

int menu() { // this is for start menu when loading into chess game
    bool startMenu = true;
    int menuLocation = 0;
    outputMenu(menuLocation);

    do {
        ShowConsoleCursor(false);
        if (GetAsyncKeyState(VK_DOWN)) { // you can scroll up and down through options
            if (menuLocation < 1) {
                menuLocation += 1;
                outputMenu(menuLocation);
            }
        }
        else if (GetAsyncKeyState(VK_UP)) {
            if (menuLocation > 0) {
                menuLocation -= 1;
                outputMenu(menuLocation);
            }
        }
        else if (GetAsyncKeyState(VK_SPACE)) { 
            startMenu = false;
        }

        Sleep(50); // sleep
    } while (startMenu);

    setConsoleColor(7); //changes color back to original
    return menuLocation;
}

int main() {
    int gameStart = menu(); // chose to store gamestart selection as a int to have multiple options foor menu

    if (gameStart == 0) {
        ChessBoard c1;

        bool game_running = true;
        int time = 30; // auto_update_interval

        system("cls");

        thread thread_obj(&ChessBoard::c_update, &c1, time); // thread for cursor update

        while (game_running) {
            if (GetAsyncKeyState(VK_DOWN)) {
                c1.move_down();
                Sleep(80); // sleep
            }
            else if (GetAsyncKeyState(VK_UP)) {
                c1.move_up();
                Sleep(80); // sleep
            }
            else if (GetAsyncKeyState(VK_RIGHT)) {
                c1.move_right();
                Sleep(80); // sleep
            }
            else if (GetAsyncKeyState(VK_LEFT)) {
                c1.move_left();
                Sleep(80); // sleep
            }
            else if (GetAsyncKeyState(VK_SPACE)) {
                c1.select();
                Sleep(250);
            }
            else if (GetAsyncKeyState(VK_ESCAPE)) {
                game_running = false;
            }

        }

        thread_obj.detach(); // stops cursor updates
        system("cls");
    }

    return 0;
}
