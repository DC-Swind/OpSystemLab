#include "common.h"
#include "x86.h"

#define VGA_DAC_READ_INDEX 0x3c7
#define VGA_DAC_WRITE_INDEX 0x3c8
#define VGA_DAC_DATA 0x3c9

void reset_palette()
{
  uint8_t r,g,b;
  out_byte(VGA_DAC_WRITE_INDEX, 0);
  for (r = 0x00; r<= 0x07; r++)
    for (g = 0x00; g<=0x07; g++)
      for (b = 0x00; b<=0x03; b++)
        {
          out_byte(VGA_DAC_DATA, r<<3);
          out_byte(VGA_DAC_DATA, g<<3);
	  out_byte(VGA_DAC_DATA, b<<4);

        }

}

