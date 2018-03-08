/*
#define MAX 0x0400
#define N (MAX / 2)
#define step (MAX/N)
#define TERMISTOR_RREF 10000.f
#define TERMISTOR_R0 9200.f
#define TERMISTOR_B 3455.0f
#define TERMISTOR_T0 25.0f

static void printThermistorTable() {
  double r, st;
  double sequence[N] = {0.0};

  int i, j;
  for (i = 0, j = 1; i < N; ++i, j+=step) {
    r = TERMISTOR_RREF / ((double)MAX / j - 1);
    st = r / TERMISTOR_R0;
    st = log(st);
    st /= TERMISTOR_B;
    st += 1.0f / (TERMISTOR_T0 + 273.15f);
    st = 1.0f / st;
    st -= 273.15f;
    sequence[i] = st;
  }

  printf("const float adc_temperature[] PROGMEM= {\r\n");
  for (i = 0; i < N; ++i) {
    if (i%8 == 0) printf("\n");
    printf("%d, ", (int32_t)(sequence[i]*100));
  }

  printf("\n}");
  printf("%d", INT16_MAX);
}*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>

#include "lcd16x2.h"
#include "commons.h"

#define FLASH_PAGE_SIZE_W 16
#define FLASH_PAGE_SIZE_B FLASH_PAGE_SIZE_W*2

static const char CurrentTempStr[] __attribute__((__progmem__, section(".destt"), aligned(FLASH_PAGE_SIZE_B)))  = "Curr. t: ";
static const char DestTempStr[] __attribute__((__progmem__, section(".destt"), aligned(FLASH_PAGE_SIZE_B))) = "Dest. t: ";
//static uint16_t table[32] __attribute__((__progmem__, section(".destt"), aligned(FLASH_PAGE_SIZE_B)));

static char pageBuff[] = "Temp. d:\0\0";

int main() {  
  uint16_t i ;
  lcd16x2_init();  
  lcd16x2_printStrPm(CurrentTempStr, LCD16X2_LINE0_ADDR, 0);
  lcd16x2_printStrPm(DestTempStr, LCD16X2_LINE1_ADDR, 0);  

  cli();
  // Perform page erase
  boot_page_erase(DestTempStr);
  // Wait until the memory is erased
  boot_spm_busy_wait();

  for (i = 0; i <= 8; i+=2) {
    boot_page_fill(&DestTempStr[i], *((uint16_t*)&pageBuff[i]));
  }

  boot_page_write(DestTempStr);
  boot_spm_busy_wait();

  lcd16x2_printStrPm(DestTempStr, LCD16X2_LINE1_ADDR, 0);
  while (1) {
  }

  return 0;
}
