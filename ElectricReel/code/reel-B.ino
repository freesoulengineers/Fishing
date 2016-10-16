#include <SPI.h>
// MCP4921 12-Bit DAC with SPI™ Interface
// http://ww1.microchip.com/downloads/en/DeviceDoc/21897a.pdf

const int slaveSelectPin = 10; // set pin 10 as the slave select
const int pickupSwitch = 9;
const int a2Switch =  A2;
const int a3Switch = A3;
const int a4Switch = A4;
int buttonPressed = false;

int i = 819;


// 시마노 릴 콘트롤 용 코드
// 은성 릴과 달리 버튼을 콘트롤 하기위해선
// High -> Low가 아닌 Low -> High 전압을 걸어줘야 해서
// 회로에서 p-ch Mosfet을 사용하였고
// 감기 동작을 가변저항으로 구현 해놓은 시마노 릴을 동작시키기 위해
// 공대형 회로에서는 12bit DAC와 기본 초기 전압을 잡아줄 수 있는 voltage divder (7.5k + 2.5k) 를 사용
// 이때 DAC MCP4921의 source / sink 전류에 주의해서 저항값을 정해야만 했다.
// 은성 릴보다 훨씬 어려웠다..


void setup() {
  pinMode (slaveSelectPin, OUTPUT);
  pinMode(pickupSwitch, OUTPUT);
  digitalWrite(pickupSwitch, HIGH);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  DAC_Write(819);
}


void loop() {

 // 공격적인 물고기 패턴 
 if (digitalRead(a2Switch) == HIGH) {
  // 12bits dac 0~4095
  longWinding();
  delay(500);
  shortWinding();
  delay(200);
  shortWinding();
  delay(200);


  
 } else {
  //DAC_Write(819);  
 }

 // 예민한 물고기용 패턴
 if(digitalRead(a3Switch) == HIGH)
 {
  for (int i = 819; i < 2400; i++)
  {
    DAC_Write(i);
    //delay(10);
  }
  delay(100);
  }

  else
  {
    DAC_Write(819);
    delay(100);
  }
  

 // 고기가 물렸을 때
 if(digitalRead(a4Switch) == HIGH)
  {
digitalWrite(pickupSwitch, LOW);
digitalWrite(pickupSwitch, LOW);
  delay(100);
  }

  else
  {

    digitalWrite(pickupSwitch, HIGH);
    digitalWrite(pickupSwitch, HIGH);

    delay(10);
  }


}


// 819 is default

void shortWinding()
{
  digitalWrite(pickupSwitch, LOW);
  delay(500);  
  digitalWrite(pickupSwitch, HIGH);
  delay(100);
}

void longWinding()
{
  digitalWrite(pickupSwitch, LOW);
  delay(1000);  
  digitalWrite(pickupSwitch, HIGH);
  delay(100);  
}

void DAC_Shutdown(){
byte data = 0;
digitalWrite(slaveSelectPin, LOW);
data = 0b00100000; // Config bits: Gain 1x, Shutdown
SPI.transfer(data);
SPI.transfer(data);
digitalWrite(slaveSelectPin, HIGH);
}

void DAC_Write(int outputValue){
byte data = 0;

digitalWrite(slaveSelectPin, LOW);
data = highByte(outputValue);
data = 0b00001111 & data; // First 4 data bits
data = 0b00110000 | data; // Config bits: Gain 1x, Enable
SPI.transfer(data); // 4 Config bits and First 4 data bits.
data = lowByte(outputValue);
SPI.transfer(data); // Last 8 data bits
digitalWrite(slaveSelectPin, HIGH);
}


