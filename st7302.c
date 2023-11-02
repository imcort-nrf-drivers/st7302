#include "st7302.h"
#include <math.h>

#define BYTE_BIT_SIZE 8.0

#define BUFFER_SIZE 4224
static uint8_t _buffer[BUFFER_SIZE];
static uint8_t _sendbuffer[BUFFER_SIZE];
static int _width, _height;
static int _buffer_size;

static void st7302_writeCommand(uint8_t cmd) {

    digitalWrite(ST7302_DC, 0);
    digitalWrite(ST7302_CS, 0);
    spi_transfer(&cmd, 1, NULL, 0);
    digitalWrite(ST7302_CS, 1);
    digitalWrite(ST7302_DC, 1);
	
}

void st7302_writeData8(uint8_t data) {

		digitalWrite(ST7302_CS, 0);
		spi_transfer(&data, 1, NULL, 0);
		digitalWrite(ST7302_CS, 1);
	
}

void st7302_writeData(uint8_t* buff, size_t buff_size) {

		digitalWrite(ST7302_CS, 0);
		
		spi_send(buff, buff_size);
		
		digitalWrite(ST7302_CS, 1);

}

/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/

void st7302_addrSet(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
		st7302_writeCommand(0x2a); //列地址设置
		st7302_writeData8(x1+0x19);
		st7302_writeData8(x2+0x19);
		st7302_writeCommand(0x2b); //行地址设置
		st7302_writeData8(y1);
		st7302_writeData8(y2);
		st7302_writeCommand(0x2c); //储存器写
}

void st7302_begin(uint8_t width, uint8_t height)
{
    
    pinMode(ST7302_DC, OUTPUT);
    pinMode(ST7302_CS, OUTPUT);

    spi_init();

    st7302_writeCommand(0x01);
    delay(120); 
    
    st7302_writeCommand(0x38);//High power mode                                              
    st7302_writeCommand(0xEB);//Enable OTP    
    st7302_writeData8(0x02); 
    st7302_writeCommand(0xD7);//OTP Load Control    
    st7302_writeData8(0x68);       
    st7302_writeCommand(0xD1);//Auto Power Control       
    st7302_writeData8(0x01);      
    st7302_writeCommand(0xC0);//Gate Voltage Setting VGH=12V ; VGL=-5V       
    st7302_writeData8(0x80);       
    st7302_writeCommand(0xC1);//VSH Setting        
    st7302_writeData8(0x28);//       
    st7302_writeData8(0x28);            
    st7302_writeData8(0x28);       
    st7302_writeData8(0x28);      
    st7302_writeData8(0x14);      
    st7302_writeData8(0x00);     
    st7302_writeCommand(0xC2);//VSL Setting VSL=0     
    st7302_writeData8(0x00);      
    st7302_writeData8(0x00);           
    st7302_writeData8(0x00);       
    st7302_writeData8(0x00);    
    st7302_writeCommand(0xCB);//VCOMH Setting       
    st7302_writeData8(0x14);//14  0C   7    
    st7302_writeCommand(0xB4);//Gate EQ Setting HPM EQ LPM EQ      
    st7302_writeData8(0xE5);      
    st7302_writeData8(0x77);           
    st7302_writeData8(0xF1);      
    st7302_writeData8(0xFF);       
    st7302_writeData8(0xFF);       
    st7302_writeData8(0x4F);      
    st7302_writeData8(0xF1);       
    st7302_writeData8(0xFF);       
    st7302_writeData8(0xFF);      
    st7302_writeData8(0x4F);     
    st7302_writeCommand(0x11);//Sleep out      
    delay(100);  // delay_ms 100ms      
    st7302_writeCommand(0xC7);//OSC Setting       
    st7302_writeData8(0xA6);       
    st7302_writeData8(0xE9);    
    st7302_writeCommand(0xB0);   //Duty Setting       
    st7302_writeData8(0x64);  //250duty/4=63    
    st7302_writeCommand(0x36);//Memory Data Access Control       
    st7302_writeData8(0x20);  
    st7302_writeCommand(0x3A);//Data Format Select 4 write for 24 bit       
    st7302_writeData8(0x11);     
    st7302_writeCommand(0xB9);//Source Setting       
    st7302_writeData8(0x23);      
    st7302_writeCommand(0xB8);//Panel Setting Frame inversion       
    st7302_writeData8(0x09);      
    st7302_writeCommand(0x2A);////Column Address Setting S61~S182       
    st7302_writeData8(0x05);       
    st7302_writeData8(0x36);         
    st7302_writeCommand(0x2B);////Row Address Setting G1~G250       
    st7302_writeData8(0x00);       
    st7302_writeData8(0xC7);        
    st7302_writeCommand(0xD0);       
    st7302_writeData8(0x1F); 
    st7302_writeCommand(0x29);//Display on  
    st7302_writeCommand(0xB9);//enable CLR RAM       
    st7302_writeData8(0xE3);      
    delay(100);       
    st7302_writeCommand(0xB9);//enable CLR RAM     
    st7302_writeData8(0x23);  
    st7302_writeCommand(0x72);     
    st7302_writeData8(0x00);         //Destress OFF
    st7302_writeCommand(0x39);//LPM
    st7302_writeCommand(0x2A);   //Column Address Setting                          
    st7302_writeCommand(0x19);                                                   
    st7302_writeData8(0x23);  //35                                               
    st7302_writeCommand(0x2B);   //Row Address Setting                             
    st7302_writeData8(0); 
    st7302_writeData8(0x7C);  
    st7302_writeData8(0x2C);   //write image data
    delay(100);
    
    _width = ceil(width / BYTE_BIT_SIZE) * BYTE_BIT_SIZE;
    _height = ceil(height / 12.0) * 12;
    _buffer_size = ceil(_height * _width / BYTE_BIT_SIZE);
    
    Debug("%d,%d,%d",_width,_height,_buffer_size);
    
    st7302_clear();
    
}

void st7302_clear(void)
{
    st7302_writeCommand(0xB9);
    st7302_writeData8(0xE3);
    delay(200);
    st7302_writeCommand(0xB9);
    st7302_writeData8(0x23);
    
    memset(_buffer, 0, BUFFER_SIZE);
}

void st7302_inversion_on(void)
{
    st7302_writeCommand(0x21);
}

void st7302_setMemory(int x_start_byte, int y, int width, int height, uint8_t * data, int len) {
  // set buffer
  int len_i = 0;
  int width_byte_size = _width / BYTE_BIT_SIZE;
  int part_width_byte_size = width / BYTE_BIT_SIZE;
  int byte_start = y * width_byte_size + x_start_byte;
  for (int i = 0; i != height; i++) {
    for (int j = 0; j != part_width_byte_size; j++) {
        if (len_i == len) {
          return;
        }
        _buffer[byte_start + i * width_byte_size + j] = data[len_i];
        len_i++;
      }
  }
}

static int sendcount = 0;

static void _send_12_row_bit(int line_i, uint8_t byte0, uint8_t byte1, uint8_t byte2,
                                 uint8_t byte3, uint8_t byte4, uint8_t byte5,
                                 uint8_t byte6, uint8_t byte7, uint8_t byte8,
                                 uint8_t byte9, uint8_t byte10, uint8_t byte11) {
  uint8_t data = (byte0 << line_i >> 0 & 0xC0)
                 | (byte1 << line_i >> 2 & 0x30)
                 | (byte2 << line_i >> 4 & 0x0C)
                 | (byte3 << line_i >> 6 & 0x03);
  //spi_send(&data, 1);
    _sendbuffer[sendcount++] = data;
  data = (byte4 << line_i>> 0 & 0xC0)
         | (byte5 << line_i>> 2 & 0x30)
         | (byte6 << line_i>> 4 & 0x0C)
         | (byte7 << line_i >> 6 & 0x03);
  //spi_send(&data, 1);
    _sendbuffer[sendcount++] = data;
  data = (byte8 << line_i>> 0 & 0xC0)
         | (byte9 << line_i >> 2 & 0x30)
         | (byte10 << line_i>> 4 & 0x0C)
         | (byte11 << line_i >> 6 & 0x03);
  //spi_send(&data, 1);
    _sendbuffer[sendcount++] = data;
}

static void _send_12_row_byte(uint8_t byte0, uint8_t byte1, uint8_t byte2,
                                  uint8_t byte3, uint8_t byte4, uint8_t byte5,
                                  uint8_t byte6, uint8_t byte7, uint8_t byte8,
                                  uint8_t byte9, uint8_t byte10, uint8_t byte11) {
  _send_12_row_bit(0, byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11);
  _send_12_row_bit(2, byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11);
  _send_12_row_bit(4, byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11);
  _send_12_row_bit(6, byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9, byte10, byte11);
}

static void _send_12_row(int _12_row) {
  int _12_row_start = _width / BYTE_BIT_SIZE;
  _12_row_start = _12_row_start * _12_row * 12;
  int width_byte_size = _width / BYTE_BIT_SIZE;
  for (int i = 0; i != width_byte_size; i++) {
    _send_12_row_byte(_buffer[_12_row_start + width_byte_size * 0 + i],
                      _buffer[_12_row_start + width_byte_size * 1 + i],
                      _buffer[_12_row_start + width_byte_size * 2 + i],
                      _buffer[_12_row_start + width_byte_size * 3 + i],
                      _buffer[_12_row_start + width_byte_size * 4 + i],
                      _buffer[_12_row_start + width_byte_size * 5 + i],
                      _buffer[_12_row_start + width_byte_size * 6 + i],
                      _buffer[_12_row_start + width_byte_size * 7 + i],
                      _buffer[_12_row_start + width_byte_size * 8 + i],
                      _buffer[_12_row_start + width_byte_size * 9 + i],
                      _buffer[_12_row_start + width_byte_size * 10 + i],
                      _buffer[_12_row_start + width_byte_size * 11 + i]);
  }
}

void st7302_flushBuffer(void) {
  // address set
  st7302_writeCommand(0x2a);
  st7302_writeData8(0x19);
  st7302_writeData8(0x19 + _height / 12);
  st7302_writeCommand(0x2b);
  st7302_writeData8(0x00);
  st7302_writeData8(_width / 2 - 1);
  st7302_writeCommand(0x2c);
    
    
  digitalWrite(ST7302_CS, 0);
  digitalWrite(ST7302_DC, 1);
    sendcount = 0;
    memset(_sendbuffer, 0, BUFFER_SIZE);
  // data send
  for (int i = 0; i != _height / 12; i++) {
    _send_12_row(i);
  }
  spi_send(_sendbuffer, sendcount-1);
  Debug("count %d", sendcount);
  // pin reset
  digitalWrite(ST7302_CS, 1);
}

static const uint8_t set[] = {1, 2, 4, 8, 16, 32, 64, 128},
                     clr[] = {(uint8_t)~1,  (uint8_t)~2,  (uint8_t)~4,
                              (uint8_t)~8,  (uint8_t)~16, (uint8_t)~32,
                              (uint8_t)~64, (uint8_t)~128};

//256*132
                              
void st7302_setPixel(int x, int y, unsigned int val)
{
    if(val){

        _buffer[y * 32 + (x / 8)] |= set[7 - (x & 7)];

    } else {

        _buffer[y * 32 + (x / 8)] &= clr[7 - (x & 7)];

    }
}
