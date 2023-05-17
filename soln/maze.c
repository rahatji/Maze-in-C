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

// validate our world
void checkState()
{
#ifndef NDEBUG
  CellNode *curr = top;
#endif
  
  // make sure the basics are clean
  assert(mazeRows > 0);
  assert(mazeRows <= MAX_DIMENSION);
  assert(mazeCols > 0);
  assert(mazeCols <= MAX_DIMENSION);
  
  // make sure our start/end are still valid
  validCell(mouse);
  validCell(escape);

  // we'll traverse the whole list and maze to validate them, but only in debug mode
#ifndef NDEBUG
  while (curr)
  {
    validCell(curr->cell);
    curr = curr->next;
  }
  
  for (int row=0; row<mazeRows; row++)
  {
    for (int col=0; col<mazeCols; col++)
    {
      assert(maze[row][col]==WALL || maze[row][col]==MOUSE || maze[row][col]==EXIT || maze[row][col]==SPACE || maze[row][col]==VISITED);
    }
  }
#endif
}

//////////////////////////////////////////////
// Cell routines
//////////////////////////////////////////////

// we check a cell over and over again...
Boolean validCell(const Cell theCell)
{
  assert(theCell.row>=0);
  assert(theCell.row<mazeRows);
  assert(theCell.column>=0);
  assert(theCell.column<mazeCols);
  
  return (theCell.row>=0 && theCell.row<mazeRows && theCell.column>=0 && theCell.column<mazeCols);
}

// a comparison routine to make it easy for us to see where we are in the maze
Boolean equalCells(const Cell cell1, const Cell cell2)
{
  Boolean equal = false;
  
  if (validCell(cell1) && validCell(cell2))
    equal = ((cell1.row == cell2.row) && (cell1.column == cell2.column));
  
  return equal;
}

// create a cell object for future storage
// how would you indicate an error???
Cell makeCell(const int row, const int col)
{
  Cell newCell = {-1,-1};
  
  assert(row>=0);
  assert(row<mazeRows);
  assert(col>=0);
  assert(col<mazeCols);
  
  if (row>=0 && row<mazeRows && col>=0 && col<mazeCols)
  {
    newCell.row = row;
    newCell.column = col;
  }
  
  validCell(newCell);
  
  return newCell;
}

//////////////////////////////////////////////
// List routines
//////////////////////////////////////////////


// returns true if our list is empty
Boolean noMoreCells()
{
  checkState();
  
  return (NULL == top);
}

// returns the first item in our list
Cell nextCell()
{
  Cell theCell = {-1,-1};
  
  checkState();
  
  if (top)
  {
    CellNode *dead = top;
    
    theCell = top->cell;
    top = top->next;
    
    free(dead);
    
    checkState();
    
    validCell(theCell);
  }
  
  return theCell;
}

void addCell(const Cell cell)
{
  CellNode *newNode = NULL;
  
  if ( validCell(cell) )
  {
    newNode = (CellNode *)malloc(sizeof(CellNode));
    assert(newNode != NULL);
    if (newNode)
    {
      newNode->cell = cell;
      newNode->next = top;
      top = newNode;
    
      checkState();
    }
  }
}

//////////////////////////////////////////////
// Maze routines
//////////////////////////////////////////////

// print the current state of our maze
void printMaze()
{
  checkState();
  
  // standard printing of a matrix
  for ( int i=0 ; i<mazeRows ; i++ )
  {
    for ( int j=0 ; j<mazeCols ; j++ )
    {
      printf( "%c", maze[i][j] );
    }
    printf( "\n" );
  }

  printf( "\n" );
  
  checkState();
}

void loadMaze()
{
  int row, col;
  int count;
  char ch;
  
  // load in the file
  count = scanf("%d %d", &(mazeRows), &(mazeCols));
  assert(2 == count);
  assert(mazeRows>0);
  assert(mazeRows<=MAX_DIMENSION);  
  assert(mazeCols>0);
  assert(mazeCols<=MAX_DIMENSION);

  if (2 == count &&
      mazeRows > 0 && mazeRows <= MAX_DIMENSION &&
      mazeCols > 0 && mazeCols <= MAX_DIMENSION)
  {
    // clear the newline...
    getchar();
    
    for (row = 0; row < mazeRows; row++)
    {
      for (col = 0; col < mazeCols; col++)
      {
        ch = getchar();
        assert(ch==WALL || ch==MOUSE || ch==EXIT || ch==SPACE);
        if (ch==WALL || ch==MOUSE || ch==EXIT || ch==SPACE)
        {
          maze[row][col] = ch;
        
          // check to see if we have a mouse or exit location
          if ( maze[row][col] == MOUSE )
            mouse = makeCell( row, col );
          else if ( maze[row][col] == EXIT )
            escape = makeCell( row, col );
        }
        // if it's an invalide character in the file, default to something that will stop our searching
        else
          maze[row][col] = VISITED;
          
        // skip the space, and end-of-line character
        getchar();
      }
    }

    checkState();
  }
}

Boolean solveMaze()
{
  Boolean deadEnd = false;
  char cellValue;
  // we need to track our location based on the mouse start position
  Cell currCell = mouse;
  
  checkState();
  
  printMaze();
  
  // search for an exit
  while ( !equalCells( escape, currCell ) && !deadEnd )
  {
    // mark the current cell so we don't try it again
    maze[currCell.row][currCell.column] = VISITED;

    // add in the neighbour cells that are valid moves
    // note the we rely on a boundary of WALLs to prevent exceeding array bounds
    cellValue = maze[currCell.row-1][currCell.column];
    if ( cellValue != WALL && cellValue != VISITED )
      addCell( makeCell( currCell.row-1, currCell.column ) );
    cellValue = maze[currCell.row][currCell.column+1];
    if ( cellValue != WALL && cellValue != VISITED )
      addCell( makeCell( currCell.row, currCell.column+1 ) );
    cellValue = maze[currCell.row][currCell.column-1];
    if ( cellValue != WALL && cellValue != VISITED )
      addCell( makeCell( currCell.row, currCell.column-1 ) );
    cellValue = maze[currCell.row+1][currCell.column];
    if ( cellValue != WALL && cellValue != VISITED )
      addCell( makeCell( currCell.row+1, currCell.column ) );

    if ( noMoreCells() )
      deadEnd = true;
    else
      currCell = nextCell();

    printMaze();
    
    checkState();
  }

  // if we didn't hit a dead end then we must have exited
  return !deadEnd;
}
