#include "lcd16x2.h"
#include "commons.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

#define LINE_CHAR_COUNT 16

#define LCD_DATA_DDR DDRB
#define LCD_CONTROL_DDR DDRD

#define LCD_DATA_PORT PORTB
#define LCD_DATA_PIN  PINB
#define LCD_CONTROL_PORT PORTD

#define LCD_OUT_D4 (1 << PB0)
#define LCD_OUT_D5 (1 << PB1)
#define LCD_OUT_D6 (1 << PB2)
#define LCD_OUT_D7 (1 << PB3)

#define LCD_IN_D4 (1 << PINB0)
#define LCD_IN_D5 (1 << PINB1)
#define LCD_IN_D6 (1 << PINB2)
#define LCD_IN_D7 (1 << PINB3)

//register select
#define LCD_RS (1 << PD5)
//enabled
#define LCD_EN (1 << PD6)
//write=0, read=1
#define LCD_RW (1 << PD4)

static void send(uint8_t val);
static void sendCmd(uint8_t cmd);
static void sendData(uint8_t data);
static void setPos(uint8_t lineAddr, uint8_t pos);

static inline void prepareLcdOut() {
  LCD_DATA_PORT &= ~(LCD_OUT_D4 | LCD_OUT_D5 | LCD_OUT_D6 | LCD_OUT_D7);
}
//////////////////////////////////////////////////////////////////////////

static inline void setRead() {
  LCD_CONTROL_PORT |= LCD_RW;
}

static inline void setWrite() {
  LCD_CONTROL_PORT &= ~LCD_RW;
}
//////////////////////////////////////////////////////////////////////////

static inline void strobeHi() {
  LCD_CONTROL_PORT |= LCD_EN;
}

static inline void strobeLo() {
  LCD_CONTROL_PORT &= ~LCD_EN;
}
//////////////////////////////////////////////////////////////////////////

static inline void setCommand() {
  LCD_CONTROL_PORT &= ~LCD_RS;
}

static inline void setData() {
  LCD_CONTROL_PORT |= LCD_RS;
}
//////////////////////////////////////////////////////////////////////////

static void waitBF() {
  setRead();
  while (LCD_DATA_PIN & LCD_IN_D7)
    ;
}
//////////////////////////////////////////////////////////////////////////

static inline void strobe() {
  strobeHi();
  _delay_ms(1); //have no idea why we should wait so much time.
  strobeLo();
  _delay_ms(1);
}
//////////////////////////////////////////////////////////////////////////

void setPos(uint8_t lineAddr, uint8_t pos) {
  sendCmd(0x80 | (lineAddr+pos));
  _delay_us(40);
}
//////////////////////////////////////////////////////////////////////////

void send(uint8_t val) {  
  prepareLcdOut();
  LCD_DATA_PORT |= ((val >> 4) & 0x0f);
  strobe();
  prepareLcdOut();
  LCD_DATA_PORT |= (val & 0x0f);
  strobe();
}
//////////////////////////////////////////////////////////////////////////

void sendCmd(uint8_t cmd) {
  setWrite();
  setCommand();
  send(cmd);
}
//////////////////////////////////////////////////////////////////////////

void sendData(uint8_t data) {
  setWrite();
  setData();
  send(data);    
}
//////////////////////////////////////////////////////////////////////////

void lcd16x2_init() {  
  static char init[]= {
    0x80, //turn off
    0x03, //init0
    0x03, //init1
    0x02, //init2
    0x28, //set 2 lines
    0x01, //clear
    0x0C, //set shift right
    0x00  //end
  };
  char *lp = init;

  LCD_CONTROL_DDR |= (LCD_RS | LCD_EN | LCD_RW);
  LCD_DATA_DDR |= (LCD_OUT_D4 | LCD_OUT_D5 | LCD_OUT_D6 | LCD_OUT_D7);
  LCD_CONTROL_PORT &= ~(LCD_EN | LCD_RS);  
  prepareLcdOut();
  setWrite();
  for (; *lp; ++lp)
    sendCmd(*lp);
}
//////////////////////////////////////////////////////////////////////////

void lcd16x2_printStr(const char *str, uint8_t line, uint8_t pos) {
  setPos(line, pos);
  for (; *str; ++str)
    sendData(*str);
}
//////////////////////////////////////////////////////////////////////////

void lcd16x2_printStrPm(const char *str, uint8_t line, uint8_t pos) {
  char c;
  setPos(line, pos);
  for (; (c = pgm_read_byte(str)); ++str)
    sendData(c);
}
//////////////////////////////////////////////////////////////////////////
