# nQueens Problem

###### Artificial Intelligence - Final Project

Solves up to n = 500,000 in reasonable time, requires further optimization for larger values of n.


###### Input:

NxN board (see format of initBoard.txt)

###### Output:

NxN board solved (see format of outputBoard.txt)
 
 
## Min-Conflicts Algorithm

Chooses a column from the list of columns which has queen with the highest number of conflicts. We choose from the lowest rows on this column as the new position for the queen. This is done for a predetermined number of steps. 

•	highestConflictColumn() - randomly chooses a queen from set of highest columns

•	minConflict() - picks a conflict value in the column that minimizes conflicts, where the highestConflictColumn is the input

•	the step counter is then incremented

•	and the board is checked to see if a solution was found


