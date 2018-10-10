#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "task.h"
#include <stdio.h>
#include "timer.h"
#include "../fonts/font3x5_1.h"
#include <stdbool.h>
#include "navswitch.h"

#define DISPLAY_TASK_RATE 250
#define SOLID_LED_RATE 250
#define FLASHING_LED_RATE 250
#define WAIT_TIME 250
#define NUM_LEDS 35
#define SOLID 0
#define FLASHING 1
#define ATK 0
#define DEF 1

int flashWait_on = 100;
int flashWait_off = 0;
int on = 0;

int numFlashingAtk = 0;
int numSolidAtk = 0;
tinygl_point_t solidPointsAtk[NUM_LEDS];
tinygl_point_t flashingPointsAtk[NUM_LEDS];

int numFlashingDef = 0;
int numSolidDef = 0;
tinygl_point_t solidPointsDef[NUM_LEDS];
tinygl_point_t flashingPointsDef[NUM_LEDS];


static void display_points (tinygl_point_t points[], int numPoints)
{
    int i = 0;
/*
    while (1) {
        if (points[i].x == NULL && points[i].y == NULL) {
            break;
        } else {
            tinygl_draw_point(points[i], 1);
            i++;
        }
    }
*/
    for (; i < numPoints; i++) {
        tinygl_draw_point(points[i], 1);
    }
}

static void hidePoints (tinygl_point_t points[], int numPoints)
{
    //size_t numPoints = sizeof(points) / sizeof(points[0]); //The size of the array
    int i = 0;
    for (; i < numPoints; i++) {
        tinygl_draw_point(points[i], 0);
    }
}


/*
static void wait_flash (void)
{
    float seconds = 500 * 0.001;
    float increments = seconds / 0.000128;
    //timer_delay_ms (500);
    timer_wait(increments);
}
*/

//Returns true (1) if the given point is already in the given lightType array
bool in(tinygl_point_t point, int lightType, int screen) { //screen either ATK (0) or DEF (1)

    tinygl_point_t* points;
    int numPoints;
    int i = 0;

    //Work out which array to check
    if (lightType == SOLID && screen == ATK) {
        points = solidPointsAtk;
        numPoints = numSolidAtk;
    } else if (lightType == FLASHING && screen == ATK) {
        points = flashingPointsAtk;
        numPoints = numFlashingAtk;
    } else if (lightType == SOLID && screen == DEF) {
        points = solidPointsDef;
        numPoints = numSolidDef;
    } else if (lightType == FLASHING && screen == DEF) {
        points = flashingPointsDef;
        numPoints = numFlashingDef;
    }

    for (; i < numPoints; i++) {
        if (point.x == points[i].x && point.y == points[i].y) {
            return true;
        }
    }
    return false;
}

static void addPoint(tinygl_point_t point, int lightType, int screen) { //lightType either SOLID (0) or FLASHING(1)
    tinygl_point_t* points;
    int* numPoints;

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

    points[*numPoints] = point;
    *numPoints = *numPoints + 1;
}

int main (void)
{
    solidPointsAtk[0] = tinygl_point(4,6); solidPointsAtk[1] = tinygl_point(2,6);
    solidPointsAtk[2] = tinygl_point(2,4); solidPointsAtk[3] = tinygl_point(3,3);
    numSolidAtk += 2; numSolidAtk += 2;

    flashingPointsAtk[0] = tinygl_point(1,5); flashingPointsAtk[1] = tinygl_point(3,0);
    flashingPointsAtk[2] = tinygl_point(0,3); flashingPointsAtk[3] = tinygl_point(4,3);
    numFlashingAtk += 4;
/*
    tinygl_point_t ghosting_points[] = {
        tinygl_point(0,0), tinygl_point(0,1), tinygl_point(0,2), tinygl_point(0,4)
        };
*/


/*
    task_t tasks[] =
            {
                {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE},
                {.func = display_points, .period = TASK_RATE / SOLID_LED_RATE, .data = solidPointsAtk},
                //{.func = wait_flash, .period = TASK_RATE / WAIT_TIME},
                {.func = display_points, .period = TASK_RATE / FLASHING_LED_RATE, .data = flashingPointsDef},
                //{.func = hidePoints, .period = TASK_RATE / FLASHING_LED_RATE, .data = flashingPointsDef},
                //{.func = tinygl_clear, .period = TASK_RATE / DISPLAY_TASK_RATE},
                //{.func = display_points, .period = TASK_RATE / SOLID_LED_RATE, .data = solidPointsAtk},
            };
*/


    system_init ();
    pacer_init (250);
    tinygl_init (DISPLAY_TASK_RATE);
    timer_init();
    navswitch_init();

    //task_schedule (tasks, ARRAY_SIZE (tasks));

    display_points(solidPointsAtk, numSolidAtk);



    while (1)
        {
            pacer_wait ();
            tinygl_update ();

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
                    display_points(flashingPointsAtk, numFlashingAtk);
                    flashWait_on = 100;
                } else {
                    flashWait_off--;
                }
            }

            if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
                //addPoint(tinygl_point(0, 0), FLASHING, ATK);
                tinygl_draw_point(tinygl_point(0, 0), 1);
            }

            //Attempt at getting ghosting working


       }
}
