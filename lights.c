/** @file   lights.c
    @author Harrison Cook, hgc25
    @author Ryan Chen, rch141
    @date   11 October 2018
    @brief  Display lights for battleship game
*/

#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include <stdio.h>
#include <string.h>

/** The duration that flashing lights will stay off/on for */
#define DURATION 100
/** The number of LEDs in the matrix  */
#define NUM_LEDS 35
/** Where a light is solid or flashing  */
#define SOLID 0
#define FLASHING 1
/** Where the screen is the attacking screen or defense screen */
#define ATK 0
#define DEF 1
/** Game won or lost */
#define LOSS 0
#define WIN 1

/** The arrays for the attacking screen, with seperate arrays for flashing and solid lights */
int numFlashingAtk = 0;
int numSolidAtk = 0;
tinygl_point_t solidPointsAtk[NUM_LEDS];
tinygl_point_t flashingPointsAtk[NUM_LEDS];

/** The arrays for the defending screen, with seperate arrays for flashing and solid lights */
int numFlashingDef = 0;
int numSolidDef = 0;
tinygl_point_t solidPointsDef[NUM_LEDS];
tinygl_point_t flashingPointsDef[NUM_LEDS];

/** Display points on the LED matrix using tinygl
    @param points array of points to display
    @param numPoints number points to display
    @return this never returns
*/
void displayPoints (tinygl_point_t points[], int numPoints)
{
    int i = 0;
    for (; i < numPoints; i++) {
        tinygl_draw_point(points[i], 1);
    }
}

/** Hide points on the LED matrix using tinygl
    @param points array of points to hide
    @param numPoints number points to hide
    @return this never returns
*/
void hidePoints (tinygl_point_t points[], int numPoints)
{
    int i = 0;
    for (; i < numPoints; i++) {
        tinygl_draw_point(points[i], 0);
    }
}

/** Finds the corresponding containers (arrays) for the requested light type and screen
    @param numPoints the number of points in the array
    @param lightType the type of light to requested, either FLASHING or SOLID
    @param screen the screen requested, either DEF (defense) or ATK (attack)
    @return a pointer to the corresponding array
*/
tinygl_point_t* getContainers(int** numPoints, int lightType, int screen)
{
    if (lightType == SOLID && screen == ATK) {
        *numPoints = &numSolidAtk;
        return solidPointsAtk;
    } else if (lightType == FLASHING && screen == ATK) {
        *numPoints = &numFlashingAtk;
        return flashingPointsAtk;
    } else if (lightType == SOLID && screen == DEF) {
        *numPoints = &numSolidDef;
        return solidPointsDef;
    } else {
        *numPoints = &numFlashingDef;
        return flashingPointsDef;
    }
}

/** Adds a point to the corresponding array
    @param point point to add
    @param lightType type of light (flashing or solid)
    @param screen what screen the point should be added to (atk/def)
    @return this never returns.
*/
void addPoint(tinygl_point_t point, int lightType, int screen)
{
    tinygl_point_t* points;
    int* numPoints;
    int i = 0;

    points = getContainers(&numPoints, lightType, screen);

    //Checks if the point is already in the array, if it is, it does not add it
    //Decided not to use the in function here as it would recheck all the containers
    bool pointFound = false;
    for (; i < *numPoints; i++) {
        if (point.x == points[i].x && point.y == points[i].y) {
            pointFound = true;
        }
    }
    if (!pointFound) { //If point not in the array
        points[*numPoints] = point;
        *numPoints = *numPoints + 1;
    }
}

/** Returns true if the point is in the corresponding array
    @param point point to add
    @param lightType type of light (flashing or solid)
    @param screen what screen the point should be added to (atk/def)
    @return true (1) or false (0)
*/
int in(tinygl_point_t point, int lightType, int screen)
{
    tinygl_point_t* points;
    int* numPoints;
    int i = 0;

    points = getContainers(&numPoints, lightType, screen);

    //Checks whether the point is in the array already
    for (; i < *numPoints; i++) {
        if (point.x == points[i].x && point.y == points[i].y) {
            return 1;
        }
    }
    return 0;
}

/** Flashes lights on and off
    @param screen the screen to flash the lights for (ATK/DEF)
    @return this never returns
*/
void flashLights(int screen)
{
    //Need to be static as this function gets called hundreds of times
    static int lightOn = 0; //Are the lights currently on
    static int flashWait_on = DURATION; //The time to wait until lights are turned off
    static int flashWait_off = 0; //The time to wait until lights are turned on

    tinygl_point_t* points = 0;
    int numPoints = 0;

    //Gets the correct array to use
    if (screen == ATK) {
        points = flashingPointsAtk;
        numPoints = numFlashingAtk;
    } else if (screen == DEF) {
        points = flashingPointsDef;
        numPoints = numFlashingDef;
    }

    if (lightOn) { //If flashing lights are currently on
        if (flashWait_on == 0) {
            lightOn = 0;
            hidePoints(points, numPoints);
            flashWait_off = DURATION;
        } else {flashWait_on--;}
    } else { //If flashing lights are currently off
        if (flashWait_off == 0) {
            lightOn = 1;
            displayPoints(points, numPoints);
            flashWait_on = DURATION;
        } else {flashWait_off--;}
    }
}

/** Infinitly scrolls the result of the game on the display
    @param result the game result (WIN or LOSS)
    @return this never returns
*/
void displayResult(int result)
{
    tinygl_clear();
    if (result == LOSS) {tinygl_text("  YOU WIN");} //Two spaces so it appears to scroll from right
    else {tinygl_text("  YOU LOSE");}

    //Loop forever with scrolling result
    while(1) {
        pacer_wait();
        tinygl_update();
    }
}

/** Displays some given text as scrolling text on the display
    @param text the text to display
    @return this never returns
*/
void displayText(char* text)
{
    int wait = 440 + (strlen(text) * 135); //Calculate the time it takes to do one scroll across hte screen
    tinygl_clear();
    tinygl_text(text);
    while (wait > 0) {
        pacer_wait();
        tinygl_update();
        wait--;
    }
}
