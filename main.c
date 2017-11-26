//
//  main.c
//  7snakes
//
//  Created by Diogo Andrade on 24/11/17.
//  Copyright © 2017 Diogo Andrade. All rights reserved.
//

#include <stdio.h>
#include "main.h"

int main(int argc, const char * argv[]) {
    
    //Variables
    snakePatternList *newList = NULL;       //this list will store the possible snake patterns
    snakeSumsHashTable *hashTable = NULL;   //this hash table will store all possible snake sums for each pattern
                                            // for all possible starting positions on a matrix
    int snakeLength = 7;                    //the snake length, it is fixed at 7, but could be any value
    int matrixSize;                         //the size of the sqare matrix row or collumn at the input file
    int hashTableSize = 64;                 //hash table array size chosen as 255/(4*snakeLength) +1
    snakeSumsHashTable *firstSum = NULL,
                       *secondSum = NULL; //stores the pair of same sum snakes
    FILE *inputCsvFile;                     //file descriptor for input file
    int **B = NULL;                         //matrix pointer for the input matrix to be read from file

    //test if a file is given
    if (argc != 2)
        return 1;

    //get the input file descriptor
    inputCsvFile = fopen(argv[1],"rt");
    
    //parse the file into a matrix, and discover its size
    B = read_matrix_from_file(inputCsvFile, &matrixSize);
    
    //if something wrong happened, abort
    if (B == NULL)
        return 1;
    
    //generate snake patterns
    newList = generate_all_snake_patterns(snakeLength);
    
    //create the hash table and initialize the nodes with 0's
    hashTable = (snakeSumsHashTable *)malloc(hashTableSize*sizeof(snakeSumsHashTable));
    initialize_hash_table(hashTable, hashTableSize);
    
    //traverse matrix with all patterns, saving : starting cell, pattern type, sum total on a hash table.
    // On a collision of sums, check if there are no cells in commom in both snakes,
    // and if there are none, return both snakes cells and their sum
    if(compute_all_snake_pattern_sums(newList, hashTable, B, snakeLength,
                                      matrixSize, hashTableSize, &firstSum, &secondSum)){
        //success, print both snakes to file
        print_results_to_file(firstSum,secondSum,snakeLength);
    }
    
    //de-allocate memory
    purge_snake_pattern_list(newList);
    purge_pattern_sum_hash_table(hashTable, hashTableSize);
    
    return 0;
}



//function that reads a matrix of integers from a csv file and places it on a 2D array of int.
//This function assumes a square matrix of integers, separated by commas, and there are no commasat the end of lines.
int **read_matrix_from_file(FILE *inputCsvFile, int *matrixSize){

    //Variables
    char *readLine = NULL;
    int charCount = 0;
    char readChar;
    int commaCount = 0;
    int i,j;            //indexes
    char *charPtr;
    int **B = NULL;
    
    //loop to find how many characters the first line has
    while((readChar = fgetc(inputCsvFile) )!= '\n'){
        charCount++;
        if(readChar == ',')
            commaCount++;
    }
    
    //the line should have commaCount + 1 numbers, and that can't change from now on
    if (B == NULL){
        *matrixSize = commaCount+1;
        //allocate the rows
        if ((B = (int **)malloc(*matrixSize * sizeof(int *))) == NULL){
            //error allocationg memory
            return NULL;
        }
    }
    
    //file pre-processing done, rewind the file pointer
    rewind(inputCsvFile);
    
    //allocate memory for line reading if not done already. 3 times charcount should be enough fo every other line
    if ((readLine = (char *)malloc(3*(charCount)*sizeof(char))) == NULL)
        //memory error
        return NULL;
    
    //loop teh parsing routines
    for(i=0;i<*matrixSize;i++) {
        
        //allocate memory for the present row
        if((B[i] = (int *)malloc(*matrixSize * sizeof(int))) == NULL){
            //something went wrong, free whatever has been aloccated and then return NULL
            //rewind to last successful allocated row
            while (--i > 0){
                free(B[i]);
            }
            free(B);
            free(readLine);
            return NULL;
        }
        
        //read the line contents
        fgets(readLine, 3*charCount, inputCsvFile);
        
        //split the strings between commas
        charPtr = strtok(readLine, " ,'\n'");
        j = 0;
        //get each chunk and transform into an integer
        while(charPtr!= NULL){
            B[i][j++] = atoi(charPtr);
            charPtr =strtok(NULL, " ,'\n'");
        }
    }
    
    return B;
}


