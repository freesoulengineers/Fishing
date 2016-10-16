int stopFlag = false;

// 은성 릴 콘트롤용 코드
// 핀 출력을 주고 적어도 100ms 쉬어주어야
// 은성 릴이 다음 동작을 위한 출력이 가능하다.


void setup() {
  pinMode (5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(A2, INPUT);
  pinMode(A4, INPUT);

  stopFlag = false;
}

void loop() {
 if (digitalRead(A2) == HIGH) {

 // dead fish pattern
  longWinding();
  shortWinding();
  shortWinding();
  delay(2000);
  shortWinding();
  shortWinding();
  shortWinding();
  delay(3000);
  
 }


 // ok or stop button
   if (digitalRead(A4) == HIGH && stopFlag == false) 
   {
      //stopFlag = true;
      digitalWrite(5, HIGH);
      delay(100);
      //digitalWrite(5, LOW);
      //delay(100);
      
   }

   if (digitalRead(A4) == LOW && stopFlag == false) 
   {
      stopFlag = true;
      //digitalWrite(5, HIGH);
      //delay(100);
      digitalWrite(5, LOW);
      delay(100);
      
   }
   
    if (digitalRead(A4) == HIGH && stopFlag == true) 
     {
      stopFlag = false;
      digitalWrite(6, HIGH);
      delay(2000);
      digitalWrite(6, LOW);
      delay(200);
     }
  
 
}

void shortWinding()
{
  digitalWrite(5, HIGH);
  delay(100);  
  digitalWrite(5, LOW);
  delay(10);  
  digitalWrite(6, HIGH);
  delay(500);  
  digitalWrite(6, LOW);
  delay(200); 

}

void longWinding()
{
  digitalWrite(5, HIGH);
  delay(800);  
  digitalWrite(5, LOW);
  delay(10);  
  digitalWrite(6, HIGH);
  delay(4000);  
  digitalWrite(6, LOW);
  delay(10); 
}


