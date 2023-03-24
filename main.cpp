#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <ctime>
#include <string>
#include <random>
#include <chrono>

// This is an Onitama board game algorithm which implements the MiniMax algorithm and Alpha Beta pruning

const int BOARD_SIZE = 5;
const int MAX_DEPTH = 5;

struct MoveCard {
    std::string name;
    int dx[4];
    int dy[4];
    int numMoves;
};

// overload equal operator to compare names
bool operator==(const MoveCard& lhs, const MoveCard& rhs) {
    return lhs.name == rhs.name;
}


// Define all move cards
MoveCard Tiger = {"Tiger",{0, 0}, {2, -1}, 2};
MoveCard Dragon = {"Dragon",{-2, 2, -1, 1}, {1, 1, -1, -1}, 4};
MoveCard Frog = {"Frog",{-2, -1, 1}, {0, 1, -1}, 3};
MoveCard Rabbit = {"Rabbit",{-1, 1, 2}, {-1, 1, 0}, 3};
MoveCard Crab = {"Crab",{-2, 0, 2}, {0, 1, 0}, 3};
MoveCard Elephant = {"Elephant",{-1, -1, 1, 1}, {1, 0, 0, 1}, 4};
MoveCard Goose = {"Goose",{-1, -1, 1, 1}, {0, 1, 0, -1}, 4};
MoveCard Rooster = {"Rooster",{-1, -1, 1, 1}, {0, -1, 0, 1}, 4};
MoveCard Monkey = {"Monkey",{-1, 1, -1, 1}, {1, 1, -1, -1}, 4};
MoveCard Mantis = {"Manits",{-1, 0, 1}, {1, -1, 1}, 3};
MoveCard Horse = {"Horse",{-1, 0, 0}, {0, 1, 1}, 3};
MoveCard Ox = {"Ox",{0, 0, 1}, {1, -1, 1}, 3};
MoveCard Crane = {"Crane",{0, -1, 1}, {1, -1, -1}, 3};
MoveCard Boar = {"Boar",{-1, 0, 1}, {0, 1, 0}, 3};
MoveCard Eel = {"Eel",{-1, -1, 1}, {1, -1, 0}, 3};
MoveCard Cobra = {"Cobra",{-1, 1, 1}, {0, 1, -1}, 3};

std::vector<MoveCard> Deck = {Tiger, Dragon, Frog, Rabbit, Crab, Elephant, Goose, Rooster, Monkey, Mantis, Horse, Ox, Crane, Boar, Eel, Cobra};

enum Piece { EMPTY, RED_MASTER, RED_STUDENT, BLUE_MASTER, BLUE_STUDENT };
enum Player { RED, BLUE, NONE };

struct Move {
    Piece usedPiece;
    MoveCard usedCard;
    int x1, y1, x2, y2;
};

struct GameState {
    std::vector<std::vector<Piece>> board;
    std::vector<Move> legalMoves;
    Player currentPlayer;
    Player winner;
};

void printBoard(const std::vector<std::vector<Piece>> &board);
void applyMove(GameState &state, const Move &move);
void checkWinner(GameState &state, const Move &move, Piece &targetPiece);
bool isMoveValid(const GameState &state, const Move &move);
void generateLegalMoves(GameState &state, MoveCard *redMoveCards, MoveCard *blueMoveCards);
int evaluate(const GameState &state, MoveCard *redMoveCards, MoveCard *blueMoveCards);
int miniMaxAlphaBeta(GameState &state, int depth, int alpha, int beta, bool maximizingPlayer, Move &bestMove, MoveCard *redMoveCards, MoveCard *blueMoveCards);


void printBoard(const std::vector<std::vector<Piece>> &board) {
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
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
}

template <typename RandomEngine>
void generateUniqueRandomIndices(int range, std::vector<int> &randomIndices, RandomEngine& random_engine) {
/**
 * @brief Generates a vector of unique random indices within a specified range.
 * 
 * This function populates a vector with unique random indices ranging from 0 to range-1.
 * The indices are then shuffled using the provided random engine to ensure uniqueness.
 * This is typically used for sampling without replacement from a larger dataset.
 * 
 * @param range The range of indices to generate (0 to range-1).
 * @param[out] randomIndices The vector to populate with unique random indices.
 * @param random_engine The random engine used to shuffle the indices.
 */
    for (int i = 0; i < range; i++) {
        randomIndices.push_back(i);
    }

    std::shuffle(randomIndices.begin(), randomIndices.end(), random_engine);
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

    if (state.currentPlayer == RED && targetPiece == BLUE_MASTER) {
        state.winner = RED;
        std::cout << "Red player wins by Way of the Stone!" << std::endl;
    } else if (state.currentPlayer == BLUE && targetPiece == RED_MASTER) {
        state.winner = BLUE;
        std::cout << "Blue player wins by Way of the Stone!" << std::endl;
    } else if (move.x2 == 2 && (move.y2 == 4 && targetPiece == BLUE_MASTER || move.y2 == 0 && targetPiece == RED_MASTER)) {
        state.winner = state.currentPlayer;
        std::cout << (state.winner == RED ? "Red" : "Blue") << " player wins by Way of the Stream!" << std::endl;
    }
    std::cout << redMoveCards[0].name << " " << redMoveCards[1].name << std::endl;
    printBoard(state.board);
    std::cout << blueMoveCards[0].name << " " << blueMoveCards[1].name << std::endl;
}

std::ostream& operator<<(std::ostream &os, const MoveCard &moveCard) {
    os << moveCard.name;
    return os;
}

int main() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine random_engine(seed);
    // Initialize game state
    GameState state;
    state.currentPlayer = RED;
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
    std::vector<int> randomIndices;
    generateUniqueRandomIndices(Deck.size(), randomIndices, random_engine);
    randomIndices.resize(4);

    MoveCard redMoveCards[2] = {Deck[randomIndices[0]], Deck[randomIndices[1]]};
    MoveCard blueMoveCards[2] = {Deck[randomIndices[2]], Deck[randomIndices[3]]};

    // remove intitialized cards from the running game deck
    for (auto it = randomIndices.rbegin(); it != randomIndices.rend(); ++it) {
        Deck.erase(Deck.begin() + *it);
    }

    int maxDepth = 5; // Adjust the search depth as needed

    while (state.winner == NONE) {
        //system("clear");
        std::cout << redMoveCards[0].name << " " << redMoveCards[1].name << std::endl;
        printBoard(state.board);
        std::cout << blueMoveCards[0].name << " " << blueMoveCards[1].name << std::endl;
        
        int alpha = std::numeric_limits<int>::min();
        int beta = std::numeric_limits<int>::max();

        // Find the best move for the current player using MiniMax with alpha-beta pruning
        Move bestMove;
        generateLegalMoves(state, redMoveCards, blueMoveCards);
        int eval = miniMaxAlphaBeta(state, maxDepth, alpha, beta, state.currentPlayer == RED, bestMove, redMoveCards, blueMoveCards);
        
        // Store the state of the target piece before applying the move
        Piece targetPiece = state.board[bestMove.x2][bestMove.y2];
        // Apply the best move found
        applyMove(state, bestMove, redMoveCards, blueMoveCards);

        // Check if the move bestMove results in a win
        checkWinner(state, bestMove, targetPiece, redMoveCards, blueMoveCards);

        // Remove value equlivalent to bestMove.usedCard from the Deck
        Deck.erase(std::remove_if(Deck.begin(), Deck.end(), [&bestMove](const MoveCard &card) {
            return card == bestMove.usedCard;

        }), Deck.end());

        // if the deck is empty refill it
        if (Deck.size() == 0){
            Deck = {Tiger, Dragon, Frog, Rabbit, Crab, Elephant, Goose, Rooster, Monkey, Mantis, Horse, Ox, Crane, Boar, Eel, Cobra};
        }

        // Swap out player cards 
        std::vector<int> nextCardIndices;
        generateUniqueRandomIndices(Deck.size(), nextCardIndices, random_engine);
        nextCardIndices.resize(1);

        int bestMoveIndex;
        if (state.currentPlayer == RED){
            // grab index of bestMove.usedCard in redMoveCards
            bestMoveIndex = (redMoveCards[0] == bestMove.usedCard) ? 0 : 1;
            redMoveCards[bestMoveIndex] = Deck[nextCardIndices[0]];
        }
            // grab index of bestMove.usedCard in blueMoveCards
        else if (state.currentPlayer == BLUE){
            bestMoveIndex = (blueMoveCards[0] == bestMove.usedCard) ? 0 : 1;
            blueMoveCards[bestMoveIndex] = Deck[nextCardIndices[0]];
        }

        // Update the current player
        state.currentPlayer = (state.currentPlayer == RED) ? BLUE : RED;

    }

    return 0;
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

    state.legalMoves.clear();
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
                            state.legalMoves.push_back(move);
                        }
                    }
                }
            }
        }
    }
}


int evaluate(const GameState &state, MoveCard *redMoveCards, MoveCard *blueMoveCards) {
/**
 * Evaluates the game state for the current player.
 * This function evaluates the game state for the current player and returns a score that represents the advantage
 * of the current player over their opponent. The higher the score, the better the position is for the current player.
 * The evaluation function considers factors such as the number of pieces remaining, the safety of each piece, and
 * the possibility of winning by Way of the Stream.

 * @param state The current GameState object representing the board and game state.
 * @param redMoveCards A pointer to an array of red player's MoveCards.
 * @param blueMoveCards A pointer to an array of blue player's MoveCards.
 * @return An integer representing the advantage of the current player over their opponent.
 */

    int score = 0;

    for (int y = 0; y < BOARD_SIZE; ++y) {
        for (int x = 0; x < BOARD_SIZE; ++x) {
            Piece piece = state.board[y][x];
            if (piece == RED_STUDENT || piece == RED_MASTER) {
                score += (piece == RED_MASTER) ? 10 : 1;

                // Check if the piece is not in a position to be captured on the next turn
                for (size_t i = 0; i < 2; ++i) {
                    MoveCard moveCard = blueMoveCards[i];
                    bool safe = true;
                    for (int i = 0; i < moveCard.numMoves; ++i) {
                        int nx = x - moveCard.dx[i];
                        int ny = y - moveCard.dy[i];
                        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                            if (state.board[nx][ny] == BLUE_STUDENT || state.board[nx][ny] == BLUE_MASTER) {
                                safe = false;
                                break;
                            }
                        }
                    }
                    if (safe) {
                        score += (piece == RED_MASTER) ? 10 : 2;
                    }
                }
            } else if (piece == BLUE_STUDENT || piece == BLUE_MASTER) {
                score -= (piece == BLUE_MASTER) ? 10 : 1;

                // Check if the piece is not in a position to be captured on the next turn
                for (size_t i = 0; i < 2; ++i) {
                    MoveCard moveCard = redMoveCards[i];
                    bool safe = true;
                    for (int i = 0; i < moveCard.numMoves; ++i) {
                        int nx = x + moveCard.dx[i];
                        int ny = y + moveCard.dy[i];
                        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE) {
                            if (state.board[nx][ny] == RED_STUDENT || state.board[nx][ny] == RED_MASTER) {
                                safe = false;
                                break;
                            }
                        }
                    }
                    if (safe) {
                        score -= (piece == BLUE_MASTER) ? 10 : 2;
                    }
                }
            }
        }
    }

    return state.currentPlayer == RED ? score : -score;
}

int miniMaxAlphaBeta(GameState &state, int depth, int alpha, int beta, bool maximizingPlayer, Move &bestMove, MoveCard *redMoveCards, MoveCard *blueMoveCards) {
/**
 * MiniMax algorithm implementation with Alpha-Beta pruning for the Onitama board game.
 * This function performs a depth-limited search using the MiniMax algorithm and Alpha-Beta pruning
 * to find the best move for the current player in the given game state. The search depth can be
 * adjusted to control the complexity and performance of the algorithm.

 * @param state The current game state.
 * @param depth The remaining search depth for the algorithm.
 * @param alpha The current best value for the maximizing player.
 * @param beta The current best value for the minimizing player.
 * @param maximizingPlayer A boolean value, true if the current player is maximizing, false if minimizing.
 * @param bestMove A reference to a Move object, which will store the best move found by the algorithm.
 * @param redMoveCards A pointer to an array of MoveCard objects for the red player.
 * @param blueMoveCards A pointer to an array of MoveCard objects for the blue player.
 * @return The evaluation score of the best move found.
 */

    if (depth == 0 || state.winner != NONE) {
        return evaluate(state, redMoveCards, blueMoveCards);;
    }

    if (maximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto &move : state.legalMoves) {
            GameState nextState = state;
            applyMove(nextState, move, redMoveCards, blueMoveCards);
            Move dummyMove;
            int eval = miniMaxAlphaBeta(nextState, depth - 1, alpha, beta, false, dummyMove, redMoveCards, blueMoveCards);
            if (eval > maxEval) {
                maxEval = eval;
                bestMove = move;
            }
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto &move : state.legalMoves) {
            GameState nextState = state;
            applyMove(nextState, move, redMoveCards, blueMoveCards);
            Move dummyMove;
            int eval = miniMaxAlphaBeta(nextState, depth - 1, alpha, beta, true, dummyMove, redMoveCards, blueMoveCards);
            if (eval < minEval) {
                minEval = eval;
                bestMove = move;
            }
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

