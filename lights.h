/** @file   lights.h
    @author Harrison Cook, hgc25
    @date   11 October 2018
    @brief  Display lights for battleships game

*/

#include "system.h"
#include "tinygl.h"
#include <stdio.h>

/** The number of LEDs in the matrix  */
#define NUM_LEDS 35
/** Where a light is solid or flashing  */
#define SOLID 0
#define FLASHING 1
/** Where the screen is attack or defense  */
#define ATK 0
#define DEF 1

int numFlashingAtk;
int numSolidAtk;
tinygl_point_t solidPointsAtk[];
tinygl_point_t flashingPointsAtk[];

int numFlashingDef;
int numSolidDef;
tinygl_point_t solidPointsDef[];
tinygl_point_t flashingPointsDef[];

/** Display points on LED matrix
    @param points array of points to display
    @param numPoints number points to display
    @return this never returns.
*/
void displayPoints (tinygl_point_t points[], int numPoints);

/** Hide points on LED matrix
    @param points array of points to hide
    @param numPoints number points to hide
    @return this never returns.
*/
void hidePoints (tinygl_point_t points[], int numPoints);

/** Adds a point to the corresponding array
    @param point point to add
    @param lightType type of light (flashing or solid)
    @param screen what screen the point should be added to (atk/def)
    @return this never returns.
*/
void addPoint(tinygl_point_t point, int lightType, int screen);
