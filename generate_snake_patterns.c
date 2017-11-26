//
//  generate_snake_patterns.c
//  7snakes
//
//  Created by Diogo Andrade on 24/11/17.
//  Copyright © 2017 Diogo Andrade. All rights reserved.
//

#include "generate_snake_patterns.h"

/**************************/
/** Function bodies *******/
/**************************/

//creates a pointer to a blank pattern of snakeLength number of cells
snakePattern *new_snake_pattern (int snakeLength){
    
    snakePattern *new_pattern;
    int i;
    
    //alocates the required memory
    new_pattern = (snakePattern *)malloc(snakeLength*sizeof(snakePattern));
    
    //sets all elements to "END"
    for(i=0;i<snakeLength;i++)
        *(new_pattern + i) = END;
    return new_pattern;
}

//de-allocates a snakePattern form memory
void purge_snake_pattern (snakePattern *node){
    if (node != NULL)
        free(node);
};

//print the sequence of moves on a snakePattern
void print_snake_pattern(snakePattern *node, int snakeLength){
    
    snakePattern *snakePtr = node;
    int i;
    //print every sequence of moves
    for(i=0;i<snakeLength;i++){
        switch (*(snakePtr + i)){
            case UP:
                printf("UP, ");
                break;
            case DOWN:
                printf("DO, ");
                break;
            case LEFT:
                printf("LE, ");
                break;
            case RIGHT:
                printf("RI, ");
                break;
            case END:
                //printf("END, ");
                break;
        }
    }
}

//creates a new snakePatternList node
snakePatternList * new_snake_pattern_list(void){
    
    snakePatternList *newList;
    //create the first node
    newList = (snakePatternList *) malloc(sizeof(snakePatternList));
    //check if allocation was successful
    if (newList){
        newList->node = NULL;
        newList->next = NULL;
    }
    
    return (newList);
}

//compare snakePatterns to check for congruent patterns. Return 1 if patterns are congruent, 0 otherwise.
//congruent nodes are those which, when their sequence of moves is reversed, they match a previously generated one exactely.
//this function run in O(snakeLength) time
int compare_snake_Patterns(snakePattern *node1, snakePattern *node2 , int snakeLength){
    
    int i;
    snakePattern node2Reversed = END;

    for(i=0;i<snakeLength-1;i++)
    {
        //compare each node1, from 0 to snakelength-2 with each node2 element reversed,
        //from snakelength-2 to 0;
        switch(*(node2+snakeLength - 2 - i)){
            case UP:
                node2Reversed = DOWN;
                break;
            case DOWN:
                node2Reversed = UP;
                break;
            case LEFT:
                node2Reversed = RIGHT;
                break;
            case RIGHT:
                node2Reversed = LEFT;
                break;
            //the "END" case can never happen, since we start with snakeLength -2 as the index.
        }
        
        //compare the nodes. 1 different node is enough to discard congruency
        if(*(node1+i) != node2Reversed){
            return 0;
        }
    }
    
    //If the program reaches this point, it means the snakePatterns match.
    return 1;
    
}

//add a copy of a snakePattern to a snakePatternList, returns 1 on success, 0 on memory allocation failure
int add_snake_pattern_to_list(snakePatternList **list, snakePattern *snakePatternNode, int snakeLength){
    
    //Variables
    snakePatternList *listPtr = NULL;       //pointer to traverse the list
    snakePattern     *copyPattern = NULL;   //pointer to store the copied pattern
    
    //point listptr to the newly created node
    listPtr = *list;
    
    //if this is the very first node, create it
    if((*list) == NULL){
        *list = new_snake_pattern_list();
        //check if there were any memory allocation errors
        if (*list == NULL)
            return 0;
        
        //point listPtr to the newly created node
        listPtr = *list;
        
    //list is not empty, search for the end node
    }else{
        //find the last list node
        while (listPtr->next != NULL)
            listPtr = listPtr->next;
        //create new node
        listPtr->next = new_snake_pattern_list();
        
        //check if there were any memory allocation errors
        if (listPtr->next == NULL)
            return 0;
        
        //point to the newly created node
        listPtr = listPtr->next;
    }
    
    //create and copy the snakePatternNode, checking for memory errors
    copyPattern = new_snake_pattern(snakeLength);
    if (copyPattern == NULL)
        return 0;
    memcpy((void*)copyPattern, (void*)snakePatternNode, snakeLength*sizeof(snakePattern));
    
    //populate the new node with given snakePattern node
    listPtr->node = copyPattern;
    
    //calculate the remaining pattern properties
    calculate_pattern_node_properties(listPtr, snakeLength);
    
    //npo errors reported
    return 1;
}

//de-allocates a snakePatternList from memory. This function takes O(number_of_patterns) space on the call stack.
void purge_snake_pattern_list(snakePatternList *list){
    
    if (list == NULL)
        return;
    
    //recursively call the function to free all the snakePatterns stored
    if (list->next != NULL)
        purge_snake_pattern_list(list->next);
    
    //purge the snakePattern node
    if (list->node != NULL)
        purge_snake_pattern(list->node);
    
    //purge the list node
    free(list);
    
}

//print all snakePatterns on a snakePAtternList
void print_snake_pattern_list (snakePatternList *list, int snakeLength){
    
    //Variables
    snakePatternList *listPtr = list; //pointer to traverse the list
    static int i = 0;                         //counts the number of patterns
    
    printf("%04d: size: %02d,%02d, start: %02d, %02d, moves: ",i++,listPtr->sizeX,listPtr->sizeY, listPtr->anchorX, listPtr->anchorY);
    //print node contents
    if (listPtr->node != NULL)
        print_snake_pattern(listPtr->node, snakeLength);
    
    //print linefeed
    printf("\r\n");
    
    //print next node
    if (listPtr->next != NULL)
        print_snake_pattern_list(listPtr->next, snakeLength);
    
}

//generate all possible patterns for a n-length snake
snakePatternList *generate_all_snake_patterns(int snakeLength){
    
    //Snake formation rule: every cell should be adjacent to only 1 or 2 cells. This generetaion will be implemented recursevly, along with plotting the current path on a matrix to check for exccessive adjacency.
    
    //Variables
    snakePatternList *patternList = NULL; //create the snakePatternList
    int matrixSize = 0;                   //chosen plot matrix size
    int startingPosition = 0;             //chosen starting position on the plotmatrix
    
    
    //check for edge case: snakeLength must be 1 or greater
    if (snakeLength <1)
        return NULL;
    
    //choose the matrix size and starting point of discovery
    matrixSize = snakeLength*2 +1;
    startingPosition = snakeLength;
    
    //create the path matrix filled with 0's. It's a square matrix of snakeLength size
    int **plotMatrix = create_plot_matrix(snakeLength*2 +1);
    
    //check for memory allocation errors
    if(plotMatrix == NULL){
        return NULL;
    }
    
    //recursive function call that will generate all
    if (generate_snake_patterns_recursive(&patternList, NULL, plotMatrix,matrixSize,
                                          snakeLength, startingPosition, startingPosition) == 0){
        //purge any memory used in case of error
        purge_snake_pattern_list(patternList);
        patternList = NULL;
    }
    
    //free plotmatrix memory
    purge_plot_matrix(plotMatrix, snakeLength);
    
    //eliminate duplicated patterns
    eliminate_duplicated_patterns(patternList, snakeLength);
    
    //return the created list if no list was created, the pointer will still be NULL
    return patternList;
}

//cerates a dynamic allocated square matrix of snakeLength size filled with 0's
int **create_plot_matrix(int matrixSize){
    
    int i;
    int **plotMatrix;
    
    //allocate row pointers
    if ((plotMatrix = (int **)malloc(matrixSize * sizeof(int *))) == NULL){
        //error allocationg memory
        return NULL;
    }
    
    //allocate row elements
    for (i=0; i<matrixSize; i++){
        if((plotMatrix[i] = (int *)malloc(matrixSize * sizeof(int))) == NULL){
            //something went wrong, free whatever has been aloccated and then return NULL
            //rewind to last successful allocated row
            while (--i > 0){
                free(plotMatrix[i]);
            }
            free(plotMatrix);
            return NULL;
        }else{
            //fill allocatted rows with 0's
            memset(plotMatrix[i], 0, matrixSize*sizeof(int));
        }
            
    }

    return (plotMatrix);
    
}

//frees plotMatrix memory
void purge_plot_matrix( int **plotMatrix, int snakeLength){
    
    int i;
    //free plotMatrix memory
    for (i=0; i<snakeLength; i++)
        free(plotMatrix[i]);
    free(plotMatrix);
    
}

//recursive function call that will generate all. returns 0 if a memory allocation error ocurred, 1 otherwise
int generate_snake_patterns_recursive(snakePatternList **patternList, snakePattern *snakeUnderConstruction,
                                       int **plotMatrix,int matrixSize, int snakeLength, int posX, int posY ){
    
    //Variables
    int result = 1;                   //stores the return results from recursive calls

    //Static Variables, should persist through all recursive calls execution
    static int currentLength = 0;
    
    
    //1st step: checks if there are at most one adjacent cell already marked as belonging to the snake
    //          under construction, so that we avoid creating cycles.
    
    if (!check_valid_cell(plotMatrix, posX, posY, matrixSize)){
        //this cell is not suitable for the snakeUnderConstruction
        return 1;
    }
    
    //2nd step: this is a suitable snake cell. We add it to the snake
    
    //check for the snakeLength. If it is zero, this will be the first cell, and therefore we should create the
    //snake under construction
    if (currentLength == 0){
        snakeUnderConstruction = new_snake_pattern(snakeLength);
        //test for memory allocation error
        if(snakeUnderConstruction == NULL){
            //the function will recursively propagate the error
            return 0;
        }
    }
    
    //increment currentLength
    currentLength++;
    
    // 3rd step:End of recursive calls check: snake is complete
    
    if(currentLength == snakeLength){
        //if so, mark the end of the snake
        *(snakeUnderConstruction + currentLength-1) = END;

        //insert a copy of the pattern into the list
        if(!add_snake_pattern_to_list(patternList, snakeUnderConstruction,snakeLength)){
            //an memory error ocurred
            //special case of a 1-cell snake, purge the snakeunderconstruction here
            if(currentLength == 1)
                purge_snake_pattern(snakeUnderConstruction);
            return 0;
        }
        
        //rewind the currentLength for other recursive calls then return
        currentLength--;
        //special case of a 1-cell snake, purge the snakeunderconstruction here
        if(currentLength == 0)
            purge_snake_pattern(snakeUnderConstruction);
        
        return 1;
    
    //4th step: snake is incomplete, need to find the next snake Cell
    }else{
        //mark the present cell onto the plotMatrix, and record the move that will be tried
        plotMatrix[posX][posY] = 1;

        //move DOWN
        snakeUnderConstruction[currentLength -1 ] = DOWN;
        result = generate_snake_patterns_recursive(patternList, snakeUnderConstruction,
                                                   plotMatrix, matrixSize, snakeLength, posX + 1,posY );
        //move RIGHT
        if (result){
            snakeUnderConstruction[currentLength -1 ] = RIGHT;
            result = generate_snake_patterns_recursive(patternList, snakeUnderConstruction,
                                                       plotMatrix, matrixSize, snakeLength, posX, posY + 1 );
        }
        //move UP
        if (result){
            snakeUnderConstruction[currentLength -1 ] = UP;
            result = generate_snake_patterns_recursive(patternList, snakeUnderConstruction,
                                                       plotMatrix, matrixSize, snakeLength, posX - 1, posY);
        }
        //move LEFT
        if (result){
            snakeUnderConstruction[currentLength -1 ] = LEFT;
            result = generate_snake_patterns_recursive(patternList, snakeUnderConstruction,
                                                       plotMatrix, matrixSize, snakeLength, posX, posY -1);
        }
    }
    
    //5th step: The all possible snake patterns from this cell has been found
    //unmark it on the plotmatrix
    plotMatrix[posX][posY] = 0;
    
    //decrement the currentLength
    currentLength--;
    
    //6th step: End of recursive calls: if the currentlength is 0, and no errors ocurred, then all patterns have been found
    //and the snakeUnderConstruction memory can be freed
    if (result && !currentLength){
        purge_snake_pattern(snakeUnderConstruction);
    }
    
    //return the current error state
    return result;
}

//check matrix cell suitability to be part of a snake under construction
int check_valid_cell(int **plotMatrix, int posX, int posY, int matrixSize){

    //Variables
    int adjacentCellsCounter = 0; //counts the number of adjacent cells that are art of the snake already
    
    //check if position is into the plotmatrix bounds
    if(!(posX <= (matrixSize - 1) && posY <= (matrixSize - 1) &&
       posX >= 0 && posY >= 0))
        return 0;
    
    //if the present cell is already on a snake, return 0, there can't be cycles
    if (plotMatrix[posX][posY] == 1)
        return 0;
    
    //special case: 1 sized snake
    if (matrixSize == 1)
        return 1;
    
    //checks for adjacent snake cell on posX + 1 and posY, checking for the bottom matrix edge
    if (posX + 1 <= (matrixSize - 1) && plotMatrix[posX + 1][posY] == 1)
        adjacentCellsCounter++;

    //checks for adjacent snake cell on posX and posY + 1, checking for the right matrix edge
    if (posY + 1 <= (matrixSize - 1) && plotMatrix[posX][posY + 1] == 1)
        adjacentCellsCounter++;
    
    //checks for adjacent snake cell on posX - 1 and posY, checking for the top matrix edge
    if (posX - 1 >= 0 && plotMatrix[posX - 1][posY] == 1)
        adjacentCellsCounter++;

    //checks for adjacent snake cell on posX and posY - 1, checking for the left matrix edge
    if (posY - 1 >= 0 && plotMatrix[posX][posY - 1] == 1)
        adjacentCellsCounter++;

    // if there are more than 1 cell already adjacent, this cell cannot be part of the snake under construction
    if (adjacentCellsCounter > 1){
        return 0;
    }
    
    //cell can be part of the snake
    return 1;
}

//eliminate duplicated patterns from a patternList
void eliminate_duplicated_patterns(snakePatternList *patternList, int snakeLength){
    
    //this function compares each pattern to every other one to their "right"
    //on the list. It should run on O(nlog(n)) time complexity, n being the list size.
    
    //Variables
    snakePatternList *anchorPtr = patternList; //points to the pattern that will be compared to the remainder of the list
    snakePatternList *runPtr = NULL;           //traverse the remainder of the list comparing its elements to anchor
    snakePatternList *prevPtr = NULL;          //marks the previous node on the list for removal purposes
    
    //special case, snakeLength = 1
    if(anchorPtr->next == NULL){
        return;
    }
    
    //comparison loop
    while (anchorPtr->next != NULL) {
        runPtr = anchorPtr->next;
        prevPtr = anchorPtr;
        while(runPtr!= NULL){
            //if patterns are congruent, then remove the runPtr node
            if(compare_snake_Patterns(anchorPtr->node, runPtr->node, snakeLength)){
                prevPtr->next = runPtr->next;
                runPtr->next = NULL;
                purge_snake_pattern_list(runPtr);
                runPtr = prevPtr->next;
                
            //snakes are different
            }else{
                //advance the pointers
                prevPtr = runPtr;
                runPtr = runPtr->next;
            }
        }
        //advance the anchor
        anchorPtr = anchorPtr->next;
        if (anchorPtr == NULL)
            break;
    }
}

//calculates the pattern node properties
void calculate_pattern_node_properties(snakePatternList *listPtr, int snakeLength){
    
    //Variables
    int maxX, maxY, minX, minY; //records the edges of the rectangle the pattern occupies
    int posX, posY;             //current cell traversing the pattern

    snakePattern *snakePtr = listPtr->node; //pointer to traverse the pattern
    
    //initiate all integer variables
    maxX = maxY = minX = minY = posX = posY = 0;
    //loop traversing the pattern
    while((*snakePtr) != END){
        switch ((*snakePtr)) {
            //with each move, update the cell position and the rectangle limits
            case UP:
                posX--;
                if(minX > posX)
                    minX = posX;
                break;
            case DOWN:
                posX++;
                if(maxX < posX)
                    maxX = posX;
                break;
            case LEFT:
                posY--;
                if(minY > posY)
                    minY = posY;
                break;
            case RIGHT:
                posY++;
                if(maxY < posY)
                    maxY = posY;
                break;
        }
        //next pattern move
        snakePtr++;
    }
    
    //calculate the rectangle size on each axis
    listPtr->sizeX = abs(maxX-minX) + 1;
    listPtr->sizeY = abs(maxY-minY) + 1;
    
    //calculate the pattern starting point offset, related to the upper-left edge of the ractangle
    listPtr->anchorX = minX*(-1);
    listPtr->anchorY = minY*(-1);
    
}

//function to print 2D matrix of integers
void print_matrix_2D(int **B, int sizeX, int sizeY){
   
    //Variables
    int i,j=0; //index
    
    for(i=0;i<sizeX;i++){
        printf("row:%02d: ",i);
        for(j=0;j<sizeY;j++){
            printf("%03d, ",B[i][j]);
        }
        printf("\r\n");
    }
}

int compute_all_snake_pattern_sums(snakePatternList *list, snakeSumsHashTable *hashTable, int ** inputMatrix,
                                   int snakeLength, int matrixSize, int tableSize, snakeSumsHashTable **firstSum, snakeSumsHashTable **secondSum){
    
    //Variables
    int i,j;        //indexes
    int maxX, maxY; //limits for indexes for a given rectangle
    int startX, startY; //starting cell for a given pattern
    int snakeSum;            //stores a snake pattern sum
    int result = 0;         //stores the function result to be returned
    snakePatternList *listPtr = list;   //pointer to traverse the psnake patterns list
    
    
    //error checking
    if(listPtr == NULL || hashTable == NULL || inputMatrix == NULL)
        return 0;
    
    //for each pattern on the list
    while (listPtr != NULL) {
        //define the index limits for the pattern rectangle
        //in case the rectangle size exceeds the matrixSize, nothing will happen
        maxX = matrixSize - listPtr->sizeX;
        maxY = matrixSize - listPtr->sizeY;
        
        //traverse the matrix, slidong the rectangle around, and computing the sums
        for(i=0;i<maxX;i++){
            //define the start X point
            startX = i + listPtr->anchorX;
            
            //slide the rectangle through the collumns
            for(j=0;j<maxY;j++){
                //define the starting Y point
                startY = i + listPtr->anchorY;
                
                //compute the sum for this pattern at this starting point
                snakeSum = get_snake_sum(inputMatrix, listPtr->node, startX, startY, snakeLength, matrixSize);
                
                //store the sum on the hashtable, test for equal sums from mutually exclusive snakes
                result = insert_pattern_sum_hash_table(hashTable, listPtr->node, snakeLength, tableSize,
                                                       startX, startY, snakeSum, firstSum,secondSum);
                //check for memory allocation errors
                if(result == -1){
                    //clear all collision lists from memory
                    purge_pattern_sum_hash_table(hashTable, tableSize);
                    return -1;
                }
                
                //check it a pair of mutually exclusive snakes with the same sum were found
                if(result){
                    //the pointers fristSum and secondSum already has the snakes on them
                    return 1;
                }
                
            }
            
        }
        //increment list pointer
        listPtr = listPtr->next;
    }
    
    //no pairs were found
    return 0;
}

//function that computes the sum for a given snake pattern at a given starting location
int get_snake_sum(int **inputMatrix, snakePattern *currentPattern, int startX, int startY, int snakeLength, int matrixsize){
 
    //Variables
    snakePattern *snakePtr = currentPattern; //traverse the movement pattern
    int sum = 0;    //stores this pattern sum
    int xPos = startX;
    int yPos = startY;
    
    //traverse the pattern
    while ((*snakePtr) != END) {
        //add the current cell
        sum+= inputMatrix[xPos][yPos];
        
        //find the next cell
        switch ((*snakePtr)) {
            case UP:
                xPos--;
                break;
            case DOWN:
                xPos++;
                break;
            case LEFT:
                yPos--;
                break;
            case RIGHT:
                yPos++;
                break;
            default:
                break;
        }
        
        //increment the pointer
        snakePtr++;
    }
    
    //sum the number on the END cell
    sum+= inputMatrix[xPos][yPos];
    
    //return total
    return sum;
}

//***** HastTable functions ******


//fucntion to store a given sum on a hastable
int insert_pattern_sum_hash_table(snakeSumsHashTable *hashTable, snakePattern *currentPattern,
                                  int snakeLength, int tableSize, int startX, int startY, int snakeSum,
                                  snakeSumsHashTable **firstSum, snakeSumsHashTable **secondSum){
    //Variables
    int tableIndex;     //index where the sum will be stored on the table
    int result = 0;     //assuming no pairs are found
    snakeSumsHashTable *hashTablePtr, *newCollisionNode; //pointer to traverse the collisions for a given entry
    
    //the hash function is snakeSum%(tableSize)
    tableIndex = snakeSum%(tableSize);
    
    //check if there are any sums on that index
    if( hashTable[tableIndex].patternNode == NULL ){
        //fill the data
        hashTable[tableIndex].patternNode = currentPattern;
        hashTable[tableIndex].path = NULL;
        hashTable[tableIndex].startX = startX;
        hashTable[tableIndex].startY = startY;
        hashTable[tableIndex].sum = snakeSum;
        
        //no pair of sums were found
        return 0;
        
    //a collision ocurred
    }else{
        
        //a collision ocurred, we create a new collision node
        newCollisionNode = new_snake_sum_has_table_node();
        
        //test for memory allocation erros
        if(newCollisionNode == NULL){
            return -1;
        }
        
        //point to the first node of the collision list
        hashTablePtr = hashTable + tableIndex;
        
        //fill in the data
        newCollisionNode->patternNode = currentPattern;
        newCollisionNode->startX = startX;
        newCollisionNode->startY = startY;
        newCollisionNode->sum = snakeSum;
        newCollisionNode->path = NULL;
        newCollisionNode->next = NULL;
        
        //traverse the collisions searching for an equal sum
        while(hashTablePtr != NULL){
            //test the sums if no pair is found yet
            if((hashTablePtr->sum == snakeSum) && !result){
                //test for mutually exclusive snake
                if(check_mutually_exclusive_snakes(hashTablePtr, newCollisionNode, snakeLength)){
                    //if positive, record the hash table node found here on firstSum
                    *firstSum = hashTablePtr;
                    *secondSum = newCollisionNode;
                    //print_snake_path_with_sum(hashTablePtr, snakeLength);
                    //print_snake_path_with_sum(newCollisionNode, snakeLength);
                    result = 1;
                    //now the loop will continue, and the newCollisonNode will be inserted at the end of the collision list
                }
            }
            
            //exit loop condition
            if (hashTablePtr->next != NULL)
                hashTablePtr = hashTablePtr->next;
            else
                break;
        }
        
        //no mutually exclusive snakes with the same sum were found. Add this sum to the end of the collision list
        hashTablePtr->next = newCollisionNode;
    }
    
    //return the result found:
    return result;
    
    
}

//initializes an instantiated HashTable
void initialize_hash_table(snakeSumsHashTable *inputTable, int tableSize){
    
    //Variables
    int i;
    
    //traverse the table initializing the properties
    for(i=0;i<tableSize;i++){
        inputTable[i].patternNode = NULL;
        inputTable[i].path = NULL;
        inputTable[i].startX = 0;
        inputTable[i].startY = 0;
        inputTable[i].sum = 0;
        inputTable[i].next = 0;
    }
}

//creates a new snakeSumHashTable node
snakeSumsHashTable *new_snake_sum_has_table_node(void){
    
    snakeSumsHashTable *newNode;
    //create the first node
    newNode = (snakeSumsHashTable *) malloc(sizeof(snakeSumsHashTable));
    //check if allocation was successful
    if (newNode){
        newNode->patternNode = NULL;
        newNode->path = NULL;
        newNode->sum = 0;
        newNode->startX = 0;
        newNode->startY = 0;
    }
    
    return (newNode);
}

//de-allocates the collisions lists from a hasTable
void purge_pattern_sum_hash_table(snakeSumsHashTable *table, int tableSize){
    
    //Variables
    int i;
    
    for (i=0;i<tableSize;i++){
        if (table[i].next != NULL)
            //purge each of the collision lists recursively
            purge_collision_list(table[i].next);
        
        //checks for allocated path
        if (table[i].path != NULL)
            free(table[i].path);
    }
    
    free(table);
}

//de-allocate memory for collision lists on the hashtable
void purge_collision_list(snakeSumsHashTable *table){
    
    if(table->next != NULL){
        purge_collision_list(table->next);
    }
    
    //checks for allocated path
    if (table->path != NULL)
        free(table->path);
    //purge this node
    free(table);
}


//function that checks if two snakes are mutually exclusive, i.e, they share no cell in common, returns 1 if they are, 0 otherwise
int check_mutually_exclusive_snakes(snakeSumsHashTable *snake1, snakeSumsHashTable *snake2, int snakeLength){
    
    //Variables
    int i,j;      //indexes
    snakePath *pathPtr1, *pathPtr2;   //pointer that will store the cmatrix cells on this snake path
    
    //create the paths if not created already
    if(snake1->path == NULL){
        //create the path
        snake1->path = (snakePath *)malloc(snakeLength*sizeof(snakePath));
        //check for allocation errors
        if (snake1->path == NULL)
            return -1;
        //fill the path here
        fill_path(snake1, snakeLength);
    }
    
    //point the pathPtr to its begining
    pathPtr1 = snake1->path;
    
    //Do the same for the second path
    if(snake2->path == NULL){
        //create the path
        snake2->path = (snakePath *)malloc(snakeLength*sizeof(snakePath));
        //check for allocation errors
        if (snake2->path == NULL)
            return -1;
        //fill the path here
        fill_path(snake2, snakeLength);
    }
    
    //point the pathPtr to its begining
    pathPtr2 = snake2->path;
    
    //compare the path of both snakes, assuming first they are not equal
    for(i=0;i<snakeLength;i++){
        for(j=0;j<snakeLength;j++){
            if (((pathPtr1 +i)->posX == (pathPtr2 +j)->posX) &&
                ((pathPtr1 +i)->posY == (pathPtr2 +j)->posY)){
                //a cell in common was found
                return 0;
            }
        }
    }
    
    //no commmon cell was found between snakes
    return 1;
}

//fills the path of a snake data structure
void fill_path(snakeSumsHashTable *snake, int snakeLength){
    
    //Variables
    int xPos, yPos;
    snakePattern *patternPtr = snake->patternNode;
    snakePath    *pathPtr = snake->path;
    
    //initialize the position variables
    xPos = pathPtr->posX = snake->startX;
    yPos = pathPtr->posY = snake->startY;
    
    //traverse the pattern
    while ((*patternPtr) != END) {
        
        //increment the path pointer
        pathPtr++;
        
        //find the next cell
        switch ((*patternPtr)) {
            case UP:
                xPos--;
                break;
            case DOWN:
                xPos++;
                break;
            case LEFT:
                yPos--;
                break;
            case RIGHT:
                yPos++;
                break;
            default:
                break;
        }
        //record the cell
        pathPtr->posX = xPos;
        pathPtr->posY = yPos;
        
        //increment the pattern Pointer
        patternPtr++;
    }
}

//prints the path of the snake and its sum
void print_snake_path_with_sum(snakeSumsHashTable *firstSum, int snakeLength){
    
    //variables
    int i;
    
    //print the snakes cell by cell
    printf("sum: %04d, path: [",firstSum->sum);
    for(i=0;i<snakeLength;i++){
        //print the path
        printf("(%02d,%02d), ",(firstSum->path + i)->posX,(firstSum->path + i)->posY);
    }
    
    //print the ending
    printf("]\r\n");
}

//prints these results to output file
void print_results_to_file(snakeSumsHashTable *firstSum, snakeSumsHashTable *secondSum,int snakeLength){
    
    FILE *outputFile;
    int i;

    outputFile = fopen("output_result.txt", "w+");
    
    //no snakes were found
    if(firstSum == NULL && secondSum == NULL){
        fprintf(outputFile, "No Matching snakes were found.\n");
        fclose(outputFile);
        return;
    }
    
    //print the snakes cell by cell
    fprintf(outputFile, "sum: %04d, path: [",firstSum->sum);
    for(i=0;i<snakeLength;i++){
        //print the path
        fprintf(outputFile, "(%02d,%02d)",(firstSum->path + i)->posX,(firstSum->path + i)->posY);
        if(i != snakeLength-1)
            fprintf(outputFile, ", ");
    }
    
    //print the ending
    fprintf(outputFile, "]\r\n");
    
    //repeat with second string
    //print the snakes cell by cell
    fprintf(outputFile, "sum: %04d, path: [",secondSum->sum);
    for(i=0;i<snakeLength;i++){
        //print the path
        fprintf(outputFile, "(%02d,%02d)",(secondSum->path + i)->posX,(secondSum->path + i)->posY);
        if(i != snakeLength-1)
            fprintf(outputFile, ", ");
    }
    
    //print the ending
    fprintf(outputFile, "]\r\n");
    
    //print explanations
    fprintf(outputFile, "The indexes are (row, collumn), and they are 0-based\r\n");
    fclose(outputFile);

}













