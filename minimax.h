#ifndef MINIMAX_H
#define MINIMAX_H

#include <iostream>
using namespace std;

#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <limits>
#include <algorithm>
#include <ctime>
#include <string>
#include "components.h"
#include "display.h"


void checkWinner(GameState &state, const Move &move, Piece &targetPiece, MoveCard *redMoveCards, MoveCard *blueMoveCards) {
/**
 * Checks if the given move results in a win condition for the current player.
 * This function checks if the move results in a win condition, either by capturing the opponent's
 * master (Way of the Stone) or moving the player's own master to the opponent's temple (Way of the Stream).
 * If a win condition is met, the function updates the game state's winner attribute and prints the
 * winning message.
    
 * @param[in,out] state The current game state.
 * @param[in] move The move to check for a win condition.
 * @param[in] targetPiece The piece at the destination cell of the move (before the move is applied).
 */ 
    string borderScreenString;

    // Is there a winner?
    if (state.currentPlayer == RED && targetPiece == BLUE_MASTER) {
        state.winner = RED;
        system("clear");
        borderScreenString = "Red player wins by Way of the Stone!";
    } else if (state.currentPlayer == BLUE && targetPiece == RED_MASTER) {
        state.winner = BLUE;
        system("clear");
        borderScreenString = "Blue player wins by Way of the Stone!";
    } else if (move.x2 == 2 && (move.y2 == 4 && targetPiece == BLUE_MASTER || move.y2 == 0 && targetPiece == RED_MASTER)) {
        state.winner = state.currentPlayer;
        system("clear");
        borderScreenString = (state.winner == RED ? "Red" : "Blue") + string(" player wins by Way of the Stream!");
    }

    if (state.winner != NONE) {
        printBorderScreen(borderScreenString);
        cout << redMoveCards[0].name << " " << redMoveCards[1].name <<  endl;
        printBoard(state.board);
        cout << blueMoveCards[0].name << " " << blueMoveCards[1].name <<  endl;

    }
}

bool isMoveValid(GameState &state, Move &move) {
/**
 * Checks if a given move is valid according to the game rules.
 * This function takes the current game state and a move as input, and checks if the move
 * is valid based on the Onitama game rules. It considers the current player, the source and
 * destination cells, as well as the target piece. The function returns true if the move is
 * valid, and false otherwise.
    
 * @param state The current game state, including the board and current player.
 * @param move The move to be checked for validity, including the used piece and card.
 * @return true If the move is valid according to the game rules.
 * @return false If the move is not valid according to the game rules.
 */

    // Implement the game rules to check if the move is valid
    int x1 = move.x1;
    int y1 = move.y1;
    int x2 = move.x2;
    int y2 = move.y2;

    // Check that source cell is difference from destination cell and that the move is in bounds
    if ((x1 == x2 && y1 == y2) || (x2 < 0 || x2 > BOARD_SIZE-1 || y2 < 0 || y2 > BOARD_SIZE-1)){
        return false;
    }

    Piece movingPiece = state.board[x1][y1];
    Piece targetPiece = state.board[x2][y2];

    // Check if the destination cell is empty or contains an opponent's piece
    if ((state.currentPlayer == RED && (targetPiece == EMPTY || targetPiece == BLUE_STUDENT || targetPiece == BLUE_MASTER)) ||
        (state.currentPlayer == BLUE && (targetPiece == EMPTY || targetPiece == RED_STUDENT || targetPiece == RED_MASTER))) {
        return true;

    }

    return false;
}


void generateLegalMoves(GameState &state, MoveCard *redMoveCards, MoveCard *blueMoveCards) {
/**
 * Generates all possible legal moves for the current player in the given game state.
 * This function iterates through the game board, identifying the current player's pieces and
 * generating a list of legal moves for each piece using the available move cards. The generated
 * legal moves are stored in the game state's legalMoves vector.
    
 * @param[in, out] state Reference to the game state object containing the game board, current player, and legal moves vector.
 * @param[in] redMoveCards Pointer to an array of move cards for the red player.
 * @param[in] blueMoveCards Pointer to an array of move cards for the blue player.
 */

    state.redLegalMoves.clear();
    state.blueLegalMoves.clear();
    MoveCard *moveCards = (state.currentPlayer == RED) ? redMoveCards : blueMoveCards;
    bool isRedPlayer = state.currentPlayer == RED;

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            Piece piece = state.board[x][y];
            if (((piece == RED_STUDENT || piece == RED_MASTER) && state.currentPlayer == RED) ||
                ((piece == BLUE_STUDENT || piece == BLUE_MASTER) && state.currentPlayer == BLUE)) {
                if (piece == RED_STUDENT){
                }
                for (int cardIdx = 0; cardIdx < 2; ++cardIdx) {
                    for (int moveIdx = 0; moveIdx < moveCards[cardIdx].numMoves; ++moveIdx) {
                        Move move;
                        move.x1 = x;
                        move.y1 = y;
                        // if player is red, moves must be negated
                        move.x2 = x + (isRedPlayer ? -moveCards[cardIdx].dx[moveIdx] : moveCards[cardIdx].dx[moveIdx]);
                        move.y2 = y + (isRedPlayer ? -moveCards[cardIdx].dy[moveIdx] : moveCards[cardIdx].dy[moveIdx]);
                        move.usedCard = moveCards[cardIdx];
                        move.usedPiece = piece;

                        if (isMoveValid(state, move)) {
                            isRedPlayer ? state.redLegalMoves.push_back(move) : state.blueLegalMoves.push_back(move);
                        }
                    }
                }
            }
        }
    }
}


int evaluate(const GameState &state, MoveCard *redMoveCards, MoveCard *blueMoveCards) {
    int score = 0;
    const int CAPTURE_BLUE_POINTS = 2;
    const int CAPTURE_BLUE_MASTER_POINTS = 20;
    const int CAPTURE_RED_POINTS = -2;
    const int CAPTURE_RED_MASTER_POINTS = -20;
    const int PROTECT_POINTS = 1;
    const int RED_MASTER_CLOSER_TO_TEMPLE_POINTS = 1;
    const int BLUE_MASTER_CLOSER_TO_TEMPLE_POINTS = -1;

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            Piece piece = state.board[x][y];
            if (piece == RED_STUDENT || piece == RED_MASTER) {
                // More points if red piece hasn't been captured
                score += (piece == RED_MASTER) ? 10 : 1;

                // Check if red piece can capture a blue piece or protect another red piece
                for (const Move &move : state.redLegalMoves) {
                    if (move.x1 == x && move.y1 == y) {
                        int newX = move.x2;
                        int newY = move.y2;

                        Piece target = state.board[newX][newY];
                        if (target == BLUE_STUDENT) {
                            score += CAPTURE_BLUE_POINTS;
                        } else if (target == BLUE_MASTER) {
                            score += CAPTURE_BLUE_MASTER_POINTS;
                        } else if (target == RED_STUDENT || target == RED_MASTER) {
                            score += PROTECT_POINTS;
                        }
                    }
                }

                // Check if the red master is closer to the blue temple
                if (piece == RED_MASTER) {
                    score += RED_MASTER_CLOSER_TO_TEMPLE_POINTS * (4 - abs(x - 2) - y);
                }
            } else if (piece == BLUE_STUDENT || piece == BLUE_MASTER) {
                // Less points if blue piece hasn't been captured
                score -= (piece == BLUE_MASTER) ? 10 : 1;

                // Check if blue piece can capture a red piece
                for (const Move &move : state.blueLegalMoves) {
                    if (move.x1 == x && move.y1 == y) {
                        int newX = move.x2;
                        int newY = move.y2;

                        Piece target = state.board[newX][newY];
                        if (target == RED_STUDENT) {
                            score += CAPTURE_RED_POINTS;
                        } else if (target == RED_MASTER) {
                            score += CAPTURE_RED_MASTER_POINTS;
                        }
                    }
                }

                // Check if the blue master is closer to the red temple
                if (piece == BLUE_MASTER) {
                    score += BLUE_MASTER_CLOSER_TO_TEMPLE_POINTS * (4 - abs(x - 2) - (4 - y));
                }
            }
        }
    }

    return score;
}

void applyMove(GameState &state, const Move &move, MoveCard *redMoveCards, MoveCard *blueMoveCards) {
/**
 * Applies a move to the given game state, updating the board and players' positions.
 * The applyMove function takes a GameState reference and a Move reference, as well as pointers to
 * the red and blue players' move cards. It modifies the game state by moving the piece from its
 * original position to the destination position specified in the Move. It also updates the players'
 * positions on the board accordingly.
    
 * @param state A reference to the current game state.
 * @param move A reference to the Move to be applied.
 * @param redMoveCards A pointer to the red player's move cards.
 * @param blueMoveCards A pointer to the blue player's move cards.
 */
   
    state.board[move.x2][move.y2] = state.board[move.x1][move.y1];
    state.board[move.x1][move.y1] = EMPTY;

}

int miniMaxAlphaBeta(GameState &state, int depth, int alpha, int beta, Move &bestMove, MoveCard *redMoveCards, MoveCard *blueMoveCards) {
/**
 * MiniMax algorithm implementation with Alpha-Beta pruning for the Onitama board game.
 * This function performs a depth-limited search using the MiniMax algorithm and Alpha-Beta pruning
 * to find the best move for the current player in the given game state. The search depth can be
 * adjusted to control the complexity and performance of the algorithm.

 * @param state The current game state.
 * @param depth The remaining search depth for the algorithm.
 * @param alpha The current best value for the maximizing player.
 * @param beta The current best value for the minimizing player.
 * @param bestMove A reference to a Move object, which will store the best move found by the algorithm.
 * @param redMoveCards A pointer to an array of MoveCard objects for the red player.
 * @param blueMoveCards A pointer to an array of MoveCard objects for the blue player.
 * @return The evaluation score of the best move found.
 */

    if (depth == 0 || state.winner != NONE) {
        return evaluate(state, redMoveCards, blueMoveCards);;
    }

    int maxEval =  numeric_limits<int>::min();
    for (const auto &move : state.redLegalMoves) {
        GameState nextState = state;
        applyMove(nextState, move, redMoveCards, blueMoveCards);
        Move dummyMove;
        int eval = miniMaxAlphaBeta(nextState, depth - 1, alpha, beta, dummyMove, redMoveCards, blueMoveCards);
        if (eval > maxEval) {
            maxEval = eval;
            bestMove = move;
        }
        alpha =  max(alpha, eval);
        if (beta <= alpha) {
            break;
        }
    }
    return maxEval;

}



#endif // ONITAMA_H