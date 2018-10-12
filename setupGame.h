/** @file   setupGame.h
    @author Ryan Chen, rch141
    @date   12 October 2018
    @brief  Do first time setup of battleships game
*/

#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "stdio.h"
#include "navswitch.h"
#include "button.h"
#include "pio.h"
#include "lights.h"

typedef struct ship_s Ship;

/** Create a ship struct from a set of coordinates
    @param col1 column coordinate for first point
    @param row1 row coordinate for first point
    @param col2 column coordinate for second point
    @param row2 row coordinate for second point
    @return a ship object.
*/
Ship createShip(int col1, int row1, int col2, int row2);

/** Display a ship on the screen
    @param ship the ship object to display
    @param toggle whether or not the ship is to be displayed or hidden
    @return this never returns
*/
void makeShip(Ship ship, int toggle);

/** Checks whether a set of coordinates are inside the LED matrix column dimensions
    @param col1 column coordinate for first point
    @param col2 column coordinate for second point
    @return true / false
*/
int checkCol(int col1, int col2);

/** Checks whether a set of coordinates are inside the LED matrix row dimensions
    @param row1 row coordinate for first point
    @param row2 row coordinate for second point
    @return true / false
*/
int checkRow(int row1, int row2);

/** Moves a ship north if it is a valid move
    @param ship pointer to a ship object to move
    @return this never returns
*/
void moveShipNorth(Ship *ship);

/** Moves a ship south if it is a valid move
    @param ship pointer to a ship object to move
    @return this never returns
*/
void moveShipSouth(Ship *ship);

/** Moves a ship east if it is a valid move
    @param ship pointer to a ship object to move
    @return this never returns
*/
void moveShipEast(Ship *ship);

/** Moves a ship west if it is a valid move
    @param ship pointer to a ship object to move
    @return this never returns
*/
void moveShipWest(Ship *ship);

/** Rotates a ship
    @param ship pointer to a ship object to rotateShip
    @param shipSize the length of the ship
    @return this never returns
*/
void rotateShip(Ship *ship, int shipSize);

/** Allows the user to place a ship
    @param toReturn array to hold coordinates of the placed ship
    @param shipSize the length of the ship
    @param toDisplay array of lights to display to ensure that already placed ships are not removed
    @param numPoints number of points in the array
    @return this never returns
*/
 void setup (int* toReturn, int shipSize);//;, int* toDisplay, int numPoints)
