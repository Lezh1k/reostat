#ifndef LCD16X2_H
#define LCD16X2_H

#include <stdint.h>


#define LCD16X2_LINE0_ADDR 0x00
#define LCD16X2_LINE1_ADDR 0x40

void lcd16x2_init(void);
void lcd16x2_printStr(const char *str, uint8_t line, uint8_t pos);
void lcd16x2_printStrPm(const char *str, uint8_t line, uint8_t pos);

#endif // LCD16X2_H
