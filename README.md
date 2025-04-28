# Simple-Battleship

This simple Battleship game was created using the C language. The user will be going against the computer (PLAYER 2). 
## Objective: 
    The goal is to sink all of your opponent’s ships before they sink yours.
## Setup: Each player places ships on a hidden grid. 
  Ships vary in length and are placed either horizontally or vertically.
## Gameplay: Players take turns calling out grid coordinates (like "B4" or "H7") to try and hit the opponent’s ships.
## Hit or Miss: If the coordinate matches a part of a ship, it’s a "hit"; otherwise, it’s a "miss." 
  Players mark their hits and misses on a separate tracking grid.
## Sinking Ships: When all the coordinates of a ship have been hit, the ship is considered sunk. The first player to sink all enemy ships wins.

-------------------------------------------------------------------------------------------------------------------------------------------------------------

* MAIN.C file includes main area where most of the game will function.

* BATTLESHIP.C file contains different game prototype functions, such as checking if there's a winner, initializing game board, updating game board, etc.
  
* BATTLESHIP.H file contains type structures and different function prototypes.
  
* BATTLESHIP.LOG file contains the log statements of the different moves each player made. A formal stats chart will be printed by the end when a player
                wins upon the log file.
