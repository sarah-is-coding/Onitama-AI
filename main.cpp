#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <ctime>
#include <string>
#include <random>
#include <chrono>
#include "components.h"
#include "display.h"
#include "minimax.h"

// This is an Onitama board game algorithm which implements the MiniMax algorithm and Alpha Beta pruning
using namespace std;

int main() {
    unsigned seed =  chrono::system_clock::now().time_since_epoch().count();
     default_random_engine random_engine(seed);
    // Initialize game state
    GameState state;
    state.winner = NONE;

    state.board = {
    {BLUE_STUDENT, EMPTY, EMPTY, EMPTY, RED_STUDENT},
    {BLUE_STUDENT, EMPTY, EMPTY, EMPTY, RED_STUDENT},
    {BLUE_MASTER, EMPTY, EMPTY, EMPTY, RED_MASTER},
    {BLUE_STUDENT, EMPTY, EMPTY, EMPTY, RED_STUDENT},
    {BLUE_STUDENT, EMPTY, EMPTY, EMPTY, RED_STUDENT}
    };

    // Initialize move cards for both players
    // generate a vector of unique random numbers within the range of Deck size
    vector<int> randomIndices;
    generateUniqueRandomIndices(Deck.size(), randomIndices, random_engine);
    randomIndices.resize(4);

    MoveCard redMoveCards[2] = {Deck[randomIndices[0]], Deck[randomIndices[1]]};
    MoveCard blueMoveCards[2] = {Deck[randomIndices[2]], Deck[randomIndices[3]]};

    // remove intitialized cards from the running game deck
    for (auto it = randomIndices.rbegin(); it != randomIndices.rend(); ++it) {
        Deck.erase(Deck.begin() + *it);
    }

    int maxDepth = 5; // Adjust the search depth as needed

    printBorderScreen("Onitama");
    printOnitamaPieces();

    bool isFirstPlayer = askFirstPlayer();

    if (isFirstPlayer) {
        state.currentPlayer = BLUE;
    } else {
        state.currentPlayer = RED;
    }

    while (state.winner == NONE) {
        Move currentMove;
        string inputcurrentMove;
        string inputBlueUsedCard;
    
        system("clear");

        int alpha =  numeric_limits<int>::min();
        int beta =  numeric_limits<int>::max();

        printBorderScreen("Onitama");
        cout << redMoveCards[0].name << " " << redMoveCards[1].name <<  endl;
        printBoard(state.board);
        cout << blueMoveCards[0].name << " " << blueMoveCards[1].name <<  endl;

        generateLegalMoves(state, redMoveCards, blueMoveCards);

        if (state.currentPlayer == BLUE){ // Human

            cout << "What is your move? ex. (0,1) -> (1,1)\n";
            getline(cin, inputcurrentMove);
            currentMove = parseMoveInput(inputcurrentMove);

            while(isMoveValid(state, currentMove) == false){
                cout << "That move is invalid. Try again." << endl;
                cout << "What is your move? ex. (0,1) -> (1,1)" << endl;
                getline(cin, inputcurrentMove);
                currentMove = parseMoveInput(inputcurrentMove);
            }

            currentMove.usedPiece = state.board[currentMove.x1][currentMove.y1];
            cout << "Which card are you using? \n" << endl;
            getline(cin, inputBlueUsedCard);
            // to add : check that move is allowed for specific card
            currentMove.usedCard = *find_if(Deck.begin(), Deck.end(), [&](const MoveCard& card) { return card.name == inputBlueUsedCard; });

        }
        else if (state.currentPlayer == RED){ // AI

            // Find the best move for the current player using MiniMax with alpha-beta pruning
            int eval = miniMaxAlphaBeta(state, maxDepth, alpha, beta, true, currentMove, redMoveCards, blueMoveCards);

        }

        // Store the state of the target piece before applying the move
        Piece targetPiece = state.board[currentMove.x2][currentMove.y2];
        // Apply the best move found
        applyMove(state, currentMove, redMoveCards, blueMoveCards);

        // Check if the move currentMove results in a win
        checkWinner(state, currentMove, targetPiece, redMoveCards, blueMoveCards);

        // Remove value equlivalent to currentMove.usedCard from the Deck
        Deck.erase( remove_if(Deck.begin(), Deck.end(), [&currentMove](const MoveCard &card) {
            return card == currentMove.usedCard;

        }), Deck.end());

        // if the deck is empty refill it
        if (Deck.size() == 0){
            Deck = {Tiger, Dragon, Frog, Rabbit, Crab, Elephant, Goose, Rooster, Monkey, Mantis, Horse, Ox, Crane, Boar, Eel, Cobra};
        }

        // Swap out player cards 
        vector<int> nextCardIndices;
        generateUniqueRandomIndices(Deck.size(), nextCardIndices, random_engine);
        nextCardIndices.resize(1);

        int currentMoveIndex;
        if (state.currentPlayer == RED){
            // grab index of currentMove.usedCard in redMoveCards
            currentMoveIndex = (redMoveCards[0] == currentMove.usedCard) ? 0 : 1;
            redMoveCards[currentMoveIndex] = Deck[nextCardIndices[0]];
        }
            // grab index of currentMove.usedCard in blueMoveCards
        else if (state.currentPlayer == BLUE){
            currentMoveIndex = (blueMoveCards[0] == currentMove.usedCard) ? 0 : 1;
            blueMoveCards[currentMoveIndex] = Deck[nextCardIndices[0]];
        }

        // Update the current player
        state.currentPlayer = (state.currentPlayer == RED) ? BLUE : RED;

    }

    return 0;
}


