# 7snakesVanHackaton

# This is the code submitted to the 7Snakes Challenge during the VanHackaton 4.0, on november 2017.
# This program has been tested only on OSX 10.11.5
 Compile instructions are:

  gcc -Wall generate_snake_patterns.c main.c -o 7snakes
 
# Usage:
  
  ./7snakes <input_csv_file>

 The input CSV file should describe a square, of any size, matrix of integers, from 1 to 255, 
 separated by commas, each line being a row. There can't be commas at the end of the lines. 
 The routine that reads the file and fills a matrix of integers does not check for these conditions.
 Using an invalid matrix at the input can cause the program to crash.

# Output
 The program should output a file called "output_result.txt", with each line showing the sum of the
 numbers from each snake cells, and the cells where each snake occupies.

# General algorithm guidelines

# 1st step:

 The program first generates all possible snake patterns for a given size, following the rule of formation,  
 and then compareseach pattern with the others to discard matching patterns. The first step has roughly a 
 runtime complexity of 
  
  K = O(4 * 3^(n-2)), n being the snakeLength, in this specific case, n = 7
  
 this formula is derived from the fact that the starting point for finding patterns has 4 directions to search,
 while the next moves would have only 3 possibilities. the second routine has an approximate runtime of

  M = O(K * log(K))
  
 because each search reduces the search space by 1. All patterns are stored into a list containing the size of the 
 rectangle its shape occupies, and the starting cell offset from the upper-left edge of this rectangle.

# 2nd step:

 Now the algorithm traverses the input matrix, "sliding" each pattern rectangle around, with proper precautiions not
 to violate the matrix boundaries, calculating the sum for each snake pattern for each possible starting point, and 
 stores these results on a hash table, with the hash function being the sums. So, when a collision occurs during
 insertion, it means the current snake has a high probability of having the same sum as the others already on the
 collision list. The algorithm then compares the current snake sum with the others from the collision list, and
 if a match is found, an then they are checked for overlapping cells. If the snakes are mutually exclusive, i.e. they
 share no cells, then the program stops and print their sum and their paths on the putput file. This step has roughly
 the time complexity of
 
  R = O( (S - (n/2))^2 ), n being the snake length, and S the matrix row/collumn size
  
 S m/2 is subtracted from S because most rectangles will have its row or collumn size generally equal to n/2, so 
 there are fewer iterations when seeping the matrix for each pattern.





 
