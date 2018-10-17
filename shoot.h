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

int slave;

typedef struct shot_s Shot;
struct shot_s {
    int col;
    int row;
};

Shot createShot(int col, int row);

void drawShot(Shot shot, int toggle);

int checkShotCol(int col);

int checkShotRow(int row);


void moveShotNorth(Shot *shot);

void moveShotSouth(Shot *shot);

void moveShotEast(Shot *shot);

void moveShotWest(Shot *shot);

int sendShot(Shot *shot);

void masterSlave (void);





    // while (1) {
    //     pacer_wait ();
    //     button_update ();
    //
    //     if (sendMode == 0 && receiveMode == 0) {
    //         if (ir_uart_read_ready_p()) {
    //             char slave = ir_uart_getc();
    //             receiveMode = 1;
    //             sendMode = 0;
    //             break;
    //         }
    //
    //         if (button_push_event_p (0)) {
    //             sendMode = 1;
    //             receiveMode = 0;
    //             drawShot(shot, 1);
    //             ir_uart_putc('s');
    //             break;
    //         }
    //     }
    // }
