#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "task.h"
#include <stdio.h>
#include "timer.h"
#include "../fonts/font3x5_1.h"
#include <stdbool.h>
#include "navswitch.h"
#include "lights.h"
#include "setupGame.h"

#define DISPLAY_TASK_RATE 250
#define SOLID_LED_RATE 250
#define FLASHING_LED_RATE 250
#define WAIT_TIME 250




int main (void)
{
    //solidPointsAtk[0] = tinygl_point(4,6); solidPointsAtk[1] = tinygl_point(3,6);
    // solidPointsAtk[2] = tinygl_point(2,4); solidPointsAtk[3] = tinygl_point(3,3);
    //numSolidAtk += 2; //numSolidAtk += 2;
    //
    flashingPointsAtk[0] = tinygl_point(1,5); flashingPointsAtk[1] = tinygl_point(3,0);
    flashingPointsAtk[2] = tinygl_point(0,3); flashingPointsAtk[3] = tinygl_point(4,3);
    numFlashingAtk += 4;

    //Initialisation
    system_init ();
    pacer_init (DISPLAY_TASK_RATE);
    tinygl_init (DISPLAY_TASK_RATE);
    timer_init();
    navswitch_init();
    button_init();

    int newShotFlashes = 5; //5 * 2
    tinygl_point_t newShot = tinygl_point(0, 0);

    int solidUpdated = 1;

    int currentScreen = DEF;

    while (1)
        {
            pacer_wait ();

            navswitch_update ();

            //TODO: Make all these (solid + flashing) update at 250hz
            tinygl_update ();
            if (solidUpdated) {
                if (currentScreen == ATK) {
                    displayPoints(solidPointsAtk, numSolidAtk);
                } else if (currentScreen == DEF) {
                    displayPoints(solidPointsDef, numSolidDef);
                }

                solidUpdated = 1;
            }



            //Flashes the flashing lights
            flashLights(ATK);

            //Quick flashes the new light/shot
            if (newShotFlashes > 0) {
                if (fastFlash(newShot)) {
                    newShotFlashes--;
                }
                if (newShotFlashes == 0) {
                    addPoint(newShot, FLASHING, DEF); //DEF hardcoded rn
                }
            }


            if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                placeShips();

                // displayPoints(solidPointsAtk, numSolidAtk);
                if (in(tinygl_point(0, 0), SOLID, DEF)) {
                    addPoint(tinygl_point(4,6), SOLID, DEF);
                }
                //displayPoints(solidPointsDef, numSolidDef);
                solidUpdated = 1;
            }
       }
}
