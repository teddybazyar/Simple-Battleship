/*
Name: Teddy Bazyar
Date: April 23, 2025
Filename: battleship.c
Purpose: Function definitions for Battleship
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "battleship.h"

// Display welcome screen
void welcomeScreen(void) {
    printf("          ~~~ Welcome to Battleship! ~~~\n\n");
}

// Display a system message and wait for user to continue
void systemMessage(const char *message) {
    printf("%s", message);
    getchar();
    getchar();
}

// Initialize the game board with water symbols and not hit
void initializeGameBoard(Cell **board) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            board[row][col].symbol = WATER;
            board[row][col].isHit = FALSE;
        }
    }
}

// Print the game board, optionally revealing ships
void printGameBoard(Cell **board, Boolean revealShips) {
    printf("    ");
    for (int col = 0; col < COLS; col++) {
        printf("%d ", col);
    }
    printf("\n");

    for (int row = 0; row < ROWS; row++) {
        printf("%d | ", row);
        for (int col = 0; col < COLS; col++) {
            if (revealShips || board[row][col].symbol == WATER || board[row][col].isHit) {
                printf("%c ", board[row][col].symbol);
            } else {
                printf("%c ", WATER);
            }
        }
        printf("\n");
    }
}

// Allow the player to manually place ships on the board
void manuallyPlaceShipsOnGameBoard(Cell **board, WaterCraft *ship) {
    int row, col, direction;
    for (int i = 0; i < NUM_OF_SHIPS; i++) {
        printf("> Placing %s (%d cells)\n", ship[i].name, ship[i].length);

        Boolean valid = FALSE;
        while (!valid) {
            printf("> Enter row, column, and direction (0=horizontal, 1=vertical): ");
            scanf("%d%d%d", &row, &col, &direction);

            if (isValidPlacement(board, row, col, direction, ship[i].length)) {
                placeShip(board, row, col, direction, ship[i]);
                valid = TRUE;
            } else {
                printf("Invalid placement. Try again.\n");
            }
        }
    }
}

// Randomly place ships on the board
void randomlyPlaceShipsOnGameBoard(Cell **board, WaterCraft *ship) {
    int row, col, direction;

    for (int i = 0; i < NUM_OF_SHIPS; i++) {
        Boolean placed = FALSE;
        while (!placed) {
            row = getRandomNumber(0, 9);
            col = getRandomNumber(0, 9);
            direction = getRandomNumber(0, 1);

            if (isValidPlacement(board, row, col, direction, ship[i].length)) {
                placeShip(board, row, col, direction, ship[i]);
                placed = TRUE;
            }
        }
    }
}

// Get target coordinates from player
Coordinate getTarget(void) {
    Coordinate target;
    printf("> Enter Target (row column): ");
    scanf("%d%d", &target.row, &target.column);
    return target;
}

// Check if a shot is a hit, miss, or invalid
int checkShot(Cell **board, Coordinate target) {
    if (target.row < 0 || target.row >= ROWS || target.column < 0 || target.column >= COLS)
        return -1; // invalid input

    if (board[target.row][target.column].isHit)
        return -1; // already targeted

    if (board[target.row][target.column].symbol != WATER)
        return 1; // hit

    return 0; // miss
}

// Update the game board after a shot
void updateGameBoard(Cell **board, Coordinate target) {
    board[target.row][target.column].isHit = TRUE;
    if (board[target.row][target.column].symbol == WATER) {
        board[target.row][target.column].symbol = MISS;
    } else {
        board[target.row][target.column].symbol = HIT;
    }
}

// Check if a ship has been sunk
short checkSunkShip(short sunkShip[2][NUM_OF_SHIPS], short player, char shipSymbol, FILE *outStream) {
    short shipIndex = -1;

    switch (shipSymbol) {
        case 'C': shipIndex = 0; break;
        case 'B': shipIndex = 1; break;
        case 'R': shipIndex = 2; break;
        case 'S': shipIndex = 3; break;
        case 'D': shipIndex = 4; break;
    }

    if (shipIndex != -1) {
        sunkShip[player][shipIndex]--;

        if (sunkShip[player][shipIndex] == 0) {
            fprintf(outStream, "> %s has been sunk!\n", getShipName(shipSymbol));
            printf("> %s has been sunk!\n", getShipName(shipSymbol));
            return 1;
        }
    }

    return 0;
}

// Check if the player has won
Boolean isWinner(Stats *players, short player) {
    return (players[player].numHits == 17);
}

// Get ship name based on symbol
char* getShipName(char symbol) {
    switch (symbol) {
        case 'C': return "Carrier";
        case 'B': return "Battleship";
        case 'R': return "Cruiser";
        case 'S': return "Submarine";
        case 'D': return "Destroyer";
        default: return "Unknown Ship";
    }
}

// Generate a random number between lower and upper (inclusive)
int getRandomNumber(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

// Check if placement of a ship is valid
Boolean isValidPlacement(Cell **board, int row, int col, int direction, int length) {
    if (direction == 0) { // horizontal
        if (col + length > COLS)
            return FALSE;
        for (int i = 0; i < length; i++) {
            if (board[row][col + i].symbol != WATER)
                return FALSE;
        }
    } else { // vertical
        if (row + length > ROWS)
            return FALSE;
        for (int i = 0; i < length; i++) {
            if (board[row + i][col].symbol != WATER)
                return FALSE;
        }
    }
    return TRUE;
}

// Place a ship on the board
void placeShip(Cell **board, int row, int col, int direction, WaterCraft ship) {
    if (direction == 0) { // horizontal
        for (int i = 0; i < ship.length; i++) {
            board[row][col + i].symbol = ship.symbol;
        }
    } else { // vertical
        for (int i = 0; i < ship.length; i++) {
            board[row + i][col].symbol = ship.symbol;
        }
    }
}