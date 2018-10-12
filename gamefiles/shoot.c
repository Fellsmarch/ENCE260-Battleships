#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "stdio.h"
#include "navswitch.h"
#include "button.h"
#include "ir_uart.h"

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

    Shot shot = createShot(2, 3);
    drawShot(shot, 1);

    while(1) {

        pacer_wait ();
        tinygl_update ();
        button_update ();
        navswitch_update ();

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
        }
    }
}
