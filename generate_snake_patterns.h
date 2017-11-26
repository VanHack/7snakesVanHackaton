//
//  generate_snake_patterns.h
//  7snakes
//
//  Created by Diogo Andrade on 24/11/17.
//  Copyright © 2017 Diogo Andrade. All rights reserved.
//

#ifndef generate_snake_patterns_h
#define generate_snake_patterns_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif /* generate_snake_patterns_h */

/* The pattern data structure is a series of moves: up, down, left, right. So, a dinamically allocated array of integers
 should be enough to describe a pattern.
*/

//definition for move types
#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3
#define END     4

/**********************/
/** Data structures ***/
/**********************/

//explicitly delcare the snake pattern type
typedef int snakePattern;

// List of snakePatterns node structure. Each node stores:
// - a pattern,
// - the size of the rectangle it occupies,
// - the starting point of the pattern relative to the upper left edge of the rectangle,
// - a pointer to the next node

typedef struct _snakePatternList {
    snakePattern *node;
    int sizeX;
    int sizeY;
    int anchorX;
    int anchorY;
    struct _snakePatternList *next;
    
} snakePatternList;

//array of cells positions on a snake's path
typedef struct _snakePath {
    int posX;
    int posY;
} snakePath;

//HashTable for storing sums for a given snake. Each node stores:
// - the snakePatternList node which the pattern belongs to
// - the starting cell for the sum
// - the sum
// - the next HashTable node for the same index, created on collisions
typedef struct _snakeSumsHashTable {
    snakePattern *patternNode;
    int sum;
    int startX;
    int startY;
    snakePath *path;
    struct _snakeSumsHashTable *next;
} snakeSumsHashTable;



/**************************/
/** Function prototypes ***/
/**************************/

//creates a pointer to a blank pattern of snakeLength number of cells
snakePattern *new_snake_pattern (int snakeLength);

//de-allocates a snakePattern form memory
void purge_snake_pattern (snakePattern *node);

//print the sequence of moves on a snakePattern
void print_snake_pattern(snakePattern *node, int snakeLength);

//creates a new snakePatternList
snakePatternList * new_snake_pattern_list(void);

//compare snakePatterns to check for congruent patterns. Return 1 if patterns are congruent, 0 otherwise
int compare_snake_Patterns(snakePattern *node1, snakePattern *node2, int snakeLength );

//add a snakePattern to a snakePatternList
int add_snake_pattern_to_list(snakePatternList **list, snakePattern *snakePatternNode, int snakeLength);

//de-allocates a snakePatternList from memory
void purge_snake_pattern_list(snakePatternList *list);

//print all snakePatterns on a snakePAtternList
void print_snake_pattern_list (snakePatternList *list, int snakeLength);

//generate all possible patterns for a n-length snake
snakePatternList *generate_all_snake_patterns(int snakeLength);

//cerates a dynamic allocated square matrix of  matrixSize filled with 0's
int **create_plot_matrix(int matrixSize);

//frees plotMatrix memory
void purge_plot_matrix( int **plotMatrix, int snakeLength);

//generate all possible patterns for a n-length snake
snakePatternList *generate_all_snake_patterns(int snakeLength);

//recursive function call that will generate all
int generate_snake_patterns_recursive(snakePatternList **patternList, snakePattern *snakeUnderConstruction,
                                       int **plotMatrix, int matrixSize, int snakeLength,int posX, int posY );

//check matrix cell suitability to be part of a snake under construction
int  check_valid_cell(int **plotMatrix, int posX, int posY, int snakeLength);

//eliminate duplicated patterns from a patternList
void eliminate_duplicated_patterns(snakePatternList *patternList, int snakeLength);

//calculates the pattern node properties
void calculate_pattern_node_properties(snakePatternList *listPtr, int snakeLength);

//function to print 2D matrix of integers
void print_matrix_2D(int **, int sizeX, int sizeY);

//function that will traverse the input matrix computing all sums for all snake patterns,
//from all possible starting points. Will exit as soon as 2 mutually exclusive snakes with the same sum is found.
//returns 1 if a pair is found, 0 if none are found
int compute_all_snake_pattern_sums(snakePatternList *list, snakeSumsHashTable *hashTable, int **inputMatrix,
                                   int snakeLength, int matrixSize, int tableSize, snakeSumsHashTable **firstSum, snakeSumsHashTable **secondSum);

//function that computes the sum for a given snake pattern at a given starting location
int get_snake_sum(int **inputMatrix, snakePattern *currentPattern, int startX, int startY, int snakeLength, int matrixSize);



//***** Hash Table functions *****

//initializes an instantiated HashTable
void initialize_hash_table(snakeSumsHashTable *inputTable, int tableSize);

//creates a new snakeSumHashTable node
snakeSumsHashTable *new_snake_sum_has_table_node(void);

//de-allocates a snakePatternList from memory
void purge_pattern_sum_hash_table(snakeSumsHashTable *table, int tablesize);

//de-allocate memory for collision lists on the hashtable
void purge_collision_list(snakeSumsHashTable *table);

//fucntion to store a given sum on a hastable
int insert_pattern_sum_hash_table(snakeSumsHashTable *hashTable, snakePattern *currentPattern,
                                  int snakeLength, int tableSize, int startX, int startY, int snakeSum,
                                  snakeSumsHashTable **firstSum, snakeSumsHashTable **secondSum);


//function that checks if two snakes are mutually exclusive, i.e, they share no cell in common
int check_mutually_exclusive_snakes(snakeSumsHashTable *snake1, snakeSumsHashTable *snake2, int snakeLength);

//fills the path of a snake data structure
void fill_path(snakeSumsHashTable *snake, int snakeLength);

//prints the path of the snake and its sum
void print_snake_path_with_sum(snakeSumsHashTable *firstSum, int snakeLength);

//prints these results to output file
void print_results_to_file(snakeSumsHashTable *firstSum, snakeSumsHashTable *secondSum,int snakeLength);

