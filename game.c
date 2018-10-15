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
    // flashingPointsAtk[0] = tinygl_point(1,5); flashingPointsAtk[1] = tinygl_point(3,0);
    // flashingPointsAtk[2] = tinygl_point(0,3); flashingPointsAtk[3] = tinygl_point(4,3);
    // numFlashingAtk += 4;

    //Initialisation
    system_init ();
    pacer_init (DISPLAY_TASK_RATE);
    tinygl_init (DISPLAY_TASK_RATE);
    timer_init();
    navswitch_init();
    button_init();



    int newShot = 0;
    int newShot_On = 0;
    int newShot_Off = 40;
    int newShotDisplaying = 0;
    tinygl_point_t newShotPos[1];

    int flashWait_on = 100;
    int flashWait_off = 0;
    int on = 0; //Flashing lights on / off

    int solidUpdated = 1;
    int firstPlace = 0;

    int currentScreen = DEF;

    while (1)
        {
            pacer_wait ();
            tinygl_update ();
            navswitch_update ();

            if (solidUpdated) {
                if (currentScreen == ATK) {
                    displayPoints(solidPointsAtk, numSolidAtk);
                } else if (currentScreen == DEF) {
                    displayPoints(solidPointsDef, numSolidDef);
                }

                solidUpdated = 1;
            }



            //Flashes the flashing lights
            if (on) { //If flashing lights are currently on
                if (flashWait_on == 0) {
                    on = 0;
                    hidePoints(flashingPointsAtk, numFlashingAtk);
                    flashWait_off = 100;
                } else {flashWait_on--;}
            } else { //If flashing lights are currently off
                if (flashWait_off == 0) {
                    on = 1;
                    displayPoints(flashingPointsAtk, numFlashingAtk);
                    flashWait_on = 100;
                } else {
                    flashWait_off--;
                }
            }

            //Quick flashes the new light/shot
            if (newShot > 0) {
                if (newShotDisplaying) {
                    if (newShot_On == 0) {
                        newShotDisplaying = 0;
                        hidePoints(newShotPos, 1);
                        newShot_Off = 40;
                        newShot--;
                    } else {newShot_On--;}
                    // newShot--;

                } else {
                    if (newShot_Off == 0) {
                        newShotDisplaying = 1;
                        displayPoints(newShotPos, 1);
                        newShot_On = 40;
                    } else {newShot_Off--;}

                }

                //Add here instead of in else so it doesn't add duplicates
                if (newShot == 0) {
                    addPoint(newShotPos[0], FLASHING, ATK);
                }
            }

            if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                placeShips();

                displayPoints(solidPointsAtk, numSolidAtk);
                if (in(tinygl_point(0, 0), SOLID, DEF)) {
                    addPoint(tinygl_point(4,6), SOLID, DEF);
                }
                displayPoints(solidPointsDef, numSolidDef);
                solidUpdated = 1;
            }
       }
}
