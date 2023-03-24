# Onitama-AI

## Introduction
Onitama is a two-player abstract board game that originated in Japan. This code implements an Onitama game algorithm using the MiniMax algorithm with alpha-beta pruning to search for the best move. Onitama is a game where two players try to capture each other's master piece or move their master piece into the opponent's temple. Each player starts with five pieces: one master and four students. Each turn, a player can move one of their pieces using a move card. The move cards specify which piece can move and how it can move. The game ends when one player captures the opponent's master piece or moves their own master piece into the opponent's temple.

## Game Board

<img src="https://i.imgur.com/xmgOsv6.png" width=50% height=50%>

## Move Cards

<img src="https://i.imgur.com/c850hXb.png" width=50% height=50%>

## Code Breakdown
Include statements
* The code includes necessary libraries, such as iostream, vector, limits, algorithm, ctime, string, random, and chrono.

Constants
* The code defines constants for board size, number of cards in the game, number of cards per player, and the depth of the search.

Enums
* Enums are defined for the different players and game pieces, to improve code readability and reduce errors.

Game state representation
* The board state is represented as a 2D character array, with 'r' and 'b' representing the two player's students, and 'R' and 'B' representing their masters. The cards are represented as a string, with their moves being a vector of pairs of integers.

Utility functions
* Several utility functions are implemented, such as printing the board, converting strings to lower or upper case, and finding the master's position for each player.

Move validation
* Functions for validating moves are implemented, such as checking whether a move is within the board's boundaries, whether a square is occupied, or whether a move is valid for a given card.

Game state manipulation
* Functions for applying and undoing moves are implemented, updating the game state accordingly. These are essential for traversing the game tree during the search process.

Game outcome evaluation
* A function to evaluate whether a player has won, lost, or if the game is a draw is implemented. It checks for both conditions of winning, i.e., capturing the opponent's master or moving one's own master to the opponent's temple.

Alpha-Beta pruning algorithm
* The core of the AI's decision-making process, the Alpha-Beta pruning algorithm, is implemented as a recursive function. It efficiently searches the game tree by    pruning branches that will not result in better outcomes, reducing the search space and speeding up computation. The function also takes depth into account, allowing for a configurable level of lookahead.

Main function
* The main function initializes the game state and runs the AI. It takes turns for each player, printing the board, reading and applying moves, and checking for the game's end. It also demonstrates the AI's ability to play against itself by simulating both players as AI agents.

Here is an example of a running state of the board:
```
Tiger Goose
_ r R r r 
r _ _ _ _ 
_ _ _ _ _ 
_ B _ _ _ 
b b _ b b 
Frog Ox
```

## Future Improvements Checklist

Add functionality to allow players to play against the AI 

Improve evaluation function.
* more points for a protected master, weight 7
* more points for control of the center of the board, weight 5
* more points for a master closer to the targeted temple, weight 2

In the real game you can see the next card that will fill your hand. You can see the opponents as well. This is something that needs to be added. 

Make a visually appealing GUI

