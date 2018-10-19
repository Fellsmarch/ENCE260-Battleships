/** @file   shoot.h
    @author Harrison Cook, hgc25
    @author Ryan Chen, rch141
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
Shot createShot(int col, int row);

/** Draws the shot as either visible or hidden
    @param shot the Shot to draw
    @param toggle whether or not the shot will be visible (1) or hidden (0)
    @return this never returns
*/
void drawShot(Shot shot, int toggle);

/** Checks that the given column is within the LED matrix boundaries
    @param col the given column
    @return returns true (1) if the column is within the bounaries and false (0) otherwise
*/
int checkShotCol(int col);

/** Checks that the given row is within the LED matrix boundaries
    @param row the given row
    @return returns true (1) if the row is within the bounaries and false (0) otherwise
*/
int checkShotRow(int row);

/** Moves the shot north on the LED matrix
    @param shot the Shot to move
    @return the never returns
*/
void moveShotNorth(Shot *shot);

/** Moves the shot south on the LED matrix
    @param shot the Shot to move
    @return the never returns
*/
void moveShotSouth(Shot *shot);

/** Moves the shot east on the LED matrix
    @param shot the Shot to move
    @return the never returns
*/
void moveShotEast(Shot *shot);

/** Moves the shot west on the LED matrix
    @param shot the Shot to move
    @return the never returns
*/
void moveShotWest(Shot *shot);

/** Formats the shot into 8 bits for sending
    @param shot the Shot to move
    @return returns the formatted shot as an integer
*/
int formatShot(Shot *shot);

/** Works out which user will be the slave (defender) and which will the the master (attacker)
    @return the screen that the user starts on (either DEF or ATK)
*/
int masterSlave (void);

/** Checks whether the player has won
    @return true (1) or false (0) depending if the player has won
*/
int checkWin(void);

/** Checks whether the player has lost
    @return true (1) or false (0) depending if the player has lost
*/
int checkLoss(void);

/** Checks whether the user is trying to move the shot and moves it if they are
    @param shot a pointer to the shot to move
    @return this never returns
*/
void moveShot (Shot* shot);

/** Proccesses the response received after sending a shot
    @param shot the sent Shot
    @return the new screen that the user should be on (either DEF or ATK)
*/
int processResponse(Shot shot);

/** Proccesses the received shot
    @return this never returns
*/
void processShot(void);
