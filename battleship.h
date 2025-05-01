/*
Name: Teddy Bazyar
Date: April 23, 2025
Filename: battleship.h
Purpose: Battleship h file that contains function prototypes, 
        structures, and macros to run the game.
*/

#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// constant grid settings
#define ROWS 10
#define COLS 10
#define NUM_OF_SHIPS 5

// coordinate constants
#define HORIZONTAL 0
#define VERTICAL 1
#define PLAYER_1 0
#define PLAYER_2 1

// log for the game
#define LOG_FILE_NAME "battleship.log"

// grid symbols
#define WATER '~'
#define HIT 'x'
#define MISS 'O'
#define CARRIER 'C'
#define BATTLESHIP 'B'
#define CRUISER 'R'
#define SUBMARINE 'S'
#define DESTROYER 'D'

// direction constant
#define NORTH 0
#define SOUTH 1
#define WEST 2
#define EAST 3

// colors
#define RESET       "\x1b[0m"
#define RED         "\x1b[31m"
#define GREEN       "\x1b[32m"
#define BLUE        "\x1b[34m"
#define MAGENTA     "\x1b[35m"
#define CYAN        "\x1b[36m"
#define BOLD        "\x1b[1m"
#define WHITE       "\x1b[37m"

// Boolean values
typedef enum {
    FALSE, TRUE
} Boolean;

// struct types

typedef struct watercraft {
    char symbol;
    short length;
    char *name;
} WaterCraft;

typedef struct stats {
    int numHits;
    int numMisses;
    int totalShots;
    double hitMissRatio;
} Stats;

typedef struct coordinate {
    int row;
    int column;
} Coordinate;

typedef struct cell {
    char symbol;
    Boolean isHit;
} Cell;

// Function prototypes

// void
void welcomeScreen(void);
void systemMessage(const char *message);
void initializeGameBoard(Cell **board);
void printGameBoard(Cell **board, Boolean revealShips);
void manuallyPlaceShipsOnGameBoard(Cell **board, WaterCraft *ship);
void randomlyPlaceShipsOnGameBoard(Cell **board, WaterCraft *ship);
Coordinate getTarget(void);
int checkShot(Cell **board, Coordinate target);
void updateGameBoard(Cell **board, Coordinate target);
short checkSunkShip(short sunkShip[2][NUM_OF_SHIPS], short player, char shipSymbol, FILE *outStream);
Boolean isWinner(Stats *players, short player);
char* getShipName(char symbol);
int getRandomNumber(int lower, int upper);
Boolean isValidPlacement(Cell **board, int row, int col, int direction, int length);
void placeShip(Cell **board, int row, int col, int direction, WaterCraft ship);

#endif