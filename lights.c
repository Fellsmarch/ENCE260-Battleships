/** @file   lights.c
    @author Harrison Cook, hgc25
    @date   11 October 2018
    @brief  Display lights for battleships game
*/

//Possibly create fast_flash and flash functions and maybe add wrappers to them to add_shot (return hit / miss)
#include "system.h"
#include "tinygl.h"
#include <stdio.h>
#include "lights.h"

/** The number of LEDs in the matrix  */
#define NUM_LEDS 35
/** Where a light is solid or flashing  */
#define SOLID 0
#define FLASHING 1
/** Where the screen is attack or defense  */
#define ATK 0
#define DEF 1

int numFlashingAtk = 0;
int numSolidAtk = 0;
tinygl_point_t solidPointsAtk[NUM_LEDS];
tinygl_point_t flashingPointsAtk[NUM_LEDS];

int numFlashingDef = 0;
int numSolidDef = 0;
tinygl_point_t solidPointsDef[NUM_LEDS];
tinygl_point_t flashingPointsDef[NUM_LEDS];

// solidPointsAtk[0] = tinygl_point(4,6); solidPointsAtk[1] = tinygl_point(2,6);
// solidPointsAtk[2] = tinygl_point(2,4); solidPointsAtk[3] = tinygl_point(3,3);
// numSolidAtk += 2; numSolidAtk += 2;
//
// flashingPointsAtk[0] = tinygl_point(1,5); flashingPointsAtk[1] = tinygl_point(3,0);
// flashingPointsAtk[2] = tinygl_point(0,3); flashingPointsAtk[3] = tinygl_point(4,3);
// numFlashingAtk += 4;

/** Display points on LED matrix
    @param points array of points to display
    @param numPoints number points to display
    @return this never returns.
*/
void displayPoints (tinygl_point_t points[], int numPoints)
{
    int i = 0;
    for (; i < numPoints; i++) {
        tinygl_draw_point(points[i], 1);
    }
}

/** Hide points on LED matrix
    @param points array of points to hide
    @param numPoints number points to hide
    @return this never returns.
*/
void hidePoints (tinygl_point_t points[], int numPoints)
{
    int i = 0;
    for (; i < numPoints; i++) {
        tinygl_draw_point(points[i], 0);
    }
}

/** Adds a point to the corresponding array
    @param point point to add
    @param lightType type of light (flashing or solid)
    @param screen what screen the point should be added to (atk/def)
    @return this never returns.
*/
//Could make a function that finds the array and returns a pointer to it instead of doing it in this function
void addPoint(tinygl_point_t point, int lightType, int screen) { //lightType either SOLID (0) or FLASHING(1) screen either ATK (0) or DEF (1)
    tinygl_point_t* points;
    int* numPoints;
    int i = 0;

    if (lightType == SOLID && screen == ATK) {
        points = solidPointsAtk;
        numPoints = &numSolidAtk;
    } else if (lightType == FLASHING && screen == ATK) {
        points = flashingPointsAtk;
        numPoints = &numFlashingAtk;
    } else if (lightType == SOLID && screen == DEF) {
        points = solidPointsDef;
        numPoints = &numSolidDef;
    } else if (lightType == FLASHING && screen == DEF) {
        points = flashingPointsDef;
        numPoints = &numFlashingDef;
    }

    //Checks whether the point is in the array already (replaces in function)
    bool pointFound = false;
    for (; i < *numPoints; i++) {
        if (point.x == points[i].x && point.y == points[i].y) {
            pointFound = true;
        }
    }
    if (!pointFound) {
        points[*numPoints] = point;
        *numPoints = *numPoints + 1;
    }
}

void flashLights(int lightOn) { //, int screen) { //Screen will be ATK/DEF
    int flashWait_on = 100;
    int flashWait_off = 0;
    int on = 0; //Flashing lights on / off
    static int duration = 100;
    if (lightOn) { //If flashing lights are currently on
        if (flashWait_on == 0) {
            lightOn = 0;
            hidePoints(flashingPointsAtk, numFlashingAtk);
            flashWait_off = duration;
        } else {flashWait_on--;}
    } else { //If flashing lights are currently off
        if (flashWait_off == 0) {
            lightOn = 1;
            displayPoints(flashingPointsAtk, numFlashingAtk);
            flashWait_on = duration;
        } else {
            flashWait_off--;
        }
    }
}

void flashLights_fast() {
    int newShot = 0;
    int newShot_On = 0;
    int newShot_Off = 40;
    int newShotDisplaying = 0;
    tinygl_point_t newShotPos[1];

}
