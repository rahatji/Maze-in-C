//---------------------------
// NAME: Jahidul Islam Rahat
// STUDENT NUMBER: 7900518
// COURSE: COMP 2160, SECTION: A01
// INSTRUCTOR: Saulo Santos
// ASSIGNMENT: #02, QUESTION: #Q1
// PURPOSE: This program read maze file through redirect operator or from standard input and solve it
//



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//-------------------------------------------------------------------------------------
// CONSTANTS and TYPES
//-------------------------------------------------------------------------------------

#define MAX_DIMENSION 20

// constant definitions for the different cell states
const char WALL    = '1';
const char SPACE   = '0';
const char VISITED = '.';
const char MOUSE   = 'm';
const char EXIT    = 'e';

typedef enum BOOL { false, true } Boolean;

struct CELL
{
  int row;
  int column;
};
typedef struct CELL Cell;

typedef struct CELL_NODE CellNode;
struct CELL_NODE
{
  Cell     cell;
  CellNode *next;
};

//-------------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------------

CellNode *top = NULL;

// a 2D array used to store the maze
char maze[MAX_DIMENSION][MAX_DIMENSION];
int mazeRows;
int mazeCols;

// holds the location of the mouse and escape hatch
Cell mouse;
Cell escape;

//-------------------------------------------------------------------------------------
// PROTOTYPES
//-------------------------------------------------------------------------------------

// basic cell manipulation

// returns true if the cells are at the same position in our maze
Boolean equalCells(const Cell cell1, const Cell cell2);
// returns a new cell object
Cell makeCell(const int row, const int col);
// returns true if the cell is within our maze
Boolean validCell(const Cell theCell);

// routines for managing our backtracking

// returns true if there are no more cells to try
Boolean noMoreCells();
// returns the next cell to try for a path out of the maze
Cell nextCell();
// introduces a new cell to try
void addCell(const Cell cell);

void printMaze();
void loadMaze();

// returns true if there's a solution to the maze
Boolean solveMaze();

// our invariant checker
void checkState();

//-------------------------------------------------------------------------------------
// FUNCTIONS
//-------------------------------------------------------------------------------------

int main( int argc, char *argv[] )
{
  loadMaze();

  if ( solveMaze() )
    printf( "The mouse is free!!!!\n" );
  else
    printf( "The mouse is trapped!!!!\n" );
  
  printf( "\nEnd of processing\n" );
    
  return EXIT_SUCCESS;
}


//////////////////////////////////////////////
// Cell routines
//////////////////////////////////////////////


// This function checks whether 2 cells are equal or not
Boolean equalCells(const Cell cell1, const Cell cell2)
{
        Boolean result = false;
        if(validCell(cell1) && validCell(cell2)){

                if((cell1.row == cell2.row) && (cell1.column == cell2.column)) {
                        result = true;
                }

        }
        return result;
}// euqal cells


//This fucntion create a cell taking row and column value
Cell makeCell(const int row, const int col)

{
        #ifndef NDEBUG

        assert(row >= 0);
        assert(col >= 0);
        assert(row < mazeRows);
        assert(col < mazeCols);
        #endif

        Cell newCell = {MAX_DIMENSION,MAX_DIMENSION};

        if (row >= 0 && row < mazeRows && col >= 0 && col < mazeCols)
        {
                newCell.row = row;
                newCell.column = col;
        }
        return newCell;

}

// This function check whether desired cell is valid or not
Boolean validCell(const Cell theCell)

{
        #ifndef NDEBUG

        assert (theCell.row >= 0);
        assert(theCell.row < mazeRows);
        assert(theCell.column >= 0);
        assert(theCell.column < mazeCols);
        #endif

        Boolean result = false;

        if (theCell.row >= 0 && theCell.row < mazeRows && theCell.column >= 0 && theCell.column < mazeCols) {
                result = true;
        }

        return result;

}


//////////////////////////////////////////////
// List routines
//////////////////////////////////////////////


// Check is there any cell left in the node
Boolean noMoreCells()

{

        Boolean result = false;
        checkState();

        if (top == NULL)

        {
                result = true;
        }//if

        return result;
}//no more cell

// call the next cell in the node
Cell nextCell()

{

        Cell next = {MAX_DIMENSION,MAX_DIMENSION};

        if( top!=NULL ) {

                next = top->cell;
                top = top->next;

                validCell(next);
        }//if
        return next;
}// nextcell

// This function add a cell in the node
void addCell(const Cell cell)

{
        CellNode *newNode = NULL;

        if(validCell(cell)) {

        newNode = (CellNode*)malloc(sizeof(CellNode));

        #ifndef NDEBUG
        assert(newNode != NULL);
        #endif

        newNode->cell = cell;
        newNode->next = top;
        top = newNode;
        checkState();
        }//if

}//add cell


//////////////////////////////////////////////
// Maze routines
//////////////////////////////////////////////


// This function prints the currect scenario of the maze
void printMaze()

{

        checkState();
        for( int i = 0; i < mazeRows; i++ )
        {

                for(int j = 0; j < mazeCols; j++)
                {
                        printf("%c", maze[i][j]);

                }//for
                printf("\n");

        } //for
        printf("\n");


}//printmaze

// this function load the maze from the standard input
void loadMaze()
{


        scanf("%d %d", &(mazeRows), &(mazeCols));

        #ifndef NDEBUG
        assert(mazeRows>0);
        assert(mazeRows<=MAX_DIMENSION);
        assert(mazeCols>0);
        assert(mazeCols<=MAX_DIMENSION);

        #endif

        getchar(); //deleting new line
        char input[MAX_DIMENSION * 2];


        for(int i = 0; i < mazeRows; i++)
        {       fgets(input, MAX_DIMENSION * 2, stdin);
                for(int j = 0; j < mazeCols; j++)
                {
                        assert(input[j*2]==WALL || input[j*2]==MOUSE || input[j*2]==EXIT || input[j*2]==SPACE || input[j*2] == VISITED);
                        if(input[j*2] == WALL || input[j*2] == SPACE || input[j*2] == MOUSE || input[j*2] == EXIT || input[j*2] == VISITED){
                                maze[i][j] = input[j*2];

                        }//if
                        if(maze[i][j] == MOUSE)
                        {
                                mouse = makeCell(i,j);

                        }//if

                        if(maze[i][j] == EXIT)
                        {
                                escape = makeCell(i,j);
                        }//if
                        //getchar();
                }//for
        }//for
        checkState();
}
// Load Maze

// This function solve the maze and return true or false
Boolean solveMaze()

{

  Boolean mouseTrapped = false;
  Cell currentCell = mouse;
  Boolean goal = equalCells(currentCell , escape) ;

  checkState();

  printMaze();



        while( !goal && !mouseTrapped)
        {
                if(equalCells(currentCell , escape)) {
                        goal = true;
                        checkState();
                        return goal;
                }//if

                maze[currentCell.row][currentCell.column] = VISITED;

                if(maze[currentCell.row+1][currentCell.column] == SPACE || (maze[currentCell.row+1][currentCell.column] == EXIT))
                {
                        addCell(makeCell(currentCell.row + 1, currentCell.column));
                }//if

                if(maze[currentCell.row-1][currentCell.column] == SPACE || maze[currentCell.row-1][currentCell.column] == EXIT)
                {
                        addCell(makeCell(currentCell.row - 1, currentCell.column));
                }//iff

                if(maze[currentCell.row][currentCell.column+1] == SPACE || maze[currentCell.row][currentCell.column+1] == EXIT)
                {
                        addCell(makeCell(currentCell.row , currentCell.column + 1));
                }//if

                if(maze[currentCell.row][currentCell.column-1] == SPACE || maze[currentCell.row][currentCell.column-1] == EXIT)
                {
                        addCell(makeCell(currentCell.row , currentCell.column-1));
                }//if


                if(noMoreCells())
                {
                        mouseTrapped = true;
                        goal = false;
                }//if
                else
                {
                        currentCell = nextCell();
                }//else
                printMaze();

        }//while
        return goal;
}//SolveMaze



//Invariant Function
void checkState()

{
        #ifndef NDEBUG

        assert(mazeRows > 0);
        assert(mazeRows <= MAX_DIMENSION);
        assert(mazeCols > 0);
        assert(mazeCols <= MAX_DIMENSION);

        validCell(mouse);
        validCell(escape);

        for (int i=0; i<mazeRows; i++)
        {
                for (int j=0; j<mazeCols; j++)
                {
                        assert(maze[i][j]==WALL || maze[i][j]==MOUSE || maze[i][j]==EXIT || maze[i][j]==SPACE || maze[i][j] == VISITED);
                }//for
        }//for

        #endif
}