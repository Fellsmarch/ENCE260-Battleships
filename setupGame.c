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
    for (; i < numSolidAtk; i++) {
        if (pointsIdentical(solidPointsAtk[i], point1)
        || pointsIdentical(solidPointsAtk[i], point2)) {
            return 1;
        }
    }
    return 0;
}

void placeShips(void)
{
    int shipSize = 4;
    int secondShipPlaced = 0;
    Ship newShip;
    setup(shipSize);

    while(!secondShipPlaced) {


    }
}

void setup (int shipSize)//(int* toReturn, int shipSize)//, int* toDisplay, int numPoints)
{
    // system_init ();
    // pacer_init (1000);
    // tinygl_init (1000);
    // navswitch_init ();

    Ship ship = createShip(2, 2, 2, shipSize + 1);
    makeShip(ship, 1);

    while (1)
        {
            pacer_wait ();
            tinygl_update ();
            navswitch_update ();
            button_update ();

            //Need to change so these only get checked every x loops
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
                // firstShip.col1 = ship.col1; firstShip.row1 = ship.row1;
                // firstShip.col2 = ship.col2; firstShip.row2 = ship.row2;
                // return toReturn;
            }

            // tinygl_draw_point(tinygl_point(4,6), 1);
            // tinygl_draw_point(tinygl_point(3,6), 1);
            displayPoints(solidPointsAtk, numSolidAtk);

        }
}
