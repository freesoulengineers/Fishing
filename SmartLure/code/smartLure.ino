#include <avr/sleep.h>
#include <avr/wdt.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

volatile boolean f_wdt = 1;

#define RED 2
#define GREEN 1
#define BLUE 0

// sleep을 이용하여 최대한 low power 모드로 동작하는것이 중요.
// 나머지는 아주 간단한 코드.
// 회로에서 점퍼핀 연결을 바꿔 주면 BLUE 출력핀이 모터동작 핀이 된다.

void setup(){
  setup_watchdog(6); // approximately 1 second sleep
  pinMode(RED, OUTPUT);  
  pinMode(GREEN, OUTPUT); 
  pinMode(BLUE, OUTPUT);  
}

void loop(){
  if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt=0;   
    // logic here
    digitalWrite(RED, !digitalRead(RED));
    delay(500);
    digitalWrite(RED, !digitalRead(RED));
    delay(500);
    digitalWrite(GREEN, !digitalRead(GREEN));
    delay(500);
    digitalWrite(GREEN, !digitalRead(GREEN));
    delay(500);
    digitalWrite(BLUE, !digitalRead(BLUE));
    delay(500);
    digitalWrite(BLUE, !digitalRead(BLUE));
    delay(500);
    system_sleep();
  }
}

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {
  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;

  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCR = bb;
  WDTCR |= _BV(WDIE);
}

// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  f_wdt=1;  // set global flag
}

// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();                        // System sleeps here
  sleep_disable();                     // System continues execution here when watchdog timed out 
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
}
