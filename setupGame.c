#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "stdio.h"
#include "navswitch.h"
#include "button.h"
#include "pio.h"

typedef struct ship_s Ship;
struct ship_s {
    int x1;
    int y1;
    int x2;
    int y2;
};



Ship createShip(int x1, int y1, int x2, int y2) {
    Ship ship = {x1, y1, x2, y2};
    return ship;
}


static void makeShip(Ship ship, int toggle)
{
    tinygl_draw_line(tinygl_point(ship.x1, ship.y1), tinygl_point(ship.x2, ship.y2), toggle);
}

static int checkX(int x1, int x2) {
    if (x1 < 0 || x2 < 0) {return 0;}
    else if (x1 > 4 || x2 > 4) {return 0;}
    else {return 1;}
}

static int checkY(int y1, int y2) {
    if (y1 < 0 || y2 < 0) {return 0;}
    else if (y1 > 6 || y2 > 6) {return 0;}
    else {return 1;}
}

static void moveShipNorth(Ship *ship)
{
    makeShip(*ship, 0);

    if (checkY(ship->y1 - 1, ship->y2 - 1)) {
        ship->y1--;
        ship->y2--;
    }

    makeShip(*ship, 1);
}

static void moveShipSouth(Ship *ship)
{
    makeShip(*ship, 0);

    if (checkY(ship->y1 + 1, ship->y2 + 1)) {
        ship->y1++;
        ship->y2++;
    }

    makeShip(*ship, 1);
}

static void moveShipEast(Ship *ship)
{
    makeShip(*ship, 0);

    if (checkX(ship->x1 + 1, ship->x2 + 1)) {
        ship->x1++;
        ship->x2++;
    }

    makeShip(*ship, 1);
}

static void moveShipWest(Ship *ship)
{
    makeShip(*ship, 0);

    if (checkX(ship->x1 - 1, ship->x2 - 1)) {
        ship->x1--;
        ship->x2--;
    }

    makeShip(*ship, 1);
}

void rotateShip(Ship *ship, int sizeShip)
{
    makeShip(*ship, 0);
    if (ship->x1 != ship->x2) {
        ship->x1 = 2;
        ship->y1 = 2;
        ship->x2 = 2;
        ship->y2 = sizeShip + 1;
    } else if (ship->y1 != ship->y2) {
        ship->x1 = 1;
        ship->y1 = 3;
        ship->x2 = sizeShip;
        ship->y2 = 3;
    }
    makeShip(*ship, 1);
}

/*
void getCoord(Ship ship, int[] coords) {

}
*/

void setup (int* toReturn, int sizeShip)
{
    system_init ();
    pacer_init (1000);
    tinygl_init (1000);
    navswitch_init ();
    button_init();
    Ship ship = createShip(0, 1, 0, sizeShip);
    makeShip(ship, 1);

    while (1)
        {
            pacer_wait ();
            tinygl_update ();
            navswitch_update ();
            button_update ();

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
                rotateShip(&ship, sizeShip);
            }
            if (button_push_event_p (0)) {
                makeShip(ship, 0);
                toReturn[0] = ship.x1; toReturn[1] = ship.y1;
                toReturn[2] = ship.x2; toReturn[3] = ship.y2;
                return toReturn;
            }

        }
}
// int main(void)
// {
//     int test[4];
//     setup(test, 4);
// }
