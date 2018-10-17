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

void drawShot(Shot shot, int toggle)
{
    tinygl_draw_point(tinygl_point(shot.col, shot.row), toggle);
}

int checkShotCol(int col) {
    if (col < 0) {return 0;}
    else if (col > 4) {return 0;}
    else {return 1;}
}

int checkShotRow(int row) {
    if (row < 0) {return 0;}
    else if (row > 6) {return 0;}
    else {return 1;}
}


void moveShotNorth(Shot *shot) {

    drawShot(*shot, 0);

    if (checkShotRow(shot->row - 1)) {
        shot->row--;
    }

    drawShot(*shot, 1);

}

void moveShotSouth(Shot *shot) {

    drawShot(*shot, 0);

    if (checkShotRow(shot->row + 1)) {
        shot->row++;
    }

    drawShot(*shot, 1);

}

void moveShotEast(Shot *shot) {

    drawShot(*shot, 0);

    if (checkShotCol(shot->col + 1)) {
        shot->col++;
    }

    drawShot(*shot, 1);

}

void moveShotWest(Shot *shot) {

    drawShot(*shot, 0);

    if (checkShotCol(shot->col - 1)) {
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
// }


// int main(void)
// {
//     system_init ();
//     pacer_init (1000);
//     tinygl_init (1000);
//     navswitch_init ();
//     button_init();
//     ir_uart_init();
//
//
//     tinygl_font_set (&font5x7_1);
//
//     Shot shot = createShot(2, 3);
//
//     int sendMode = 0;
//     int receiveMode = 0;
//
//     while (1) {
//         pacer_wait ();
//         button_update ();
//
//         if (sendMode == 0 && receiveMode == 0) {
//             if (ir_uart_read_ready_p()) {
//                 char slave = ir_uart_getc();
//                 receiveMode = 1;
//                 sendMode = 0;
//                 break;
//             }
//
//             if (button_push_event_p (0)) {
//                 sendMode = 1;
//                 receiveMode = 0;
//                 drawShot(shot, 1);
//                 ir_uart_putc('s');
//                 break;
//             }
//         }
//     }
//
//     while(1) {
//
//         pacer_wait ();
//         tinygl_update ();
//         button_update ();
//         navswitch_update ();
//
//
//
//         tinygl_update ();
//         if (sendMode) {
//
//             if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
//                 moveShotNorth(&shot);
//             }
//             if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
//                 moveShotSouth(&shot);
//             }
//             if (navswitch_push_event_p (NAVSWITCH_EAST)) {
//                 moveShotEast(&shot);
//             }
//             if (navswitch_push_event_p (NAVSWITCH_WEST)) {
//                 moveShotWest(&shot);
//             }
//
//             if (button_push_event_p (0)) {
//                 ir_uart_putc(sendShot(&shot));
//                 drawShot(shot, 0);
//                 while(1) {
//                     if (ir_uart_read_ready_p()) {
//                         tinygl_point_t tinyglShot = tinygl_point(shot.col, shot.row);
//                         char hit_miss = ir_uart_getc();
//                         if (hit_miss == 'H') {
//                             //tinygl_text("H");
//                             addPoint(tinyglShot, SOLID, DEF);
//                             displayPoints(solidPointsDef, numSolidDef);
//                         } else if (hit_miss == 'M') {
//                             //tinygl_text("M");
//                             addPoint(tinyglShot, FLASHING, DEF);
//                             displayPoints(numFlashingDef, numFlashingDef);
//                         }
//                         sendMode = 0;
//                         receiveMode = 1;
//                         break;
//                     }
//                 }
//
//
//             }
//         }
//         tinygl_update ();
//         if (receiveMode) {
//             addPoint(tinygl_point(3, 3) , SOLID, DEF);
//             //displayPoints(solidPointsDef, numSolidDef);
//             if (ir_uart_read_ready_p()) {
//                 int coord = ir_uart_getc();
//                 shot.col = coord & 0x0F;
//                 shot.row = (coord & 0xF0) >> 4;
//
//                 //drawShot(shot, 1);
//
//                 tinygl_point_t tinyglShot = tinygl_point(shot.col, shot.row);
//                 if (in(tinyglShot, SOLID, DEF)) {
//                     ir_uart_putc('H');
//                     sendMode = 1;
//                     receiveMode = 0;
//                 } else {
//                     ir_uart_putc('M');
//                     sendMode = 1;
//                     receiveMode = 0;
//                 }
//             }
//         }
//     }
//     displayPoints(numFlashingDef, numFlashingDef);
//     displayPoints(numSolidDef, numSolidDef);
// }
