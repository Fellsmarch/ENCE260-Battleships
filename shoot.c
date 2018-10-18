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

int slave = 1;

typedef struct shot_s Shot;
struct shot_s {
    int col;
    int row;
};

Shot createShot(int col, int row) {
    Shot shot = {col, row};
    return shot;
}

/* Draws the shot or erases the shot */
void drawShot(Shot shot, int toggle)
{
    tinygl_draw_point(tinygl_point(shot.col, shot.row), toggle);
}

/* Checks the col boundaries of the screen before 
moving the shot */
int checkShotCol(int col) {
    if (col < 0) {return 0;}
    else if (col > 4) {return 0;}
    else {return 1;}
}

/* Checks the row boundaries of the screen before 
moving the shot */
int checkShotRow(int row) {
    if (row < 0) {return 0;}
    else if (row > 6) {return 0;}
    else {return 1;}
}

/* Moves the shot north */
void moveShotNorth(Shot *shot) {

    drawShot(*shot, 0);

    if (checkShotRow(shot->row - 1)) {
        shot->row--;
    }

    drawShot(*shot, 1);

}

/* Moves the shot south */
void moveShotSouth(Shot *shot) {

    drawShot(*shot, 0);

    if (checkShotRow(shot->row + 1)) {
        shot->row++;
    }

    drawShot(*shot, 1);

}

/* Moves the shot east */
void moveShotEast(Shot *shot) {

    drawShot(*shot, 0);

    if (checkShotCol(shot->col + 1)) {
        shot->col++;
    }

    drawShot(*shot, 1);

}

/* Moves the shot west */
void moveShotWest(Shot *shot) {

    drawShot(*shot, 0);

    if (checkShotCol(shot->col - 1)) {
        shot->col--;
    }

    drawShot(*shot, 1);

}

/* Puts the col and row of shot into 8 bits to send to opponent */
int sendShot(Shot *shot) {
    int toSend = 0;
    toSend |= shot->col;
    toSend |= shot->row << 4;
    return toSend;
}

/* Determines which player is player 1 and 2 */
void masterSlave (void)
{
    if (!ir_uart_read_ready_p()) {
        slave = 0;
        ir_uart_putc('_');

        tinygl_text("  WAIT");
        while(!ir_uart_read_ready_p()) {
            pacer_wait();
            tinygl_update();
        }
        ir_uart_getc(); //We don't care about the character itself

        tinygl_text("   ATTACK");
        int wait = 1515;
        while (wait > 0) {
            pacer_wait();
            tinygl_update();
            wait--;
        }
    } else {
        while(!ir_uart_read_ready_p()) {
                continue;
            }
            ir_uart_getc(); //We don't care about its return
            ir_uart_putc('_');
            tinygl_text("  DEFENCE");
            int wait = 1515;
            while (wait > 0) {
                pacer_wait();
                tinygl_update();
                wait--;
            }
    }
}