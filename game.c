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

#define DISPLAY_TASK_RATE 250
#define SOLID_LED_RATE 250
#define FLASHING_LED_RATE 250
#define WAIT_TIME 250

enum { //These will be automatically numbered by enum
    r0_c0, r0_c1, r0_c2, r0_c3, r0_c4, //Row 0
    r1_c0, r1_c1, r1_c2, r1_c3, r1_c4, //Row 1
    r2_c0, r2_c1, r2_c2, r2_c3, r2_c4, //Row 2
    r3_c0, r3_c1, r3_c2, r3_c3, r3_c4, //Row 3
    r4_c0, r4_c1, r4_c2, r4_c3, r4_c4, //Row 4
    r5_c0, r5_c1, r5_c2, r5_c3, r5_c4, //Row 5
    r6_c0, r6_c1, r6_c2, r6_c3, r6_c4, //Row 6
};
// #define NUM_LEDS 35
// #define SOLID 0
// #define FLASHING 1
// #define ATK 0
// #define DEF 1
//
// int numFlashingAtk = 0;
// int numSolidAtk = 0;
// tinygl_point_t solidPointsAtk[NUM_LEDS];
// tinygl_point_t flashingPointsAtk[NUM_LEDS];
//
// int numFlashingDef = 0;
// int numSolidDef = 0;
// tinygl_point_t solidPointsDef[NUM_LEDS];
// tinygl_point_t flashingPointsDef[NUM_LEDS];
//
//
// static void display_points (tinygl_point_t points[], int numPoints)
// {
//     int i = 0;
//     for (; i < numPoints; i++) {
//         tinygl_draw_point(points[i], 1);
//     }
// }
//
// static void hidePoints (tinygl_point_t points[], int numPoints)
// {
//     int i = 0;
//     for (; i < numPoints; i++) {
//         tinygl_draw_point(points[i], 0);
//     }
// }

// static void addPoint(tinygl_point_t point, int lightType, int screen) { //lightType either SOLID (0) or FLASHING(1) screen either ATK (0) or DEF (1)
//     tinygl_point_t* points;
//     int* numPoints;
//
//     if (lightType == SOLID && screen == ATK) {
//         points = solidPointsAtk;
//         numPoints = &numSolidAtk;
//     } else if (lightType == FLASHING && screen == ATK) {
//         points = flashingPointsAtk;
//         numPoints = &numFlashingAtk;
//     } else if (lightType == SOLID && screen == DEF) {
//         points = solidPointsDef;
//         numPoints = &numSolidDef;
//     } else if (lightType == FLASHING && screen == DEF) {
//         points = flashingPointsDef;
//         numPoints = &numFlashingDef;
//     }
//
//     //Checks whether the point is in the array already (replaces in function)
//     bool pointFound = false;
//     for (; i < *numPoints; i++) {
//         if (point.x == points[i].x && point.y == points[i].y) {
//             pointFound = true;
//         }
//     }
//     if (!pointFound) {
//         points[*numPoints] = point;
//         *numPoints = *numPoints + 1;
//     }
// }

//We can make ship placement run its own while loop until ships have been placed
int main (void)
{
    solidPointsAtk[0] = tinygl_point(4,6); solidPointsAtk[1] = tinygl_point(2,6);
    solidPointsAtk[2] = tinygl_point(2,4); solidPointsAtk[3] = tinygl_point(3,3);
    numSolidAtk += 2; numSolidAtk += 2;

    flashingPointsAtk[0] = tinygl_point(1,5); flashingPointsAtk[1] = tinygl_point(3,0);
    flashingPointsAtk[2] = tinygl_point(0,3); flashingPointsAtk[3] = tinygl_point(4,3);
    numFlashingAtk += 4;

    //Initialisation
    system_init ();
    pacer_init (250);
    tinygl_init (DISPLAY_TASK_RATE);
    timer_init();
    navswitch_init();



    int newShot = 0;
    int newShot_On = 0;
    int newShot_Off = 40;
    int newShotDisplaying = 0;
    tinygl_point_t newShotPos[1];

    int flashWait_on = 100;
    int flashWait_off = 0;
    int on = 0; //Flashing lights on / off

    int solidUpdated = 1;

    while (1)
        {
            pacer_wait ();
            tinygl_update ();
            navswitch_update ();

            if (solidUpdated) {
                displayPoints(solidPointsAtk, numSolidAtk);
                solidUpdated = 0;
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
                newShot = 5;
                newShotPos[0] = tinygl_point(0, 0);
                //tinygl_draw_line(tinygl_point(0, 0), tinygl_point(4,4), 1);
            }
       }
}
