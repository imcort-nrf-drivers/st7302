#ifndef __ST7302_H__
#define __ST7302_H__

#include "transfer_handler.h"

#define SCREEN_WIDTH    250
#define SCREEN_HEIGHT   122

//SPI Mode 3 CPOL=1, CPHA=1
//122*250
//NRF_DRV_SPI_FREQ_2M

void st7302_addrSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void st7302_begin(uint8_t width, uint8_t height);
void st7302_writeData(uint8_t* buff, size_t buff_size);
void st7302_writeData8(uint8_t data);

void st7302_clear(void);
void st7302_inversion_on(void);
void st7302_setMemory(int x_start_byte, int y, int width, int height, uint8_t * data, int len);
void st7302_flushBuffer(void);
void st7302_setPixel(int x, int y, unsigned int val);
#endif
