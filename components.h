#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <iostream>
using namespace std;

#include <string>
#include <vector>

const int BOARD_SIZE = 5;
const int MAX_DEPTH = 5;

struct MoveCard {
    string name;
    int dx[4];
    int dy[4];
    int numMoves;
};

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

vector<MoveCard> Deck = {Tiger, Dragon, Frog, Rabbit, Crab, Elephant, Goose, Rooster, Monkey, Mantis, Horse, Ox, Crane, Boar, Eel, Cobra};

enum Piece { EMPTY, RED_MASTER, RED_STUDENT, BLUE_MASTER, BLUE_STUDENT };
enum Player { RED, BLUE, NONE };

struct Move {
    Piece usedPiece;
    MoveCard usedCard;
    int x1, y1, x2, y2;
};

struct GameState {
    vector< vector<Piece>> board;
    vector<Move> redLegalMoves;
    vector<Move> blueLegalMoves;
    Player currentPlayer;
    Player winner;
};

// overload equal operator to compare names
bool operator==(const MoveCard& lhs, const MoveCard& rhs) {
    return lhs.name == rhs.name;
}

// bool operator==(string lhs, const MoveCard& rhs) {
//     return lhs == rhs.name;
// }

ostream& operator<<( ostream &os, const MoveCard &moveCard) {
    os << moveCard.name;
    return os;
}

template <typename RandomEngine>
void generateUniqueRandomIndices(int range, vector<int> &randomIndices, RandomEngine& random_engine) {
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

    shuffle(randomIndices.begin(), randomIndices.end(), random_engine);
}

#endif // COMPONENTS_H