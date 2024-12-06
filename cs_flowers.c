// cs_flowers
//
// This program was written by Ryan Tchan z5258155
// on 17/3/24
//
// This is a C program called CS Flowers that creates a 2D array map as a game
// board on which the player will move around the map and defeat the enemy 
// flowers whilst avoiding traps and their attacks

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

#define MAX_ROW 11
#define MAX_COL 11
#define TRUE 1
#define FALSE 0

// TODO: put your constants here 

//------------------------------------------------------------------------------
// Struct & Enum
//------------------------------------------------------------------------------

enum flower_state {
    NONE,
    DORMANT,
    DISTURBED,
    AWAKENED,
    ANXIOUS,
    ENERGETIC
};

struct flower {
    enum flower_state state;
    // TODO: You will need to add more to this struct in Stage 3
    int flwr_row;
    int flwr_col;
    int move;
    int bush_placed;
};

enum tile_type {
    EMPTY,
    BUSH,
    BRANCH,
    DANGER,
    ROOT,
    POLLEN,
    SEEDLING,
    FLOWER,
};

struct tile {
    enum tile_type type;
    struct flower flower;
};

// TODO: Put your structs here 
struct player {
    int row;
    int col;
};


//------------------------------------------------------------------------------
// Your Function Prototypes
//------------------------------------------------------------------------------

// TODO: Put your function prototypes here
void print_foliage(struct tile map[MAX_ROW][MAX_COL], 
    char branch_command,
    int branch_row, 
    int branch_col,
    int branch_count,
    char bush_direction,
    int bush_length);

void place_flower(struct tile map[MAX_ROW][MAX_COL],
    int flwr_col,
    int flwr_row);

void place_flower_manual(struct tile map[MAX_ROW][MAX_COL],
    int flower_count);

void place_flower_auto(struct tile map[MAX_ROW][MAX_COL]);

void player_cut(struct tile map[MAX_ROW][MAX_COL],
    struct player *player,
    char move_cmd,
    char cut_direction);

void player_move(struct tile map[MAX_ROW][MAX_COL],
    struct player *player,
    char move_cmd);

void alert_grid(struct tile map[MAX_ROW][MAX_COL],
    struct player *player);

void change_flower_state(struct tile map[MAX_ROW][MAX_COL],
    struct player *player,
    int flower_row,
    int flower_col);

int win_condition(struct tile map[MAX_ROW][MAX_COL],
    struct player *player,
    int flower_count);
//------------------------------------------------------------------------------
// Provided Function Prototypes
//------------------------------------------------------------------------------

void initialise_map(struct tile map[MAX_ROW][MAX_COL]);
void print_map(
    struct tile map[MAX_ROW][MAX_COL], 
    int player_row, 
    int player_col
);

void print_tile(struct tile tile);
void print_flower(struct flower flower);

//------------------------------------------------------------------------------
// Main Function
//------------------------------------------------------------------------------

int main(void) {
    struct tile map[MAX_ROW][MAX_COL];
    initialise_map(map);

    printf("Welcome to CS Flowers!\n");

    // TODO: Start writing code here!
    int player_row;
    int player_col;

    // TODO: Stage 1.1 - Scan in the player position and print out the map
    printf("Player's starting position: ");
    scanf(" %d %d", &player_row, &player_col);
    
    // TODO: Stage 1.2 - Validate the player's spawn and re-scan if invalid
    while ((player_row > 0 && player_col > 0) && 
        (player_row < MAX_ROW - 1 && player_col < MAX_COL - 1)) {

        printf("Invalid starting position!\n");
        printf("Re-enter starting position: ");
        scanf(" %d %d", &player_row, &player_col);      
    }

    print_map(map, player_row, player_col);

    // TODO: Stage 1.3 - Add foliage onto the map, starting with branches
    printf("How many tiles of foliage: ");
    int foliage_count;
    scanf(" %d", &foliage_count);

    char foliage_command;
    int foliage_row;
    int foliage_col;
    char bush_direction;
    int bush_length;

    // seperate scanf for branch and bush and use print_foliage function till
    // while loop completes
    int i = 0;
    while (i < foliage_count) {
        scanf(" %c", &foliage_command);
        
        if (foliage_command == 'b') {
            scanf(" %d %d", &foliage_row, &foliage_col);
        } else if (foliage_command == 'u') {
            scanf(" %d %d %c %d", &foliage_row, &foliage_col,
            &bush_direction, &bush_length);
        }

        print_foliage(map, foliage_command, foliage_row, 
            foliage_col, foliage_count, bush_direction, bush_length);
        i++;
    }

    print_map(map, player_row, player_col);

    // Stage 2.1 - Spawn flowers
    printf("How many flowers: ");
    int flower_count;
    scanf(" %d", &flower_count);
    
    if (flower_count >= 25) {
        place_flower_auto(map);
    } else {
        place_flower_manual(map, flower_count);
    }

    print_map(map, player_row, player_col);

    // Stage 2.2 onwards Command loop + player movement
    printf("Game Started!\n");

    struct player *player;
    player = (struct player *) malloc(sizeof(struct player)); 
    if (player == NULL) {
        return -1;
    }
    player->row = player_row;
    player->col = player_col; 

    struct flower *flower;
    flower = (struct flower *) malloc(sizeof(struct flower)); 
    if (flower == NULL) {
        return -1;
    }
    flower->move = 0;

    char move_cmd;
    char cut_direction;

    int win_con = 0;
    printf("Enter command: ");
    while ((scanf(" %c", &move_cmd) == 1) && (win_con == 0)) {

        if (move_cmd == 'c') {
            scanf(" %c", &cut_direction);
            player_cut(map, player, move_cmd, cut_direction);
            print_map(map, player->row, player->col);
        } else if ((move_cmd == 'w') || (move_cmd == 'a') ||
            (move_cmd == 's') || (move_cmd == 'd') || (move_cmd == 'i'))  {
            player_move(map, player, move_cmd);
            print_map(map, player->row, player->col);

            
        } 
        
        win_con = win_condition(map, player, flower_count);
    }
    //player_move(map, player_row, player_col);
 
    return 0;
}

//------------------------------------------------------------------------------
// Your Function
//------------------------------------------------------------------------------

// TODO: Put your functions here

// Function that places the inputed amount of folliage onto the map
// 1. if statement for branches
// 2. else if statement for bushes
// 3. else statement for invalid foliage 
void print_foliage(struct tile map[MAX_ROW][MAX_COL],
    char foliage_command,
    int foliage_row, 
    int foliage_col,
    int foliage_count,
    char bush_direction,
    int bush_length) {
    
    if ((foliage_row >= 1 && foliage_col >= 1) && 
        (foliage_row <= MAX_ROW - 2 && foliage_col <= MAX_COL - 2) && 
        (foliage_command == 'b')) {
        printf("Branch added!\n");
        map[foliage_row][foliage_col].type = BRANCH;
    } else if ((foliage_row >= 1 && foliage_col >= 1) && 
        (foliage_row <= MAX_ROW - 2 && foliage_col <= MAX_COL - 2) && 
        (foliage_command == 'u')) {
        
        if (bush_direction == 'v') {
            for (int i = 0; i < bush_length; i++) {
                map[foliage_row][foliage_col].type = BUSH;
                foliage_row++;
            }
        } else if (bush_direction == 'h') {
            for (int i = 0; i < bush_length; i++) {
                map[foliage_row][foliage_col].type = BUSH;
                foliage_col++;
            }
        } else {
            
        }
        printf("Bush added!\n"); 
    } else {
        printf("Invalid foliage position!\n");
    }
}

// Function that prints out flowers on odd tiles or voids if invalid or not EMPTY
void place_flower(struct tile map[MAX_ROW][MAX_COL],
    int flwr_row,
    int flwr_col) {
    
    if ((map[flwr_row][flwr_col].type != EMPTY) ||
        ((flwr_row % 2 == 0) || 
        (flwr_col % 2 == 0))) {
        
        printf("Invalid flower position!\n");
    } else {
        map[flwr_row][flwr_col].type = FLOWER;
        map[flwr_row][flwr_col].flower.state = DORMANT;
    }
}

void place_flower_auto(struct tile map[MAX_ROW][MAX_COL]) {
    for (int flwr_row = 1; flwr_row < MAX_ROW - 1; flwr_row = flwr_row + 2) {
        for (int flwr_col = 1; flwr_col < MAX_COL - 1; flwr_col = flwr_col + 2) {
            if (map[flwr_row][flwr_col].type == EMPTY) {
                map[flwr_row][flwr_col].type = FLOWER;
                map[flwr_row][flwr_col].flower.state = DORMANT;
            } 
        }
    }
}

void place_flower_manual(struct tile map[MAX_ROW][MAX_COL],
    int flower_count) {
    
    int counter = 0;
    int flwr_row;
    int flwr_col; 

    while ((counter < flower_count)) {
        scanf(" %d %d", &flwr_row, &flwr_col);

        if ((flwr_row > 0 && flwr_row <= MAX_ROW) &&
            (flwr_col > 0 && flwr_col <= MAX_COL)) {    
            place_flower(map, flwr_row, flwr_col);
        } else {
            printf("Invalid flower position!\n");
        }
        counter++;
    }        
}


// Function that cuts flowers/bushes and alerts for flowers cut
void player_cut(struct tile map[MAX_ROW][MAX_COL],
    struct player *player,
    char move_cmd,
    char cut_direction) {

    if ((cut_direction == 'w') && (player->row - 1 >= 0)) {
        player->row--;
        if (map[player->row][player->col].type == BUSH) {
            map[player->row][player->col].type = EMPTY;
        } else if (map[player->row][player->col].type == FLOWER) {
            alert_grid(map, player);
            map[player->row][player->col].type = EMPTY; 
        }
        player->row++;
    } else if ((cut_direction == 's') && (player->row + 1 < MAX_ROW)) {
        player->row++;
        if (map[player->row][player->col].type == BUSH) {
            map[player->row][player->col].type = EMPTY;
        } else if (map[player->row][player->col].type == FLOWER) {
            alert_grid(map, player);
            map[player->row][player->col].type = EMPTY;
        }
        player->row--;
    } else if ((cut_direction == 'a') && (player->col - 1 >= 0)) {
        player->col--;
        if (map[player->row][player->col].type == BUSH) {
            map[player->row][player->col].type = EMPTY;
        } else if (map[player->row][player->col].type == FLOWER) {
            alert_grid(map, player);
            map[player->row][player->col].type = EMPTY;
        }
        player->col++;
    } else if ((cut_direction == 'd') && (player->col + 1 < MAX_COL)) {
        player->col++;
        if (map[player->row][player->col].type == BUSH) {
            map[player->row][player->col].type = EMPTY;
        } else if (map[player->row][player->col].type == FLOWER) {
            alert_grid(map, player);
            map[player->row][player->col].type = EMPTY;
        } 
        player->col--;
    } 
}

//Player movement for w,a,s,d,i -> movement affected on tile types != EMPTY
void player_move(struct tile map[MAX_ROW][MAX_COL],
    struct player *player,
    char move_cmd) {   
 
    if ((move_cmd == 'w') && (player->row - 1 >= 0)) {
        player->row--;
        if (map[player->row][player->col].type == BRANCH) {
            alert_grid(map, player);
            map[player->row][player->col].type = EMPTY;                
        } else if ((map[player->row][player->col].type == BUSH) ||
            (map[player->row][player->col].type == FLOWER)) {
            player->row++; 
        }
    } else if ((move_cmd == 's') && (player->row + 1 < MAX_ROW)) {
        player->row++;
        if (map[player->row][player->col].type == BRANCH) {
            alert_grid(map, player);
            map[player->row][player->col].type = EMPTY;
        } else if ((map[player->row][player->col].type == BUSH) ||
            (map[player->row][player->col].type == FLOWER)) {
            player->row--;   
        }
    } else if ((move_cmd == 'a') && (player->col - 1 >= 0)) {
        player->col--;
        if (map[player->row][player->col].type == BRANCH) {
            alert_grid(map, player);
            map[player->row][player->col].type = EMPTY; 
        } else if ((map[player->row][player->col].type == BUSH) ||
            (map[player->row][player->col].type == FLOWER)) {
            player->col++;
        }
    } else if ((move_cmd == 'd') && (player->col + 1 < MAX_COL)) {
        player->col++;
        if (map[player->row][player->col].type == BRANCH) {
            alert_grid(map, player);
            map[player->row][player->col].type = EMPTY;
        } else if ((map[player->row][player->col].type == BUSH) ||
            (map[player->row][player->col].type == FLOWER)) {
            player->col--;
        }
    } else if ((move_cmd == 'i') || 
        (player->row - 1 < 0) ||
        (player->row + 1 > MAX_ROW) ||
        (player->col - 1 < 0) ||
        (player->col + 1 > MAX_COL)) {
    }
}

// A function that alerts in a 5x5 tile from the branch or cut flower and 
// ensures alert is within the bounds of the board 'MAX_COL * MAX_ROW'
void alert_grid(struct tile map[MAX_ROW][MAX_COL],
    struct player *player) {

    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if ((player->row + i >= 1) && (player->row + i <= 10) &&
                (player->col + j >= 1) && (player->col + j <= 10)) {
                    
                change_flower_state(map, player, player->row + i, player->col + j);
            }
        }
    }
}

void change_flower_state(struct tile map[MAX_ROW][MAX_COL],
    struct player *player,
    int flower_row,
    int flower_col) {

    if (map[flower_row][flower_col].flower.state == DORMANT) {
        map[flower_row][flower_col].flower.state = DISTURBED;
    } else if (map[flower_row][flower_col].flower.state == DISTURBED) {
        map[flower_row][flower_col].flower.state = AWAKENED;
    }
}

// Function that determines whether the game continues or finishes with player win
// or flower win
int win_condition(struct tile map[MAX_ROW][MAX_COL],
    struct player *player,
    int flower_count) {

    int flowers_alive = 0;
    int win = 0; 

    for (int row = 0; row < MAX_ROW; row++) {
        for (int col = 0; col < MAX_COL; col++) {
            if (map[row][col].type == FLOWER) {
                flowers_alive++;
            }
        }
    }

    if (flowers_alive == flower_count) {
        printf("Enter command: ");
        win = 0;
    } else if ((flower_count > flowers_alive) && (flowers_alive != 0)) {
        printf("Enter command: ");
        win = 0;
    } else if (flowers_alive == 0) {
        printf("All flowers are eradicated and UNSW has been saved!\n");
        win = 1;
    }
        
    return win;
}

//------------------------------------------------------------------------------
// Provided Functions
//------------------------------------------------------------------------------

/**
 * Initialises the game map with empty tiles, setting the type of each tile to 
 * `EMPTY` and initialising flower-related properties.
 *
 * @param map: The 2D array of `struct tile` representing the game map 
 *
 * @returns None
 *
**/
void initialise_map(struct tile map[MAX_ROW][MAX_COL]) {
    for (int row = 0; row < MAX_ROW; row++) {
        for (int col = 0; col < MAX_COL; col++) {
            map[row][col].type = EMPTY;
            map[row][col].flower.state = NONE;
        }
    }
}

/**
 * Prints the game map 
 * 
 * @param map: The 2D array of `struct tile` representing the game map.
 * @param player_row: The row coordinate of the player->
 * @param player_col: The column coordinate of the player->
 *
 * @returns None
**/
void print_map(
    struct tile map[MAX_ROW][MAX_COL], 
    int player_row, 
    int player_col
) {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("+---");
        }
        printf("+\n");

        for (int j = 0; j < MAX_COL;j++) {
            printf("|");
            if (player_row != i || player_col != j) {
                print_tile(map[i][j]);
            } else if (map[i][j].type == EMPTY) {
                printf(" P ");
            } else if (map[i][j].type == DANGER) {
                printf("<P>");
            }
        }

        printf("|\n");
    }

    for (int j = 0; j < MAX_COL; j++) {
        printf("+---");
    }
    printf("+\n");
}

/**
 * Helper function which prints the representation of a single tile 
 * based on its type. 
 *
 * @params tile: The `struct tile` to be printed.
 *
 * @returns None
**/
void print_tile(struct tile tile) {
    if (tile.type == EMPTY) {
        printf("   ");
    } else if (tile.type == BUSH) {
        printf("###");
    } else if (tile.type == BRANCH) {
        printf("_/-");
    } else if (tile.type == DANGER) {
        printf("< >");
    } else if (tile.type == ROOT) {
        printf("<^>");
    } else if (tile.type == POLLEN) {
        printf(":::");
    } else if (tile.type == FLOWER) {
        print_flower(tile.flower);
    }
}

/**
 * Helper function which prints a visual representation of the 
 * flower face based on its state.
 *
 * @params flower: The `struct flower` containing the state of the flower.
 *
 * @returns None
 *
**/
void print_flower(struct flower flower) {
    if (flower.state == DORMANT) {
        printf("uwu");
    } else if (flower.state == DISTURBED) {
        printf("uwo");
    } else if (flower.state == AWAKENED) {
        printf("owo");
    } else if (flower.state == ANXIOUS) {
        printf("o~o");
    } else if (flower.state == ENERGETIC) {
        printf("*w*");
    }
}

