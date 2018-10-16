#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "stdio.h"
#include "navswitch.h"
#include "button.h"
#include "ir_uart.h"
#include "../lights.h"

typedef struct shot_s Shot;
struct shot_s {
    int col;
    int row;
};

Shot createShot(int col, int row) {
    Shot shot = {col, row};
    return shot;
}

void drawShot(Shot shot, int toggle)
{
    tinygl_draw_point(tinygl_point(shot.col, shot.row), toggle);
}

static int checkCol(int col) {
    if (col < 0) {return 0;}
    else if (col > 4) {return 0;}
    else {return 1;}
}

static int checkRow(int row) {
    if (row < 0) {return 0;}
    else if (row > 6) {return 0;}
    else {return 1;}
}


void moveShotNorth(Shot *shot) {

    drawShot(*shot, 0);

    if (checkRow(shot->row - 1)) {
        shot->row--;
    }

    drawShot(*shot, 1);

}

void moveShotSouth(Shot *shot) {

    drawShot(*shot, 0);

    if (checkRow(shot->row + 1)) {
        shot->row++;
    }

    drawShot(*shot, 1);

}

void moveShotEast(Shot *shot) {

    drawShot(*shot, 0);

    if (checkCol(shot->col + 1)) {
        shot->col++;
    }

    drawShot(*shot, 1);

}

void moveShotWest(Shot *shot) {

    drawShot(*shot, 0);

    if (checkCol(shot->col - 1)) {
        shot->col--;
    }

    drawShot(*shot, 1);

}

int sendShot(Shot *shot) {
    int toSend = 0;
    toSend |= shot->col;
    toSend |= shot->row << 4;
    return toSend;
}


int main(void)
{
    system_init ();
    pacer_init (1000);
    tinygl_init (1000);
    navswitch_init ();
    button_init();
    ir_uart_init();

    Shot shot = createShot(2, 3);

    int sendMode = 1;
    int receiveMode = 0;

    while(1) {

        pacer_wait ();
        tinygl_update ();
        button_update ();
        navswitch_update ();

/*
        if (sendMode == 0 && receiveMode == 0) {
            if (ir_uart_read_ready_p()) {
                char slave = ir_uart_getc();
                receiveMode = 1;
                sendMode = 0;
            }

            if (button_push_event_p (0)) {
                sendMode = 1;
                receiveMode = 0;
                drawShot(shot, 1);
                ir_uart_putc('s');
            }
        }
*/
        tinygl_update ();
        if (sendMode) {

            if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
                moveShotNorth(&shot);
            }
            if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                moveShotSouth(&shot);
            }
            if (navswitch_push_event_p (NAVSWITCH_EAST)) {
                moveShotEast(&shot);
            }
            if (navswitch_push_event_p (NAVSWITCH_WEST)) {
                moveShotWest(&shot);
            }

            if (button_push_event_p (0)) {
                ir_uart_putc(sendShot(&shot));
                drawShot(shot, 0);
                if (ir_uart_read_ready_p()) {
                    tinygl_point_t tinyglShot = tinygl_point(shot.col, shot.row);
                    char hit_miss = ir_uart_getc();
                    if (hit_miss == 'H') {
                        addPoint(tinyglShot, 0, 0);
                    } else if (hit_miss == 'M') {
                        addPoint(tinyglShot, 1, 0);
                    }
                    sendMode = 0;
                    receiveMode = 1;
                }

            }
        }
        tinygl_update ();
        if (receiveMode) {
            addPoint(tinygl_point(3, 3) , 0, 0);
            displayPoints(solidPointsDef, numSolidDef);
            if (ir_uart_read_ready_p()) {
                int coord = ir_uart_getc();
                shot.col = coord & 0x0F;
                shot.row = (coord & 0xF0) >> 4;

                //drawShot(shot, 1);
                sendMode = 1;
                receiveMode = 0;
                tinygl_point_t tinyglShot = tinygl_point(shot.col, shot.row);
                if (!in(tinyglShot, 0, 1)) {
                    ir_uart_putc('H');
                } else {
                    ir_uart_putc('M');
                }
            }
        }
    }
}
