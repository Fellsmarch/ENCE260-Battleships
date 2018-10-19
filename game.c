/** @file   game.c
    @author Harrison Cook, hgc25
    @author Ryan Chen, rch141
    @date   16 October 2018
    @brief  Run and play battleship game
*/

#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include "../fonts/font3x5_1.h"
#include "navswitch.h"
#include "lights.h"
#include "setupGame.h"
#include "shoot.h"

#define DISPLAY_TASK_RATE 250
#define PACER_RATE 500

int main (void)
{
    //Initialisation, including tinygl text options
    system_init ();
    pacer_init (PACER_RATE);
    tinygl_init (DISPLAY_TASK_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_font_set (&font3x5_1);
    timer_init();
    navswitch_init();
    button_init();
    ir_uart_init();

    //A boolean (ish) to tell the program that the solid lights have been
    //updated and hence re-displayed
    int solidUpdated = 1;
    //The current screen the user is on/seeing (DEF or ATK)
    int currentScreen;

    //Display the title screen
    displayText("  BATTLESHIP"); //Two spaces so it appears to scroll from the right
    tinygl_clear();

    //Get the users to place the ships
    placeShips();

    //Work out which player will be the master and start attacking first and
    //which will be the slave and start defending first
    currentScreen = masterSlave();

    //Create the shot object to be used
    Shot shot = createShot(2, 3);
    int waitingOnReply = 0; //If a shot has been sent and we are waiting on hit result


    while (1)
        {
            pacer_wait();
            navswitch_update();
            button_update();
            tinygl_update();
            flashLights(currentScreen);

            if (solidUpdated) {
                if (currentScreen == ATK) {
                    tinygl_clear();
                    displayPoints(solidPointsAtk, numSolidAtk);
                } else if (currentScreen == DEF) {
                    tinygl_clear();
                    displayPoints(solidPointsDef, numSolidDef);
                }
                solidUpdated = 0;
            }

            if (currentScreen == ATK) {
                drawShot(shot, 1); //Ensure the shot is shown on the screen
                moveShot(&shot); //Moves the shot (if the user moves it)
                displayPoints(solidPointsAtk, numSolidAtk); //Ensures the correct points are being displayed

                //User confirms shot placement and we check that shot hasn't been sent already
                if (button_push_event_p(0) && !waitingOnReply) {
                    ir_uart_putc(formatShot(&shot));
                    drawShot(shot, 0);
                    waitingOnReply = 1;
                }

                //If we have sent the shot already
                if (waitingOnReply) {
                    if (ir_uart_read_ready_p()) { //If something has been received
                        currentScreen = processResponse(shot);
                        solidUpdated = 1;
                        if (checkWin()) {displayResult(WIN);}
                        waitingOnReply = 0;
                    }
                }
            }
            tinygl_update(); //Updates here to ensure no drawing errors

            if (currentScreen == DEF) {
               if (ir_uart_read_ready_p()) {
                   processShot();
                   currentScreen = ATK;
                   shot.col = 2; shot.row = 3; //Reset shot position before switching to ATK screen
                   solidUpdated = 1;
               }
           }
       }
       return EXIT_SUCCESS;
}
