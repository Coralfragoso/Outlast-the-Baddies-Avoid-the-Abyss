// boardcell.h
//
// BoardCell class starter code for course project
// University of Illinois at Chicago
// CS 211 - Programming Practicum
// Original Author: Professor Scott Reckinger
//

#ifndef _BOARDCELL_H
#define _BOARDCELL_H

using namespace std;

// First, the BoardCell abstract base class 

class BoardCell {
	
    public:  
		BoardCell() {} // default contstructor (do nothing)
        virtual ~BoardCell() {} // destructor (do nothing)
        
        virtual char display( ) = 0; // pure virtual function; this is an abstract base class

        /*
        This function is the base class version for a board cell's attempted move to position 
        (newR,newC). The default action is to stay put, i.e. new position (newR, newC) is same 
        as old position (myRow, myCol).
        Parameters:
            newR - new row position
            newC - new column position
            hRow - hero's row position
            hCol - hero's column position
        Return:
            Passes by reference newR and newC to retrieve new positions for move.
        */
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            
            newR = myRow; // newR remains the same as current row
            newC = myCol; // newC remains the same as current column
        }
        
        virtual bool isHero( ) {return false;}
    	virtual bool isBaddie( ) {return false;}
    	virtual bool isSpace( ) {return false;}
        virtual bool isBarrier( ) {return false;}
        virtual bool isHole( ) {return false;}
    	virtual bool isExit( ) {return false;}
    	virtual bool isStatic( ) {return true;}

        virtual void setPower(int num) {}
        virtual int getPower() {return 1;}
        virtual void setNextMove(char inChar) {}
        
        void setMoved(bool m) {moved = m;}
        bool getMoved() {return moved;}
        void setRow(size_t r) {myRow = r;}
        size_t getRow() {return myRow;}
        void setCol(size_t c) {myCol = c;}
        size_t getCol() {return myCol;}
        void setPos(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }

    private:
        size_t myRow; // current row for this board cell in a 2D grid
        size_t myCol; // current column for this board cell in a 2D grid
        bool moved;   // true = this board cell already moved in the current round

}; // BoardCell (abstract base class)


// Then, all the derived classes

class Hero: public BoardCell {
	
    public:
    	Hero(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        
        virtual bool isHero( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return 'H';}
        virtual ~Hero() {}
        virtual void setNextMove(char inChar ) {
            nextMove = inChar;
        }
    	
        /*
        This function gets the attempted move for Hero.
            Hero's attempted move is determined by the nextMove data member 
                  nextMove to determine attempted new position for Hero
                      'q' = up and left       'w' = up        'e' = up and right
                      'a' = left              's' = stay      'd' = right
                      'z' = down and left     'x' = down      'c' = down and right
                      any other input defaults to stay
        Parameters:
            newR - new position for row
            newC - new positon for column
            hRow - hero's row position
            hCol - hero's column position
        Return:
            Passes by reference newR and newC to retrieve new positions for move.
        */
        virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
            // modifies next move to be up and left
            if (nextMove == 'q') {
                newR = hRow - 1; 
                newC = hCol - 1; 
            } 
            // modifies next move to be up
            else if (nextMove == 'w') {
                newR = hRow - 1;
                newC = hCol;
            }
            // modifies next move to be up and right
            else if (nextMove == 'e') {
                newR = hRow - 1;
                newC = hCol + 1;
            }
            // modifies next move to be left
            else if (nextMove == 'a') {
                newR = hRow;
                newC = hCol - 1;
            }
            // modifies next move to be right
            else if (nextMove == 'd') {
                newR = hRow;
                newC = hCol + 1;
            }
            // modifies next move to be down and left
            else if (nextMove == 'z') {
                newR = hRow + 1;
                newC = hCol - 1;
            }
            // modifies next move to be down
            else if (nextMove == 'x') {
                newR = hRow + 1;
                newC = hCol;
            }
            // modifies next move to be down and right
            else if (nextMove == 'c') {
                newR = hRow + 1;
                newC = hCol + 1;
            }
            // stay put otherwise
            else {
                newR = hRow;
                newC = hCol;
            }       
        }

    private:
        char nextMove;

}; // Hero


class Monster: public BoardCell {
    
    public:
		Monster(size_t r, size_t c) {
            setRow(r);
            setCol(c);
            power = 1;
        }
        
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        
        virtual char display( ) {
            if (power == 2) {
                return 'M'; // Super Monster
            } else {
                return 'm'; // Monster
            } 
        }

        virtual void setPower(int num) {
            power = num;
        }

        virtual int getPower() {
            return power;
        }
        
        /*
        This function gets the attempted move for Monster. Monsters always attempt to navigate 
        toward the hero. Regular monsters with powers of 1 move 1 step vertically or horizontally 
        towards the hero and  SuperMonsters with powers of 2 take 2 steps unless they are in the
        same column or row as the hero.
        Parameters:
            newR - new position for row
            newC - new postion for columns
            hRow - 
            hCol -
        Return:
            Passes by reference newR and newC to retrieve new positions for attempted move.
        */
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {   
            // checks if monster and hero are in different columns       
            if (hCol != getCol()) {
                // checks if monster is above hero
                if (getRow() > hRow) {
                    newR = getRow() - power; // monster's row moves towards hero
                }
                else { // monster is below hero
                    newR = getRow() + power; // monster's row moves towrds hero
                }         
            }
            // checks if monster and hero are in different rows
            if (hRow != getRow()) {
                // checks if monster is to the right of hero
                if (getCol() > hCol) {
                    newC = getCol() - power; // monster's column moves towards hero
                }
                else { // monster is to the left of hero
                    newC = getCol() + power; // monster's column moves towards hero
                }
            }
            // checks if monster and hero are in the same column
            if (hCol == getCol()) {
                // checks if hero is above monster
                if (hRow > getRow()) {
                    newR = getRow() + power; // monster's row moves towards hero
                }
                else { // hero is below monster
                    newR = getRow() - power; // monster's row moves towards hero
                }

                newC = getCol(); // column is unchanged
            }
            // checks if monster and hero are in the same row
            if (hRow == getRow()) {
                // checks if hero is to the right of monster
                if (hCol > getCol()) { 
                    newC = getCol() + power; // monster's column moves towards hero
                }
                else { // hero is to the left of monster
                    newC = getCol() - power; // monster's column moves towards hero
                }
                
                newR = getRow(); // row is unchanged
            }

            // checks if hero and monster are in the same position
            if (hRow == getRow() && hCol == getCol()) {
                // monster's position is unchanged
                newR = getRow();
                newC = getCol();
            }           
        }
    
    private:
        int power; // power = 1 for Monster, power = 2 for SuperMonster

}; // Monster

class Bat: public BoardCell {
    public:
		Bat(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual bool isBaddie( ) {return true;}
        virtual bool isStatic( ) {return false;}
        virtual char display( ) {return '~';}

        /*
        This function gets the attempted move for Bat. Bats always attempt to navigate to the hero's 
        column, but cannot change rows.
        Parameters:
            newR - new position for row
            newC - new position for column
            hRow - hero's row position
            hCol - hero's column position
        Return:
            Passes by reference newR and newC to retrieve new positions for attempted move.
        */
    	virtual void attemptMoveTo(size_t& newR, size_t& newC, size_t hRow, size_t hCol) {
  
            newR = getRow(); // row is unchanged
            newC = hCol; // Bat moves to the hero's column          
        }

}; // Bat


class Abyss: public BoardCell {
	public:
    	Abyss(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '#';}
        virtual bool isHole( ) {return true;}
}; // Abyss


class Wall: public BoardCell {
	public:
    	Wall(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '+';}
    	virtual bool isBarrier( ) {return true;}
}; // Wall


class Nothing: public BoardCell {
	public:
    	Nothing(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return ' ';}
    	virtual bool isSpace( ) {return true;}
}; // Nothing


class EscapeLadder: public BoardCell {
	public:
    	EscapeLadder(size_t r, size_t c) {
            setRow(r);
            setCol(c);
        }
        virtual char display( ) {return '*';}
    	virtual bool isExit( ) {return true;}
}; // EscapeLadder


#endif //_BOARDCELL_H
