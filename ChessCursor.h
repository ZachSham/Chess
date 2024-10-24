#ifndef CURSOR_H
#define CURSOR_H
#include <vector>
#include <string>
#include <iostream>
#include <mutex>
using namespace std;
const int chessBoardSize = 64;
class ChessCursor { // same cursor used for minesweeper
protected:
    int cursorShape = 0;
    vector<char> Shapes = { '|', '/', '-', '\\' };
public:
    static int CURSOR_LOCATION;

    void setCLocation(int location) {
        CURSOR_LOCATION = location;
    }

    int getCLocation() const {
        return CURSOR_LOCATION;
    }

    char showCursor() {
        char currentShape = Shapes.at(cursorShape);
        cursorShape++;
        if (cursorShape >= Shapes.size()) {
            cursorShape = 0;
        }
        return currentShape;
    }

};

int ChessCursor::CURSOR_LOCATION = (chessBoardSize - 4);
#endif // !CURSOR_H