#include <stddef.h>
#include <stdint.h>
#include "../drivers/port.h"

unsigned char *screen = (unsigned char *) 0xb8000;
size_t positionx = 0;
size_t position = 0;

void cursorSetPosition(uint16_t pos){
	port_byte_out(0x3D4, 0x0F);
	port_byte_out(0x3D5, (uint8_t) (pos & 0xFF));
	port_byte_out(0x3D4, 0x0E);
	port_byte_out(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
void putchar(const char value){
    switch(value){
        case '\n':
            while(positionx < 80){
                position += 2;
                positionx++;
            }
            break;
        case '\r':
            while(positionx > 0){
                position -= 2;
                positionx--;
            }
            break;
        case '\b':
            position -= 2;
            positionx--;
            break;
        default:
            screen[position++] = value;
            screen[position++] = 0x07;
            break;
    }
    cursorSetPosition(position/2);
    positionx++;
    if(positionx >= 81)
        positionx = 0;
}
void print(const char *text){
    for(; *text; text++)
        putchar(*text);
}
void clear(){
    position = 0;
    positionx = 0;
    for(size_t loop = 0; loop < 80 * 25; loop++){
        putchar(0);
    }
    position = 0;
    positionx = 0;
    cursorSetPosition(position/2);
}