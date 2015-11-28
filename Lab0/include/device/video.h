#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "common.h"

#define SCR_WIDTH  320
#define SCR_HEIGHT 200
#define SCR_SIZE ((SCR_WIDTH) * (SCR_HEIGHT))
#define VMEM_ADDR  ((uint8_t*)0xA0000)

extern uint8_t *vmem;

static inline void
draw_pixel(int x, int y, int color) {
/*
	//assert(x >= 0 && y >= 0 && x < SCR_WIDTH && y < SCR_HEIGHT);
	x += 32;
        //x = x + x/4;
        if (y >= 20 && y<60){
            y -= 20;
        }else if (y>=70 && y<230){
            y -= 30;
        }else y = -1;  

        if (y != -1){
            vmem[(y << 8) + (y << 6) + x] = color;
            //if (x % 5 == 0) vmem[(y << 8)+(y << 6) + x - 1] = (vmem[(y << 8)+(y << 6) + x] + vmem[(y << 8)+(y << 6) + x - 2])/2;
            //if (x % 5 == 3) vmem[(y << 8)+(y << 6) + x + 1] = (vmem[(y << 8)+(y << 6) + x] + vmem[(y << 8)+(y << 6) + x + 2])/2;
        }  
 */
                vmem[(y << 8) + (y << 6) + x] = color;
  
}

void prepare_buffer(void);
void display_buffer(void);

void draw_string(const char*, int, int, int);

#endif
