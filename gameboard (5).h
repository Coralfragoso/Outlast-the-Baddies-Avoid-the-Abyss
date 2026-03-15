// gameboard.h
//
// GameBoard class starter code for course project
// University of Illinois at Chicago
// CS 211 - Programming Practicum
// Original Author: Professor Scott Reckinger
//

#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
	private: 
	    Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        size_t HeroRow; // Hero's position row
	    size_t HeroCol; // Hero's position column
        int numMonsters;
        int numSuperMonsters;
        int numAbysses;
        int numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully

		
	public: 
		/* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;
            
            this -> numRows = 15;
            this -> numCols = 40;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;
            
            this -> numRows = numRows;
            this -> numCols = numCols;
            
            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;
            
            blankBoard();
        }
        
        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }

        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }

        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }

        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }
    
        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);
            size_t r,c;

            r = rand() % numRows;
            c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);
            
            int sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (int i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (int i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (int i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (int i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
		void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            
        }
		
        bool getWonGame() {
            return wonGame;
        }
        
        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }

        void setNumAbysses(int num) {
            numAbysses = num;
        }

        void setNumBats(int num) {
            numBats = num;
        }

        size_t getNumRows() {
            return numRows;
        }

        size_t getNumCols() {
            return numCols;
        }

        /*
        This getter function gets the Hero's position
        Parameters:
            row - hero's row position
            col - hero's column position
        Return:
            Passes by reference the private data members int HeroRow and int HeroCol
            as row and col
        */
        void getHeroPosition(size_t& row, size_t& col) {

            row = HeroRow;  // gets Hero's row
            col = HeroCol;  // gets Hero's column
        }

        /*
        This setter function sets the Hero's position, which are private data members
                                                                        int HeroRow;
                                                                        int HeroCol;
        Parameters:
            row - row position for setting
            col - column position for setting
        Return:
            This function does not return anything, instead sets hero's position
        */
        void setHeroPosition(size_t row, size_t col) {

            HeroRow = row; // sets hero's row
            HeroCol = col; // sets hero's column
        }

        /*
        This function is an updater for Hero's position, which are private data members
                                                                            int HeroRow;
                                                                            int HeroCol;

        The function finds the hero in the board and updates HeroRow and HeroCol with the Hero's 
        updated position. If Hero cannot be found in board, then set Hero's position to (-1,-1).
        Parameters:
            This function has no parameters
        Return:
            This function does not return anything, instead sets the hero's position
        */
        void findHero() {
            bool found = false; // hero is not found

            // checks every cell in board for hero
            for (size_t r = 0; r < numRows; ++r) {
                for (size_t c = 0; c < numCols; ++c) {
                    // checks if a cell contains the hero
                    if (board(r,c)->isHero()) {
                        found = true; // hero is found
                        size_t row = r; // gets hero's row
                        size_t col = c; // gets hero's column
                        setHeroPosition(row, col); // sets ther hero's position
                        break;
                    }
                }
                if (found) { // stops searching if hero is found
                    break;
                }
            }
            if (!found){ // checks if hero is not found
                setHeroPosition(-1, -1); // sets position of missing hero
            }            
        }

        /*
        This function performs the primary gameplay operation for a single round of the game. This function handles
        illegal moves (Baddie or Hero move out-of-bounds, move onto a wall, Baddies moves onto EscapeLadder) and moves that removes the hero or baddies 
        from the board (Hero or Baddie land in the Abyss, Hero wins, Hero is defeated).
        Parameters:
            HeroNextMove - character representing move
        Return:
            returns true if Hero is still on board at the end of the round
            returns false if Hero is NOT on board at the end of the round
        */
        bool makeMoves(char HeroNextMove) {
        
            /*---------------------------
                    HERO MOVEMENT
            ---------------------------*/

            // determine where hero proposes to move to
            size_t newR, newC;
            board(HeroRow,HeroCol)->setNextMove(HeroNextMove);
            board(HeroRow,HeroCol)->attemptMoveTo(newR,newC,HeroRow,HeroCol);


            try {
                // hero attempts to move out-of-bounds in rows
                if (newR < 0 || newR >= numRows) { 
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
                } 
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                newR = HeroRow; // sets next row position to remain the same
                cout << "Changing row for Hero position to stay in-bounds" << endl;
            }

            try {
                // hero attempts to move out-of-bounds in columns
                if (newC < 0 || newC >= numCols) {
                    throw runtime_error("Hero trying to move out-of-bounds with invalid column");
                }
            }
            catch (runtime_error& excpt) {
                cout << excpt.what() << endl;
                newC = HeroCol; // sets next column position to remain the same
                cout << "Changing column for Hero position to stay in-bounds" << endl;
            }
            try {
                // hero attempts to move onto Wall
                if (board(newR, newC)->display() == '+') {
                    throw logic_error("Hero trying to move onto Wall");
                }

            }
            catch (logic_error& excpt) {
                cout << excpt.what() << endl;
                
                // checks if move is different from current position
                if (newR != HeroRow && newC != HeroCol) {
                    newC = HeroCol; // sets new column position to remain the same
                    try {
                        // hero attempts to move onto a Wall again
                        if (board(newR, newC)->display() == '+') {
                            throw logic_error("Hero trying to move onto Wall again");
                        }
                    }
                    catch (logic_error& excpt2) {
                        cout << excpt2.what() << endl;

                        // sets position to stay put
                        newR = HeroRow;
                        newC = HeroCol;

                        cout << "Changing row/column to stay off the Wall" << endl;
                    }
                } 
                else { // there is nothing in the new cell in move after 1st Wall
                    
                    // sets new position from move
                    newR = HeroRow;
                    newC = HeroCol;
                
                    cout << "Changing row/column to stay off the Wall" << endl;
                }   
            }

            bool escaped = false; // stores true if hero escapes via EscapeLadder

            // Hero's attempted move is the EscapeLadder
            if (board(newR, newC)->display() == '*') {
                escaped = true; // hero will escape
            }

            bool lostExit = false; // True if hero loses the game by being taken out of board

            // Hero attempts to move into the Abyss
            if (board(newR, newC)->display() == '#' || board(newR, newC)->isBaddie()) {
                lostExit = true; // Hero loses
            }
            
            // Hero's attempted move is different from current position
            if (newR != HeroRow || newC != HeroCol) {
                // Hero has left the board
                if (escaped || lostExit) {
                    BoardCell* oldHero = board(HeroRow, HeroCol); // tracks the old position of hero
                    board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol); // old postion is now initialized empty 
                    delete oldHero; // frees memory allocated for hero (removes hero from board)

                    setHeroPosition(-1, -1); // position is now set for missing hero

                    // Hero escaped
                    if (escaped) {
                        wonGame = true; // The hero has won the game
                    }

                    return false; // Hero is not on the board
                }
                else { // Hero has not left the board
                    BoardCell* temp = board(HeroRow, HeroCol); // tracks the old position of hero
                    board(HeroRow, HeroCol) = board (newR, newC); // Hero's position is now the attempted move
                    board(newR, newC) = temp; // The attempted move is the hero's position
                
                    board(HeroRow, HeroCol)->setRow(HeroRow); // sets the row of the updated position for hero
                    board(HeroRow, HeroCol)->setCol(HeroCol); // sets the column of the updated position for hero

                    board(newR, newC)->setRow(newR); // sets the row of the attempted move
                    board(newR, newC)->setCol(newC); // sets the column of the attempted move
                }
            }

            findHero(); // searches for hero on the board

            /*---------------------------------
                       BADDIE MOVEMENT
            ---------------------------------*/

            bool defeated = false; // hero is defeated by baddies
            size_t baddieR, baddieC; // gets the attempted move of Baddie


            // sets the default Moved state of each cell to false
            for (size_t r = 0; r < numRows; ++r) {
                for (size_t c = 0; c < numCols; ++c) {
                    board(r, c)->setMoved(false);
                }
            }

            // loops through every cell to check a baddie's movement
            for (size_t r = 0; r < numRows; ++r) {
                for (size_t c = 0; c < numCols; c++) {
                    // cell is a baddie
                    if (board(r,c)->isBaddie()) {
                        // cell has not moved yet
                        if (!(board(r,c)->getMoved())) {
                            size_t newRow = r; // sets new row of Baddie to current position
                            size_t newCol = c; // sets new column of Baddie to current position

                            board(r,c)->setNextMove(HeroNextMove); // Next move of Baddie is dependent on Hero
                            board(r,c)->attemptMoveTo(newRow, newCol, HeroRow, HeroCol); // gets attempted move of Baddie

                            try {
                                // Baddie attempts to move out-of-bounds with invalid row
                                if (newRow < 0 || newRow >= numRows) {
                                    throw runtime_error("Baddie trying to move out-of-bounds with an invalid row");
                                }
                            }
                            catch (runtime_error& excpt) {
                                cout << excpt.what() << endl;
                                newRow = r; // sets new row position of Baddie to remain unchanged
                                cout << "Changing row for Baddie position to stay in-bounds" << endl;
                            }
                            try {
                                // Baddie attempts to move out-of-bounds with invalid column
                                if (newCol < 0 || newCol >= numCols) {
                                    throw runtime_error("Baddie trying to move out-of-bounds with an invalid column");
                                }
                            }
                            catch (runtime_error& excpt) {
                                cout << excpt.what() << endl;
                                newCol = c; // sets new column position of Badddie to remain unchanged
                                cout << "Changing column for Baddie position to stay in-bounds" << endl;
                            }
                            try {
                                // Baddie attempts to move onto a Wall or the EscapeLadder
                                if (board(newRow, newCol)->display() == '+' || board(newRow, newCol)->display() == '*') {
                                    throw logic_error("Baddie is trying to move onto Wall/EscapeLadder");
                                }
                            }
                            catch (logic_error& excpt) {
                                cout << excpt.what() << endl;

                                // attempted Baddie move is different from current position
                                if (newRow != r && newCol != c) {
                                    newCol = c; // new column postion remains unchanged
                                    try {
                                        // Baddie still attempts to move onto Wall or EscapeLadder
                                        if (board(newRow, newCol)->display() == '+' || board(newRow, newCol)->display() == '*') {
                                            throw logic_error("Hero is trying to move onto Wall/EscapeLadder again");
                                        }
                                    }
                                    catch (logic_error& excpt2) {
                                        cout << excpt2.what() << endl;

                                        // sets new postion to remain as the current position
                                        newRow = r;
                                        newCol = c;

                                        cout << "Changing row/column to stay off the Wall" << endl;                                
                                    }
                                }
                                else { // Baddie stays put
                                    newRow = r;
                                    newCol = c;
                                    cout << "Changing row/column to stay off the Wall" << endl;                       
                                }
                            }

                            bool baddieExit = false; // true if Baddie is taken off the board

                            // Baddie's attempted move lands on the Abyss
                            if (board(newRow, newCol)->display() == '#') {
                                baddieExit = true; // Baddie will exit the board
                            }
                            
                            bool beatsHero = false; // true if Baddie removes hero from board

                            // Badddie's attempted move lands on the Hero
                            if (board(newRow, newCol)->isHero()) {
                                beatsHero = true; // Baddie beats the Hero
                            }

                            // Baddie's attempted move is different from current postion
                            if (newRow != r || newCol != c) {
                                // Baddie will exit the board
                                if (baddieExit) {
                                    BoardCell* oldBaddie = board(r, c); // tracks Baddie
                                    board(r, c) = new Nothing(r, c); // replaces Baddies position with initialized empty cell
                                    delete oldBaddie; // frees allocated memory for Baddie (removed from board)
                                    
                                }
                                // Baddie beats the hero and the hero is not yet defeated
                                else if (beatsHero && !defeated) {
                                    cout << "Uh Oh! A Baddie has caught the Hero..." << endl;

                                    // keeps track of the Baddie's position who beats the hero
                                    baddieR = r;
                                    baddieC = c;                                 
                                    defeated = true; // Hero will be defeated
                                }
                                // Baddie attempts to land on another Baddie
                                else if (board(newRow, newCol)->isBaddie()) {       
                                    // position remains unchanged of Baddie                  
                                    newRow = r;
                                    newCol = c;
                                    cout << "Changing row/column to stay off Baddie" << endl;
                                     
                                }
                                // Baddie can move without restrictions
                                else if (board(newRow, newCol)->isSpace()) {
                                    BoardCell* temp = board(r, c); // tracks position of Baddie
                                    board(r, c) = board(newRow, newCol); // Baddie's new position is updated
                                    board(newRow, newCol) = temp; // new postion is now the Baddie

                                    // sets the position of Baddie
                                    board(r, c)->setRow(r);
                                    board(r, c)->setCol(c);

                                    // sets the cell for the new position of Baddie
                                    board(newRow, newCol)->setRow(newRow);
                                    board(newRow, newCol)->setCol(newCol);

                                    board(newRow, newCol)->setMoved(true); // Baddie has moved
                                   
                                }
                            }                        
                        }
                    }
                }
            }

            // Hero is defeated
            if (defeated) {
                BoardCell* oldBaddie = board(baddieR, baddieC); // Tracks Baddie that defeated Hero
                BoardCell* oldHero = board(HeroRow, HeroCol); // Tracks defeated hero
                delete oldHero; // Hero's memory is freed (removed from board)
                board(HeroRow, HeroCol) = oldBaddie; // The hero's position is assigned and tracks the Baddie cell

                // The former Hero cell has its position set
                board(HeroRow, HeroCol)->setRow(HeroRow); 
                board(HeroRow, HeroCol)->setCol(HeroCol);
                board(HeroRow, HeroCol)->setMoved(true); // sets the cell as moved 

                board(baddieR, baddieC) = new Nothing(baddieR, baddieC); // Baddie's former postion is initialized as empty 

                setHeroPosition(-1, -1); // updates missing hero's position
            }
            
            // defaults all the cell's state of Moved to false
            for (size_t r = 0; r < numRows; ++r) {
                for (size_t c = 0; c < numCols; ++c) {
                    board(r,c)->setMoved(false);
                }
            }

            // returns false if hero is defeated (not on board)
            if (defeated) {
                return false;
            }

            // returns true if hero is on the board
            if (HeroRow < numRows && HeroCol < numCols) {
                return true;
            }

            return false; // else, false is returned
        }   
};

#endif //_GAMEBOARD_H