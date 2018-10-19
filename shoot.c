/** @file   shoot.c
    @author Ryan Chen, rch141
    @author Harrison Cook, hgc25
    @date   15 October 2018
    @brief  Functions to facilitate shooting of shots
*/

#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "stdio.h"
#include "navswitch.h"
#include "button.h"
#include "ir_uart.h"
#include "lights.h"
#include "../fonts/font5x7_1.h"
#include "setupGame.h"

/** The struct to hold the shot information (row and column) */
typedef struct shot_s Shot;
struct shot_s {
    int col;
    int row;
};

/** Creates and returns a Shot object
    @param col the column the shot sits in
    @param row the row the shot sits in
    @return a new Shot object
*/
Shot createShot(int col, int row) {
    Shot shot = {col, row};
    return shot;
}

/** Draws the shot as either visible or hidden
    @param shot the Shot to draw
    @param toggle whether or not the shot will be visible (1) or hidden (0)
    @return this never returns
*/
void drawShot(Shot shot, int toggle)
{
    tinygl_draw_point(tinygl_point(shot.col, shot.row), toggle);
}

/** Checks that the given column is within the LED matrix boundaries
    @param col the given column
    @return returns true (1) if the column is within the bounaries and false (0) otherwise
*/
int checkShotCol(int col) {
    if (col < 0) {return 0;} //False
    else if (col > 4) {return 0;} //False
    else {return 1;} //True
}

/** Checks that the given row is within the LED matrix boundaries
    @param row the given row
    @return returns true (1) if the row is within the bounaries and false (0) otherwise
*/
int checkShotRow(int row) {
    if (row < 0) {return 0;} //False
    else if (row > 6) {return 0;} //False
    else {return 1;} //True
}

/** Moves the shot north on the LED matrix
    @param shot the Shot to move
    @return the never returns
*/
void moveShotNorth(Shot *shot) {
    drawShot(*shot, 0); //Hide the shot before moving
    //Check if the new row is within the LED matrix boundaries and move shot if it is
    if (checkShotRow(shot->row - 1)) {shot->row--;}
    drawShot(*shot, 1); //Redraw the shot
}

/** Moves the shot south on the LED matrix
    @param shot the Shot to move
    @return the never returns
*/
void moveShotSouth(Shot *shot) {
    drawShot(*shot, 0);
    if (checkShotRow(shot->row + 1)) {shot->row++;}
    drawShot(*shot, 1);
}

/** Moves the shot east on the LED matrix
    @param shot the Shot to move
    @return the never returns
*/
void moveShotEast(Shot *shot) {
    drawShot(*shot, 0);
    if (checkShotCol(shot->col + 1)) {shot->col++;}
    drawShot(*shot, 1);
}

/** Moves the shot west on the LED matrix
    @param shot the Shot to move
    @return the never returns
*/
void moveShotWest(Shot *shot) {
    drawShot(*shot, 0);
    if (checkShotCol(shot->col - 1)) {shot->col--;}
    drawShot(*shot, 1);
}

/** Formats the shot into 8 bits for sending
    @param shot the Shot to move
    @return returns the formatted shot as an integer
*/
int formatShot(Shot *shot) {
    int toSend = 0;
    toSend |= shot->col;
    toSend |= shot->row << 4;
    return toSend;
}

/** Works out which user will be the slave (defender) and which will the the master (attacker)
    @return the screen that the user starts on (either DEF or ATK)
*/
int masterSlave(void)
{
    //If you are first to finish the ship placement
    if (!ir_uart_read_ready_p()) {
        ir_uart_putc('_'); //Send a blank character to indicate that the other user will be the slave
        tinygl_text("  WAIT");
        while(!ir_uart_read_ready_p()) { //Wait for the other user to catch up
            pacer_wait();
            tinygl_update();
        }
        ir_uart_getc(); //Wait for other user to respond

        //Display the attack title screen to the user
        tinygl_text("   ATTACK");
        int wait = 1515;
        while (wait > 0) {
            pacer_wait();
            tinygl_update();
            wait--;
        }
        return ATK;

    } else {
        while(!ir_uart_read_ready_p()) {
                continue;
            }
        ir_uart_getc(); //Wait for other user to send intial communication
        ir_uart_putc('_'); //Send a ready message
        //Display the defense title screen to the user
        tinygl_text("  DEFENCE");
        int wait = 1515;
        while (wait > 0) {
            pacer_wait();
            tinygl_update();
            wait--;
        }
        return DEF;
    }
}

/** Checks whether the player has won
    @return true (1) or false (0) depending if the player has won
*/
int checkWin(void)
{
    //Checks the current number of points in the solid attack array, if
    //it is greater than or equal to the number of points taken up by the
    //ships then the player has won
    return (numSolidAtk >= TOTALSHIPSIZE);
}

/** Checks whether the player has lost
    @return true (1) or false (0) depending if the player has lost
*/
int checkLoss(void)
{
    //Checks the current number of points in the flashing defense array,
    //if it is greater than or equal to the number of points taken up by
    //the ships then the player has lost
    return (numFlashingDef >= TOTALSHIPSIZE);
}

/** Checks whether the user is trying to move the shot and moves it if they are
    @param shot a pointer to the shot to move
    @return this never returns
*/
void moveShot(Shot* shot)
{
    if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
        moveShotNorth(shot);
    }
    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
        moveShotSouth(shot);
    }
    if (navswitch_push_event_p (NAVSWITCH_EAST)) {
        moveShotEast(shot);
    }
    if (navswitch_push_event_p (NAVSWITCH_WEST)) {
        moveShotWest(shot);
    }
}

/** Proccesses the response received after sending a shot
    @param shot the sent Shot
    @return the new screen that the user should be on (either DEF or ATK)
*/
int processResponse(Shot shot)
{
    tinygl_point_t shotToAdd = tinygl_point(shot.col, shot.row);
    char hitOrMiss = ir_uart_getc();
    if (hitOrMiss == 'H') {
        addPoint(shotToAdd, SOLID, ATK);
        displayText("  HIT");
        return DEF;
    } else if (hitOrMiss == 'M') {
        addPoint(shotToAdd, FLASHING, ATK);
        displayText("  MISS");
        return DEF;
    } else {return ATK;} //If some random other character was received
}

/** Proccesses the received shot
    @return this never returns
*/
void processShot(void)
{
    //Decode the received shot
    char coord = ir_uart_getc();
    int col = coord & 0x0F;
    int row = (coord & 0xF0) >> 4;

    tinygl_point_t shotToAdd = tinygl_point(col, row);
    if (in(shotToAdd, SOLID, DEF)) { //If the shot is in the array that hold the ships points
        ir_uart_putc('H'); //Send hit confirmation
        addPoint(shotToAdd, FLASHING, DEF);
        displayText("  HIT");
        if (checkLoss()) {displayResult(LOSS);}
    } else {
        ir_uart_putc('M'); //Send miss confirmation
        displayText("  MISS");
    }
}
