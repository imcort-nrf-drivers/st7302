#include "st7302.h"

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

void st7302_begin(void)
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
    st7302_writeData8(0x00);  
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
    delay(120);
    
}

//void st7302_setMemory(int x_start_byte, int y, int width, int height, uint8_t * data, int len) {
//  // set buffer
//  int len_i = 0;
//  int width_byte_size = _width / 8;
//  int part_width_byte_size = int(width / 8);
//  int byte_start = y * width_byte_size + x_start_byte;
//  for (int i = 0; i != height; i++) {
//    for (int j = 0; j != part_width_byte_size; j++) {
//        if (len_i == len) {
//          return;
//        }
//        _buffer[byte_start + i * width_byte_size + j] = data[len_i];
//        len_i++;
//      }
//  }
//}
