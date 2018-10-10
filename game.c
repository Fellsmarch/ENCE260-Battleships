#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "stdio.h"

#define DISPLAY_TASK_RATE 250

int main (void)
{
    system_init ();
    pacer_init (50);
    tinygl_init (5);

    //tinygl_draw_line (tinygl_point (1, 2), tinygl_point (3, 5), 0);
    //tinygl_draw_point(tinygl_point(4, 6), 1);
    tinygl_point_t test[] = {
        tinygl_point(4,6), tinygl_point(0,0), tinygl_point(4,2), tinygl_point(1,6),
        tinygl_point(1,5), tinygl_point(3,0), tinygl_point(0,3), tinygl_point(4,3)
        };
    int i = 0;
    int j = 1;

    while (1)
        {
            pacer_wait ();
            for (; i < test.length; i++) {
                tinygl_draw_point(test[i], j);
            }
            tinygl_update ();
            if (j == 0) {j = 1;}
            else {j = 0;}
        }
}
