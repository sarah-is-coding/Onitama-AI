#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
using namespace std;
#include "components.h"
#include <vector>
#include <sstream>
#include <string>

void printBorderScreen(string write) {
    string border(31, '=');
    string emptyLine = "                              ";
    string gameName = write + "           ";

    cout << border << endl;
    cout << emptyLine << endl;
    cout << gameName << endl;
    cout << emptyLine << endl;
    cout << border << endl;
}

void printOnitamaPieces() {
    //cout << " \033[1;36m"; // Set text color to cyan
    cout << "               _ \n";
    cout << "              ( )\n";
    cout << "      ()|    -| |-     ()| \n";
    cout << "     -||-    || ||    -||-  \n";
    cout << "     .||     |---|    .||   \n";
    cout << "     /==\\   /|/ \\|\\   /==\\  \n";
    cout << "    /    \\ /  | |  \\ /    \\ \n";
    cout << "   /      \\   | |   /      \\ \n";
    cout << "  /________\\__|_|__/________\\ \n";
    //cout << "\033[0m"; // Reset text color
    cout << "\n";
}

bool askFirstPlayer() {
    string input;
    cout << "Would you like to be first player? (y/n): ";
    getline(std::cin, input);

    return input == "y" || input == "Y";
}

Move parseMoveInput(const string& input) {
    Move move;
    char dummy; // To read unwanted characters such as '(', ',', ')', and '-'
    
    istringstream ss(input);
    ss >> dummy >> move.x1 >> dummy >> move.y1 >> dummy >> dummy >> dummy >> dummy >> move.x2 >> dummy >> move.y2 >> dummy;

    return move;
}

void printBoard(const  vector< vector<Piece>> &board) {
/**
 * Prints the Onitama board game state in a human-readable format.
 * The printBoard function displays the current board state by iterating through the 2D
 * board vector and printing the appropriate characters for each piece on the board.
 * The output is formatted to display the board as a grid, with 'r' representing red students,
 * 'R' for red master, 'b' for blue students, 'B' for blue master, and '_' for empty spaces.

 * @param board A 2D vector of Piece enums representing the current game board.
 */
    for (int col = BOARD_SIZE - 1; col >= 0; --col) {
        for (int row = 0; row < BOARD_SIZE; ++row) {
            char c;
            Piece piece = board[row][col];
            switch (piece) {
                case RED_STUDENT:
                    c = 'r';
                    break;
                case RED_MASTER:
                    c = 'R';
                    break;
                case BLUE_STUDENT:
                    c = 'b';
                    break;
                case BLUE_MASTER:
                    c = 'B';
                    break;
                default:
                    c = '_';
                    break;
            }
            cout << c << ' ';
        }
        cout << endl;
    }
}


#endif // DISPLAY_H