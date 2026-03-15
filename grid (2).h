/*-------------------------------------------------
Grid

The Grid class provides a grid (2D array) abstraction.
A Grid is defined to have a given number of rows and columns, and 
All elements are initialized with C++'s natural default value. By 
default, a 4x4 grid is created, unless otherwise specified.

Course: CS 211, Fall 2025, UIC
Author: Coral Fragoso Herrera
-------------------------------------------------*/

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class Grid {
private:
  struct ROW {
    T*  Cols;       // array of column elements
    size_t NumCols; // total # of columns (0..NumCols-1)
  };

  ROW* Rows;       // array of ROWs
  size_t  NumRows; // total # of rows (0..NumRows-1)
    
public:

  /*
  This function is a default constructor and is automatically called
  to construct a 4x4 Grid.
  Parameters:
    This function has no Parameters.
  Return:
    This function doesn't return anything, instead initializes
    elements to default value of T.
  */
  Grid() {
    Rows = new ROW[4];  // 4 rows
    NumRows = 4;        // total # of rows

    // initialize each row to have 4 columns:
    for (size_t r = 0; r < NumRows; ++r) {
      Rows[r].Cols = new T[4];
      Rows[r].NumCols = 4; // total # of columns

      // initialize the elements to their default value:
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = T();  // default value for type T:
      }
    }
  }

  /*
  This function is a parameterized constructor called to automatically 
  construct a Grid with R rows, where each row has C columns.  
  Parameters:
    R - # of rows
    C - # of columns
  Return:
    This function does not directly return anything, instead allows
    elements initialized to default value of T.
  */
  Grid(size_t R, size_t C) {
    if (R < 1) { // checks if # of rows is invalid
      throw invalid_argument("Grid::constructor: # of rows"); // exception handling
    }
    if (C < 1) { // checks if # of columns is invalid
      throw invalid_argument("Grid::constructor: # of cols"); // exception handling
    }

    // allocates memory for rows of R size
    Rows = new ROW[R]; 
    NumRows = R;

    // alloactes memory for columns of C size
    // and defaults cell to T type
    for (size_t r = 0; r < NumRows; ++r) {
      Rows[r].Cols = new T[C];
      Rows[r].NumCols = C;

      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        Rows[r].Cols[c] = T();
      }
    }
  }
    
  /*
  This function is a destructor called automatically to 
  free memory for this Grid.
  Parameters:
    This function has no parameters.
  Return:
    This function does not return anything, instead frees memory.
  */
  virtual ~Grid() {
    // frees allocated memory for a column
    for (size_t r = 0; r < NumRows; ++r) {
      delete[] Rows[r].Cols;
    }
    delete[] Rows; // frees memory for a row itself
  }

  /*
  This function is a copy constructor that automatically is called
  to construct a Grid that contains a copy of an existing Grid.
  Parameters:
    other - existing Grid
  Return:
    This function does not return anything, instead creates a copy of a grid.
  */
  Grid(const Grid<T>& other) {

    // allocates the same amount of memory for rows as in other
    this->Rows = new ROW[other.NumRows];
    this->NumRows = other.NumRows;

    // allocates the same amount of memory for columns as in other
    for (size_t r = 0; r < other.NumRows; ++r) {
      size_t other_numcols = other.Rows[r].NumCols;

      this->Rows[r].Cols = new T[other_numcols];
      this->Rows[r].NumCols = other_numcols;

      // copies over every cell from other
      for (size_t c = 0; c < other_numcols; ++c) {
        this->Rows[r].Cols[c] = other.Rows[r].Cols[c];
      }
    }
  }
    
  /*
  This function is a copy operator= that is called when
  one Grid is assigned into another.
  Parameters:
    other - existing Grid
  Return:
    returns a references to the current object 
  */
  Grid& operator=(const Grid& other) {
      Grid<T> temp;
      // prevents the copy operator from being called 
      // if it tries to copy itself
      if (this == &other) {
        return *this;
      }

      // deletes the current information (frees memory) from object
      for (size_t r = 0; r < NumRows; ++r) {
        delete[] Rows[r].Cols; // frees columns
      }
      delete[] Rows;           // frees rows itself

      // assigns new row memory based on other's size
      this->Rows = new ROW[other.NumRows];
      this->NumRows = other.NumRows;

      // assigns new column memory based on other's size
      for (size_t r = 0; r < other.NumRows; ++r) {
        size_t other_numcols = other.Rows[r].NumCols;

        this->Rows[r].Cols = new T[other_numcols];
        this->Rows[r].NumCols = other_numcols;

        // assigns other's information to copy for each cell
        for (size_t c = 0; c < other_numcols; ++c) {
          this->Rows[r].Cols[c] = other.Rows[r].Cols[c];
        }
      }

    return *this;  // returns reference to the current object
  }

  /*
  This function gets the # of rows in the Grid.
  Parameters:
    This function has no parameters.
  Return:
    This function returns the # of rows in the Grid.
  */
  size_t numrows() const {

    return NumRows;  // returns # of rows in Grid
  }
  
  /*
  This function gets the # of columns in row r. 
  Parameters: 
    r - a row in Grid
  Return:
    returns the # of columns in row r.
  */
  size_t numcols(size_t r) const {
      // checks if r is a valid row
      if (r < 0 || r >= NumRows) {
        throw out_of_range("# of rows");
      }
      
      return Rows[r].NumCols;  // returns # of columns in row r
  }

  /*
  This function gets the the total # of elements in the Grid.
  Parameters:
    This function has no parameters.
  Return:
    This function returns the sum of elements in Grid.
  */
  size_t size() const {
    int sum = 0; // gets sum of elements

    // increments sum for each cell in Grid
    for (size_t r = 0; r < NumRows; ++r) {
      for (size_t c = 0; c < Rows[r].NumCols; ++c) {
        sum++;
      }
    }
      
      return sum;  // sum of elements
  }

  /*
  This function is an overloaded operator and gets a reference to the 
  element at location (r, c) and allows you to access or assign the element.
  Parameters:
    r - row r
    c = column c
  Return:
    returns a reference to the element at location (r,c).
  */
  T& operator()(size_t r, size_t c) {

      // checks if r and c are valid location in Grid    
      if (r >= NumRows) {
        throw invalid_argument("operator(): Invalid row #");
      }
      if (c >= Rows[r].NumCols) {
        throw invalid_argument("operator(): Invalid col #");
      }
      
      return Rows[r].Cols[c];  // returns reference to element 
  }

  /*
  This function outputs the contents of the grid for debugging purposes.
  Parameters:
    This function has no parameters.
  Return:
    This function does no return anything
  */
  void _output() {
    // displays each cell in Grid
    for (size_t r = 0; r < NumRows; ++r) {
        cout << "| ";
        for (size_t c = 0; c < Rows[r].NumCols; ++c) {
            cout << Rows[NumRows-r-1].Cols[c] << " | ";
        }
        cout << endl;
    }
  }
};
