# Onitama-AI

## Introduction
Onitama is a two-player abstract board game that originated in Japan. This code implements an Onitama game algorithm using the MiniMax algorithm with alpha-beta pruning to search for the best move. Onitama is a game where two players try to capture each other's master piece or move their master piece into the opponent's temple. Each player starts with five pieces: one master and four students. Each turn, a player can move one of their pieces using a move card. The move cards specify which piece can move and how it can move. The game ends when one player captures the opponent's master piece or moves their own master piece into the opponent's temple.

## Code Breakdown
The code is written in C++, and it uses several data structures and algorithms. The code begins by defining the size of the board, the maximum search depth for the MiniMax algorithm, and the structure for the move cards. The move cards define the possible moves a player can make on a turn. There are sixteen move cards in total, each with a unique name, movement pattern, and number of moves.

The code defines the game state, which includes the board, legal moves, the current player, and the winner. The board is a 5x5 grid that represents the game board. The legal moves are the possible moves a player can make on their turn. The current player is the player whose turn it is, and the winner is the player who wins the game.

The code implements several functions, including isMoveValid, generateLegalMoves, evaluate, and applyMove. The isMoveValid function checks if a move is valid. The generateLegalMoves function generates all possible legal moves for the current player. The evaluate function evaluates the game state and returns a score. The applyMove function applies a move to the game state.

The main function initializes the game state, including the board, move cards for both players, and the current player. It then loops through the game, searching for the best move using the MiniMax algorithm with alpha-beta pruning. Once the best move is found, it is applied to the game state, and the move cards for the current player are updated. The current player is then updated, and the loop continues until a winner is determined.

NOTE: If the code runs an infinite loop just run it again until it doesn't. I am fixing this.

## Conclusion
This code implements the Onitama game algorithm using the MiniMax algorithm with alpha-beta pruning. It is a complex game that requires strategic thinking and planning to win. The code is well-written and easy to understand, making it an excellent resource for anyone looking to learn more about game programming or the MiniMax algorithm. 

I am working to add functionality to allow players to play against an AI, which would make it even more interesting and engaging.
