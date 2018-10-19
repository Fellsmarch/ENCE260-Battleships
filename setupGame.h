/** @file   setupGame.h
    @author Harrison Cook, hgc25
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

#define FIRSTSHIPSIZE 4
#define SECONDSHIPSIZE 2
#define TOTALSHIPSIZE FIRSTSHIPSIZE + SECONDSHIPSIZE

/**
    Define a Ship struct, which holds four coordinates that are the bounds
    of the ship
*/
typedef struct ship_s Ship;

/** Create a object of type ship struct from a set of coordinates
    @param col1 column coordinate for first point
    @param row1 row coordinate for first point
    @param col2 column coordinate for second point
    @param row2 row coordinate for second point
    @return a new ship object.
*/
Ship createShip(int col1, int row1, int col2, int row2);

/** Finds all points in a straight line between (and including) two given points
    @param points the array of points between (and including) the given points
    @param shipBounds the two points that make up the borders of the line
    @return this never returns
*/
void getPoints(tinygl_point_t* array, int* shipBounds);

/** Draws a ship on the screen
    @param ship the ship object to display
    @param toggle whether or not the ship is to be displayed or hidden
    @return this never returns
*/
void drawShip(Ship ship, int toggle);

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

/** Rotates a ship on the LED matrix
    @param ship pointer to a ship object to rotateShip
    @param shipSize the length of the ship
    @return this never returns
*/
void rotateShip(Ship *ship, int shipSize);

/** Allows the user to place a ship and fills toReturn with the coordinates of the placed ship
    @param shipSize the length of the ship
    @param toReturn array to hold coordinates of the placed ship
    @return this never returns
*/
 void setupShip(int shipSize, int* toReturn);//;, int* toDisplay, int numPoints)

 /** The function which places both of the ships
     @param this takes no parameters
     @return this never returns
 */
 void placeShips(void);
