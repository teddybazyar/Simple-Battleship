/*
Name: Teddy Bazyar
Date: April 23, 2025
Filename: main.c
Purpose: Battleship main file
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "battleship.h"

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

int main (void) {
    Stats *players = (Stats *)calloc(2, sizeof(Stats)); // dynamic memory for players
    if (!players) {
        printf("Memory allocation failed for players.\n");
        exit(EXIT_FAILURE);
    }

    // game boards
    Cell **user = (Cell **)malloc(ROWS * sizeof(Cell *));
    Cell **computer = (Cell **)malloc(ROWS * sizeof(Cell *));
    if (!user || !computer) {
        printf("Memory allocation failed for game boards.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < ROWS; i++) {
        user[i] = (Cell *)malloc(COLS * sizeof(Cell));
        computer[i] = (Cell *)malloc(COLS * sizeof(Cell));
    }

    // coordinates
    Coordinate *target = (Coordinate *)malloc(sizeof(Coordinate));
    Coordinate *targetTemp = (Coordinate *)malloc(sizeof(Coordinate));
    Coordinate *targetOrigin = (Coordinate *)malloc(sizeof(Coordinate));
    Coordinate *targetAI = (Coordinate *)malloc(sizeof(Coordinate));

    // ships
    WaterCraft ship[NUM_OF_SHIPS] = {
        {'C', 5, "Carrier"},
        {'B', 4, "Battleship"},
        {'R', 3, "Cruiser"},
        {'S', 3, "Submarine"},
        {'D', 2, "Destroyer"}
    };

    // booleans
    Boolean huntMode = TRUE;
    Boolean targetMode = FALSE;
    Boolean flipper = TRUE;
    Boolean cardinals[4] = {TRUE, TRUE, TRUE, TRUE};
    Boolean hasAShipSunked = FALSE;

    // sunked ships
    short sunkShip[2][NUM_OF_SHIPS] = {{5, 4, 3, 3, 2}, {5, 4, 3, 3, 2}};

    // variables
    short player = 0;
    short shot = 0;
    int option = 0;
    int north = 0, south = 0, east = 0, west = 0;
    int counter = 1;
    int i;
    char shipSymbol = '\0';
    FILE *outStream = NULL;

    outStream = fopen(LOG_FILE_NAME, "w");
    if (!outStream) {
        printf("Error opening log file.\n");
        exit(EXIT_FAILURE);
    }

    srand((unsigned int) time (NULL));

    welcomeScreen();
    systemMessage("\t   Hit <ENTER> to continue!\n");
    system(CLEAR_SCREEN);

    initializeGameBoard(user);
    initializeGameBoard(computer);

    // smart AI mode
    int aiMode = 0;
    printf("> Choose AI mode:\n");
    printf("> [1] Regular AI (Random)\n");
    printf("> [2] Smart AI (Probability Grid)\n");
    printf("> Enter option: ");
    scanf("%d", &aiMode);

    int probability[ROWS][COLS] = {0};

    printf("\n> Please select from the following menu:\n");
    printf("> [1] Manually\n");
    printf("> [2] Randomly\n");
    printf("> Enter Option: ");

    int valid = 0;
    do {
        scanf("%d", &option);
        switch (option) {
            case 1:
                manuallyPlaceShipsOnGameBoard(user, ship);
                valid = 1;
                break;
            case 2:
                randomlyPlaceShipsOnGameBoard(user, ship);
                valid = 1;
                break;
            default:
                printf("ERROR: invalid input, try again.\n");
                break;
        }
    } while (!valid);

    randomlyPlaceShipsOnGameBoard(computer, ship);
    printf("> Player 2 (Computer's) board has been generated.\n");

    player = getRandomNumber(0, 1);
    printf("> Player %d has been randomly selected to go first.\n", player + 1);
    systemMessage("> Hit <ENTER> to continue!\n");
    system(CLEAR_SCREEN);

    while (TRUE) {
        fprintf(outStream, "Player %d's turn.\n", player + 1);

        switch(player) {
            case PLAYER_1:
                printf("> Player 2's Board:\n");
                printGameBoard(computer, FALSE);
                printf("> PLAYER 1'S TURN\n");

                do {
                    *target = getTarget();
                    shot = checkShot(computer, *target);
                    if (shot == -1)
                        printf("> Try inputting another target!\n");
                } while (shot == -1);

                shipSymbol = computer[target->row][target->column].symbol;
                break;

            case PLAYER_2:
                printf("> Player 1's Board:\n");
                printGameBoard(user, TRUE);
                printf("> COMPUTER'S TURN\n");
            
                if (aiMode == 1) {
                    // === Regular AI Mode ===
                    if (hasAShipSunked) {
                        hasAShipSunked = FALSE;
                        targetMode = FALSE;
                        huntMode = TRUE;
                    }
            
                    if (targetMode) {
                        *target = *targetAI;
                        do {
                            *targetTemp = *target;
                            if (cardinals[NORTH]) {
                                north = target->row - counter;
                                if (north >= 0) {
                                    targetTemp->row = north;
                                    shot = checkShot(user, *targetTemp);
                                    if (shot != -1) break;
                                }
                            }
                            if (cardinals[SOUTH]) {
                                south = target->row + counter;
                                if (south <= 9) {
                                    targetTemp->row = south;
                                    shot = checkShot(user, *targetTemp);
                                    if (shot != -1) break;
                                }
                            }
                            if (cardinals[WEST]) {
                                west = target->column - counter;
                                if (west >= 0) {
                                    targetTemp->column = west;
                                    shot = checkShot(user, *targetTemp);
                                    if (shot != -1) break;
                                }
                            }
                            if (cardinals[EAST]) {
                                east = target->column + counter;
                                if (east <= 9) {
                                    targetTemp->column = east;
                                    shot = checkShot(user, *targetTemp);
                                    if (shot != -1) break;
                                }
                            }
            
                            counter++;
                            shot = -1;
                            if (!cardinals[NORTH] && !cardinals[SOUTH] && !cardinals[WEST] && !cardinals[EAST]) {
                                targetMode = FALSE;
                                huntMode = TRUE;
                                break;
                            }
                        } while (shot == -1 && targetMode);
            
                        if (shot == 1) {
                            for (i = 0; i < 4; i++)
                                cardinals[i] = FALSE;
                        }
                    }
            
                    if (huntMode) {
                        counter = 1;
                        flipper = TRUE;
                        for (i = 0; i < 4; i++) cardinals[i] = TRUE;
            
                        do {
                            target->row = getRandomNumber(0, 9);
                            target->column = getRandomNumber(0, 9);
                            shot = checkShot(user, *target);
                        } while (shot == -1);
            
                        if (shot == 1)
                            *targetOrigin = *target;
                    }
            
                    if (shot == 1) {
                        if (!cardinals[NORTH] && !cardinals[SOUTH] && !cardinals[WEST] && !cardinals[EAST])
                            *target = *targetOrigin;
            
                        huntMode = FALSE;
                        targetMode = TRUE;
                        *targetAI = *target;
                    }
                }
                else if (aiMode == 2) {
                    // === Smart AI Mode (Probability Grid) ===
                    int probability[ROWS][COLS] = {0};
            
                    for (int r = 0; r < ROWS; r++) {
                        for (int c = 0; c < COLS; c++) {
                            if (user[r][c].isHit) continue;
            
                            for (int s = 0; s < NUM_OF_SHIPS; s++) {
                                int len = ship[s].length;
            
                                // Check horizontal placement
                                if (c + len <= COLS) {
                                    int valid = 1;
                                    for (int k = 0; k < len; k++) {
                                        if (user[r][c + k].isHit) {
                                            valid = 0;
                                            break;
                                        }
                                    }
                                    if (valid) for (int k = 0; k < len; k++) probability[r][c + k]++;
                                }
            
                                // Check vertical placement
                                if (r + len <= ROWS) {
                                    int valid = 1;
                                    for (int k = 0; k < len; k++) {
                                        if (user[r + k][c].isHit) {
                                            valid = 0;
                                            break;
                                        }
                                    }
                                    if (valid) for (int k = 0; k < len; k++) probability[r + k][c]++;
                                }
                            }
                        }
                    }
            
                    // Prefer cells next to a known hit
                    for (int r = 0; r < ROWS; r++) {
                        for (int c = 0; c < COLS; c++) {
                            if (!user[r][c].isHit && (
                                    (r > 0     && user[r - 1][c].isHit && user[r - 1][c].symbol != WATER) ||
                                    (r < 9     && user[r + 1][c].isHit && user[r + 1][c].symbol != WATER) ||
                                    (c > 0     && user[r][c - 1].isHit && user[r][c - 1].symbol != WATER) ||
                                    (c < 9     && user[r][c + 1].isHit && user[r][c + 1].symbol != WATER))) {
                                probability[r][c] += 10; // Boost around hits
                            }
                        }
                    }
            
                    int maxProb = -1;
                    for (int r = 0; r < ROWS; r++) {
                        for (int c = 0; c < COLS; c++) {
                            if (!user[r][c].isHit && probability[r][c] > maxProb) {
                                maxProb = probability[r][c];
                                target->row = r;
                                target->column = c;
                            }
                        }
                    }
            
                    shot = checkShot(user, *target);
                }
            
                shipSymbol = user[target->row][target->column].symbol;
                break;
        }

        if (shot == 1) {
            printf("> %d, %d is a hit!\n", target->row, target->column);
            fprintf(outStream, "%d, %d is a hit!\n", target->row, target->column);
            players[player].numHits++;
            if (player == 1)
                hasAShipSunked = checkSunkShip(sunkShip, !player, shipSymbol, outStream);
            else
                checkSunkShip(sunkShip, !player, shipSymbol, outStream);
        } else {
            printf("> %d, %d is a miss!\n", target->row, target->column);
            fprintf(outStream, "%d, %d is a miss!\n", target->row, target->column);
            players[player].numMisses++;
        }

        if (player == 0)
            updateGameBoard(computer, *target);
        else
            updateGameBoard(user, *target);

        if (isWinner(players, player)) {
            printf("\n> Player %d wins!\n", player + 1);
            fprintf(outStream, "\n>>>> Player %d wins! <<<<\n", player + 1);
            break;
        }

        systemMessage("> Hit <ENTER> to continue!\n");
        player = !player;
        system(CLEAR_SCREEN);
    }

    players[0].totalShots = players[0].numHits + players[0].numMisses;
    players[0].hitMissRatio = players[0].numMisses == 0 ? 100.0 : ((double) players[0].numHits / players[0].numMisses) * 100;
    players[1].totalShots = players[1].numHits + players[1].numMisses;
    players[1].hitMissRatio = players[1].numMisses == 0 ? 100.0 : ((double) players[1].numHits / players[1].numMisses) * 100;

    fprintf(outStream, "+===================================================\n");
    fprintf(outStream, "|                    PLAYER STATS                   \n");
    fprintf(outStream, "+---------------------------------------------------\n");
    fprintf(outStream, "| PLAYER 1 : %d hits                                \n", players[0].numHits);
    fprintf(outStream, "|            %d misses                              \n", players[0].numMisses);
    fprintf(outStream, "|            %d total shots                         \n", players[0].totalShots);
    fprintf(outStream, "|            %.2lf%% hit/miss ratio                 \n", players[0].hitMissRatio);
    fprintf(outStream, "| PLAYER 2 : %d hits                                \n", players[1].numHits);
    fprintf(outStream, "|            %d misses                              \n", players[1].numMisses);
    fprintf(outStream, "|            %d total shots                         \n", players[1].totalShots);
    fprintf(outStream, "|            %.2lf%% hit/miss ratio                 \n", players[1].hitMissRatio);
    fprintf(outStream, "+===================================================");

    fclose(outStream);

    // Free memory
    free(players);
    for (int i = 0; i < ROWS; i++) {
        free(user[i]);
        free(computer[i]);
    }
    free(user);
    free(computer);
    free(target);
    free(targetTemp);
    free(targetOrigin);
    free(targetAI);

    return 0;
}