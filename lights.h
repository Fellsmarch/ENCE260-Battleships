/** @file   lights.h
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
int numFlashingAtk;
int numSolidAtk;
tinygl_point_t solidPointsAtk[NUM_LEDS];
tinygl_point_t flashingPointsAtk[NUM_LEDS];

/** The arrays for the defending screen, with seperate arrays for flashing and solid lights */
int numFlashingDef;
int numSolidDef;
tinygl_point_t solidPointsDef[NUM_LEDS];
tinygl_point_t flashingPointsDef[NUM_LEDS];

/** Display points on the LED matrix using tinygl
    @param points array of points to display
    @param numPoints number points to display
    @return this never returns.
*/
void displayPoints (tinygl_point_t points[], int numPoints);

/** Hide points on the LED matrix using tinygl
    @param points array of points to hide
    @param numPoints number points to hide
    @return this never returns.
*/
void hidePoints (tinygl_point_t points[], int numPoints);

/** Finds the corresponding containers (arrays) for the requested light type and screen
    @param numPoints the number of points in the array
    @param lightType the type of light to requested, either FLASHING or SOLID
    @param screen the screen requested, either DEF (defense) or ATK (attack)
    @return a pointer to the corresponding array.
*/
tinygl_point_t* getContainers(int** numPoints, int lightType, int screen);

/** Adds a point to the corresponding array
    @param point point to add
    @param lightType type of light (flashing or solid)
    @param screen what screen the point should be added to (atk/def)
    @return this never returns.
*/
void addPoint(tinygl_point_t point, int lightType, int screen);

/** Returns true if the point is in the corresponding array
    @param point point to add
    @param lightType type of light (flashing or solid)
    @param screen what screen the point should be added to (atk/def)
    @return true (1) or false (0)
*/
int in(tinygl_point_t point, int lightType, int screen);

/** Flashes lights on and off
    @param screen the screen to flash the lights for (ATK/DEF)
    @return this never returns
*/
void flashLights(int screen);

/** Infinitly scrolls the result of the game on the display
    @param result the game result (WIN or LOSS)
    @return this never returns
*/
void displayResult(int result);

/** Displays some given text as scrolling text on the display
    @param text the text to display
    @return this never returns
*/
void displayText(char* text);
