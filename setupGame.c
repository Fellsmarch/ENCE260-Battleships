/** @file   setupGame.c
    @author Ryan Chen, rch141
    @author Harrison Cook, hgc25
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
struct ship_s {
    int col1;
    int row1;
    int col2;
    int row2;
};

/** Create a object of type ship struct from a set of coordinates
    @param col1 column coordinate for first point
    @param row1 row coordinate for first point
    @param col2 column coordinate for second point
    @param row2 row coordinate for second point
    @return a new ship object.
*/
Ship createShip(int col1, int row1, int col2, int row2)
{
    Ship ship = {col1, row1, col2, row2};
    return ship;
}

/** Finds all points in a straight line between (and including) two given points
    @param points the array of points between (and including) the given points
    @param shipBounds the two points that make up the borders of the line
    @return this never returns
*/
void getPoints(tinygl_point_t* points, int* shipBounds) {
    int col1 = shipBounds[0];
    int row1 = shipBounds[1];
    int col2 = shipBounds[2];
    int row2 = shipBounds[3];
    int i = 0;

    //If the ship is vertical
    if (col1 == col2) {
        if (row1 < row2) { //If the first row coordinate is less than the second
            while (row1 <= row2) {
                points[i] = tinygl_point(col1, row1);
                row1++;
                i++;
            }

        } else if (row1 >= row2) {
            while (row1 >= row2) {
                points[i] = tinygl_point(col1, row2);
                row2++;
                i++;
            }

        }
    } else { //If the ship is horizontal
        if (col1 < col2) {
            while (col1 <= col2) {
                points[i] = tinygl_point(col1, row1);
                col1++;
                i++;
            }

        } else if (col1 >= col2) {
            while (col1 >= col2) {
                points[i] = tinygl_point(col2, row1);
                col2++;
                i++;
            }
        }
    }
}

/** Draws a ship on the screen
    @param ship the ship object to display
    @param toggle whether or not the ship is to be displayed or hidden
    @return this never returns
*/
void drawShip(Ship ship, int toggle)
{
    tinygl_draw_line(tinygl_point(ship.col1, ship.row1), tinygl_point(ship.col2, ship.row2), toggle);
}

/** Checks whether a set of coordinates are inside the LED matrix column dimensions
    @param col1 column coordinate for first point
    @param col2 column coordinate for second point
    @return true / false
*/
int checkCol(int col1, int col2)
{
    if (col1 < 0 || col2 < 0) {return 0;} //False
    else if (col1 > 4 || col2 > 4) {return 0;} //False
    else {return 1;} //True
}

/** Checks whether a set of coordinates are inside the LED matrix row dimensions
    @param row1 row coordinate for first point
    @param row2 row coordinate for second point
    @return true / false
*/
int checkRow(int row1, int row2)
{
    if (row1 < 0 || row2 < 0) {return 0;}
    else if (row1 > 6 || row2 > 6) {return 0;}
    else {return 1;}
}

/** Moves a ship north if it is a valid move
    @param ship pointer to a ship object to move
    @return this never returns
*/
void moveShipNorth(Ship *ship)
{
    drawShip(*ship, 0);

    if (checkRow(ship->row1 - 1, ship->row2 - 1)) {
        ship->row1--;
        ship->row2--;
    }

    drawShip(*ship, 1);
}

/** Moves a ship south if it is a valid move
    @param ship pointer to a ship object to move
    @return this never returns
*/
void moveShipSouth(Ship *ship)
{
    drawShip(*ship, 0);

    if (checkRow(ship->row1 + 1, ship->row2 + 1)) {
        ship->row1++;
        ship->row2++;
    }

    drawShip(*ship, 1);
}

/** Moves a ship east if it is a valid move
    @param ship pointer to a ship object to move
    @return this never returns
*/
void moveShipEast(Ship *ship)
{
    drawShip(*ship, 0);

    if (checkCol(ship->col1 + 1, ship->col2 + 1)) {
        ship->col1++;
        ship->col2++;
    }

    drawShip(*ship, 1);
}

/** Moves a ship west if it is a valid move
    @param ship pointer to a ship object to move
    @return this never returns
*/
void moveShipWest(Ship *ship)
{
    drawShip(*ship, 0);

    if (checkCol(ship->col1 - 1, ship->col2 - 1)) {
        ship->col1--;
        ship->col2--;
    }

    drawShip(*ship, 1);
}

/** Rotates a ship on the LED matrix
    @param ship pointer to a ship object to rotateShip
    @param shipSize the length of the ship
    @return this never returns
*/
void rotateShip(Ship *ship, int shipSize)
{
    drawShip(*ship, 0);
    if (ship->col1 != ship->col2) {
        ship->col1 = 2;
        ship->row1 = 2;
        ship->col2 = 2;
        ship->row2 = shipSize + 1;
    } else if (ship->row1 != ship->row2) {
        ship->col1 = 1;
        ship->row1 = 3;
        ship->col2 = shipSize;
        ship->row2 = 3;
    }
    drawShip(*ship, 1);
}

/** Allows the user to place a ship and fills toReturn with the coordinates of the placed ship
    @param shipSize the length of the ship
    @param toReturn array to hold coordinates of the placed ship
    @return this never returns
*/
void setupShip(int shipSize, int* toReturn)//(int* toReturn, int shipSize)//, int* toDisplay, int numPoints)
{

    Ship ship = createShip(2, 2, 2, shipSize + 1);
    drawShip(ship, 1);

    while (1)
        {
            //TODO: Will need to pace these as well
            pacer_wait ();
            tinygl_update ();
            navswitch_update ();
            button_update ();

            //TODO: Need to change so these only get checked every x loops
            if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
                moveShipNorth(&ship);
            }

            if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                moveShipSouth(&ship);
            }

            if (navswitch_push_event_p (NAVSWITCH_EAST)) {
                moveShipEast(&ship);
            }

            if (navswitch_push_event_p (NAVSWITCH_WEST)) {
                moveShipWest(&ship);
            }

            if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                rotateShip(&ship, shipSize);
            }
            if (button_push_event_p (0)) {
                drawShip(ship, 0);
                toReturn[0] = ship.col1; toReturn[1] = ship.row1;
                toReturn[2] = ship.col2; toReturn[3] = ship.row2;
                break;
            }

            displayPoints(solidPointsDef, numSolidDef);

        }
}

/** The function which places both of the ships
    @param this takes no parameters
    @return this never returns
*/
void placeShips(void)
{
    int secondShipPlaced = 0; //False
    int firstShipBounds[4]; //The four coordinates for the two points of the ship
    int secondShipBounds[4];
    int j = 0;
    int i = 0;

    setupShip(FIRSTSHIPSIZE, firstShipBounds); //Get first ship coordinates from user

    tinygl_point_t firstShip[FIRSTSHIPSIZE];
    getPoints(firstShip, firstShipBounds); //Get all points that will display the ship on the LED matrix

    //Add the ship to the correct display array
    for (; j < FIRSTSHIPSIZE; j++) {
        addPoint(firstShip[j], SOLID, DEF);
    }

    tinygl_point_t secondShip[SECONDSHIPSIZE];

    //Get second ship coordinates
    //While loop ensures that if the placement isn't valid, it won't let the user place there
    while(!secondShipPlaced) {
        setupShip(SECONDSHIPSIZE, secondShipBounds);
        getPoints(secondShip, secondShipBounds);

        i = 0;
        secondShipPlaced = 1; //True
        //Checks if any of the points from either ship are identical
        for (; i < SECONDSHIPSIZE; i++) {
            j = 0;
            for (; j < FIRSTSHIPSIZE; j++) {
                if (secondShip[i].x == firstShip[j].x && secondShip[i].y == firstShip[j].y) {
                    secondShipPlaced = 0; //False
                }
            }
        }
    }

    //Add second ship points to display array
    i = 0;
    for (; i < SECONDSHIPSIZE; i++) {
        addPoint(secondShip[i], SOLID, DEF);
    }

}
