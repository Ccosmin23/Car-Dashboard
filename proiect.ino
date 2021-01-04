#include <LiquidCrystal.h>
#include <Servo.h>


//senzor temperatura
int sensePin = A0;
int sensorInput;
volatile double temp;
float logR2, R2;

//senzor sonar
int trigger_pin = 8;
int echo_pin = 9;
int trigger_pin2 = 11;
int echo_pin2 = 12;

//buzzer
int buzzer_pin = 13;

volatile int distance;
volatile int distance2;
volatile int distance3;
volatile int distance4;
int time1;
int time2;

int servoPin = 10;
Servo Servo1;

int btn[4];


LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int i = 0;
int h = 0;
int m = 0;
int s = 0;
int d = 0;
int mn = 0;   //month
int y = 0;
char buffer[30];


void check_temp()
{
  sensorInput = analogRead(sensePin);
  R2 = 10000 * (1023.0 / (float)sensorInput - 1.0);
  logR2 = log(R2);
  temp = (1.0 / (1.009249522e-03 + 2.378405444e-04 * logR2 + 2.019202697e-07 * logR2 * logR2 * logR2));
  temp = temp - 273.15;
  temp = (temp * 9.0) / 5.0 + 32.0;
  delay(1000);
}


void check_dist()
{
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  time1 = pulseIn(echo_pin, HIGH);
  distance = (time1 * 0.034) / 2;
}


void check_dist2()
{
  digitalWrite (trigger_pin2, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigger_pin2, LOW);
  time2 = pulseIn (echo_pin2, HIGH);
  distance2 = (time2 * 0.034) / 2;
}


void check_servo()
{
  if (Servo1.read() == 90) {
    lcd.setCursor(0,0);
    lcd.print("One door is");
    lcd.setCursor(0,1);
    lcd.print("closing");
    delay(1000);
    Servo1.write(0);
  }
  else {   
    lcd.setCursor(0,0);
    lcd.print("One door is");
    lcd.setCursor(0,1);
    lcd.print("opening");
    delay(1000);
    Servo1.write(90);
  }
}


void initLCD()
{
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Arduino");
  lcd.setCursor(1, 1);
  lcd.print("Car Dashboard");
}


void check_tone(int distance)
{
  //tone(numar pin, frecventa in hertz, durata in millisecunde);
  if (distance > 15 && distance <= 30) {
    tone(buzzer_pin, 1000, 200);
    lcd.setCursor(0, 1);
    lcd.print("Object detected   ");
  }
  else if (distance <= 15 && distance > 5) {
    tone(buzzer_pin, 1200, 500);
    lcd.setCursor(0, 1);
    lcd.print("Mind the alarm");
  }
  else if (distance <= 5) {
    tone(buzzer_pin, 1400, 700);
    lcd.setCursor(0, 1);
    lcd.print("Close enough    ");
  }
  else
    noTone(buzzer_pin);
}


void displayTime_on_lcd()
{
  lcd.setCursor(0, 1);
  i = sprintf (buffer, "%d:%d:%d                  ", h, m, s);
  for (int l = 0; l <= i; l++)
    lcd.print(buffer[l]);

  delay(1000);
}


void check_time() {
  s++;
  if (s == 60) {
    s = 0;
    m++;
  }
  else if (m == 60) {
    m = 0;
    h++;
  }
  else if (h == 24) {
    h = 0;
    d++;
  }
  else if (d == 30) {
    d = 0;
    m++;
  }
  else if (mn == 12) {
    mn = 0;
    y++;
  }
  else if (y == 9999) {
    y = 0;
    mn = 0;
    d = 0;
  }
}





void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode (trigger_pin, OUTPUT);
  pinMode (echo_pin, INPUT);

  pinMode (trigger_pin2, OUTPUT);
  pinMode (echo_pin2, INPUT);

  pinMode (buzzer_pin, OUTPUT);

  Servo1.attach(servoPin);
  Servo1.write(0);

  btn[0] = 20;
  btn[1] = 21;
  btn[2] = 44;
  btn[3] = 45;
  pinMode(btn[0], INPUT_PULLUP);
  pinMode(btn[1], INPUT_PULLUP);
  pinMode(btn[2], INPUT_PULLUP);
  pinMode(btn[3], INPUT_PULLUP);

  //attachInterrupt(digitalPinToInterrupt(pin), ISR, mode)
  attachInterrupt(digitalPinToInterrupt(btn[0]), loop, CHANGE);
  attachInterrupt(digitalPinToInterrupt(btn[1]), loop, CHANGE);
  attachInterrupt(digitalPinToInterrupt(btn[2]), loop, CHANGE);
  attachInterrupt(digitalPinToInterrupt(btn[3]), loop, CHANGE);

  initLCD();
}



void loop()
{

  check_time();

  for (int i = 0; i < 4; i++) {
    switch (btn[i])
    {
      case 20 :
        if (digitalRead(20) == 0)
        {
          check_temp();
          check_dist();
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Temp: ");
          lcd.print(temp);
          lcd.print((char)223);
          lcd.print("C");
          lcd.setCursor(0, 1);
          if (distance < 5)
            lcd.print("Doors closed");
          else
            lcd.print("Door open");
          delay(3000);
          initLCD();
        }
        break;
      case 21 :
        if (digitalRead(21) == 0)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("distance:");
          check_dist2();
          lcd.print(distance2);
          lcd.print("        ");
          check_tone(distance2);
          delay(500);
          initLCD();
        }
        break;
      case 44 :
        if (digitalRead(44) == 0)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("hh:mm:ss");
          displayTime_on_lcd();
          delay(1500);
          lcd.clear();
          initLCD();
          break;
        }
      case 45 :
        if (digitalRead(45) == 0)
        {
          lcd.clear();
          check_servo();
          delay(1500);
          lcd.clear();
          initLCD();
          break;
        }
      default :  break;
    }
  }


}
