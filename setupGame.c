/** @file   setupGame.c
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
#include "setupGame.h"
#include "lights.h"

#define FIRSTSHIPSIZE 4
#define SECONDSHIPSIZE 2

typedef struct ship_s Ship;
struct ship_s {
    int col1;
    int row1;
    int col2;
    int row2;
};


Ship createShip(int col1, int row1, int col2, int row2)
{
    Ship ship = {col1, row1, col2, row2};
    return ship;
}

void getPoints(tinygl_point_t* array, int* shipBounds) {
    int col1 = shipBounds[0];
    int row1 = shipBounds[1];
    int col2 = shipBounds[2];
    int row2 = shipBounds[3];
    int i = 0;

    if (col1 == col2) {
        if (row1 < row2) {
            while (row1 <= row2) {
                array[i] = tinygl_point(col1, row1);
                row1++;
                i++;
            }

        } else if (row1 >= row2) {
            while (row1 >= row2) {
                array[i] = tinygl_point(col1, row2);
                row2++;
                i++;
            }

        }
    } else {
        if (col1 < col2) {
            while (col1 <= col2) {
                array[i] = tinygl_point(col1, row1);
                col1++;
                i++;
            }

        } else if (col1 >= col2) {
            while (col1 >= col2) {
                array[i] = tinygl_point(col2, row1);
                col2++;
                i++;
            }

        }
    }

}

void makeShip(Ship ship, int toggle)
{
    tinygl_draw_line(tinygl_point(ship.col1, ship.row1), tinygl_point(ship.col2, ship.row2), toggle);
    // tinygl_draw_point(tinygl_point(ship.col1, ship.row1), 1);
    // tinygl_draw_point(tinygl_point(ship.col2, ship.row2), 1);

}

int checkCol(int col1, int col2)
{
    if (col1 < 0 || col2 < 0) {return 0;}
    else if (col1 > 4 || col2 > 4) {return 0;}
    else {return 1;}
}

int checkRow(int row1, int row2)
{
    if (row1 < 0 || row2 < 0) {return 0;}
    else if (row1 > 6 || row2 > 6) {return 0;}
    else {return 1;}
}

void moveShipNorth(Ship *ship)
{
    makeShip(*ship, 0);

    if (checkRow(ship->row1 - 1, ship->row2 - 1)) {
        ship->row1--;
        ship->row2--;
    }

    makeShip(*ship, 1);
}

void moveShipSouth(Ship *ship)
{
    makeShip(*ship, 0);

    if (checkRow(ship->row1 + 1, ship->row2 + 1)){
        ship->row1++;
        ship->row2++;
    }

    makeShip(*ship, 1);
}

void moveShipEast(Ship *ship)
{
    makeShip(*ship, 0);

    if (checkCol(ship->col1 + 1, ship->col2 + 1)) {
        ship->col1++;
        ship->col2++;
    }

    makeShip(*ship, 1);
}

void moveShipWest(Ship *ship)
{
    makeShip(*ship, 0);

    if (checkCol(ship->col1 - 1, ship->col2 - 1)) {
        ship->col1--;
        ship->col2--;
    }

    makeShip(*ship, 1);
}

void rotateShip(Ship *ship, int shipSize)
{
    makeShip(*ship, 0);
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
    makeShip(*ship, 1);
}

int pointsIdentical(tinygl_point_t point1, tinygl_point_t point2)
{
    return (point1.x == point2.x && point1.y == point2.y);
}

int shipConflict(Ship ship)
{
    tinygl_point_t point1 = tinygl_point(ship.col1, ship.row1);
    tinygl_point_t point2 = tinygl_point(ship.col2, ship.row2);
    int i = 0;
    for (; i < numSolidDef; i++) {
        if (pointsIdentical(solidPointsDef[i], point1)
        || pointsIdentical(solidPointsDef[i], point2)) {
            return 1;
        }
    }
    return 0;
}

void placeShips(void)
{
    int firstShipSize = 4;
    int secondShipSize = 2;
    int secondShipPlaced = 0;

    int* firstShipBounds[FIRSTSHIPSIZE];
    int* secondShipBounds[SECONDSHIPSIZE];

    setup(FIRSTSHIPSIZE, firstShipBounds);

    tinygl_point_t firstShip[FIRSTSHIPSIZE];
    getPoints(firstShip, firstShipBounds);

    int j = 0;
    int i = 0;
    for (; j < FIRSTSHIPSIZE; j++) {
        addPoint(firstShip[j], SOLID, DEF);
    }

    tinygl_point_t secondShip[SECONDSHIPSIZE];

    //Get second ship points
    while(!secondShipPlaced) {
        setup(SECONDSHIPSIZE, secondShipBounds);
        getPoints(secondShip, secondShipBounds);

        i = 0;
        secondShipPlaced = 1;
        for (; i < SECONDSHIPSIZE; i++) {
            j = 0;
            for (; j < FIRSTSHIPSIZE; j++) {
                if (secondShip[i].x == firstShip[j].x && secondShip[i].y == firstShip[j].y) {
                    secondShipPlaced = 0;
                }
            }
        }
    }

    //Add second ship points to display arrays
    i = 0;
    for (; i < secondShipSize; i++) {
        addPoint(secondShip[i], SOLID, DEF);
    }

}

void setup (int shipSize, int* toReturn)//(int* toReturn, int shipSize)//, int* toDisplay, int numPoints)
{

    Ship ship = createShip(2, 2, 2, shipSize + 1);
    makeShip(ship, 1);

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
                makeShip(ship, 0);
                toReturn[0] = ship.col1; toReturn[1] = ship.row1;
                toReturn[2] = ship.col2; toReturn[3] = ship.row2;
                break;
            }

            displayPoints(solidPointsDef, numSolidDef);

        }
}
