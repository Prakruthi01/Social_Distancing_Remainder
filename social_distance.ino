#include <HCSR04.h>
#include <LiquidCrystal.h>

#define trigPin 7 
#define echoPin 6 
#define buzzer 9 
#define backlight 10 

LiquidCrystal lcd(12, 11, 5, 4, 3, 8); 
UltraSonicDistanceSensor distanceSensor(trigPin, echoPin); 

int tempReading; 
double tempK; 
float tempC; 
int rounded; 
int temp_round; 
volatile boolean modes = 0; 
double distance; 

void setup() {
  lcd.begin(16, 2);
  attachInterrupt(digitalPinToInterrupt(2), changeMode, FALLING);
  pinMode(2, INPUT); 
  pinMode(buzzer, OUTPUT); 
  pinMode(backlight, OUTPUT); 
  digitalWrite(backlight, HIGH); 
  backlightOn(); 
}

void loop() { 
  tempReading = analogRead(A0); 
  tempK = log(10000.0 * ((1024.0 / tempReading) - 1)); 
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK); 
  tempC = tempK - 273.15;
  distance = distanceSensor.measureDistanceCm(tempC); 
  temp_round = round(tempC); 

  if (modes == 1) { 
    backlightOn();
    if (distance >= 300 || distance <= 0) {
      rounded = 0;
      lcd.clear();
      lcd.print("Out of range");
      lcd.setCursor(0, 1); 
      lcd.print("Temperature: " + String(temp_round) + " C"); 
    } else { 
      rounded = round(distance); 
      lcd.clear(); 
      lcd.print("Distance: "); 
      lcd.print(rounded); 
      lcd.print(" cm"); 
      lcd.setCursor(0, 1); 
      lcd.print("Temperature: " + String(temp_round) + " C"); 
    } 
  } else { 
    if (distance >= 300 || distance <= 0) { 
      rounded = 0;
      lcd.clear();
      backlightOff(); 
    } else { 
      rounded = round(distance); 
      if (distance >= 200) { 
        backlightOff(); 
        lcd.clear(); 
      } else if (distance <= 200 && distance > 100) { 
        backlightOn(); 
        lcd.clear(); 
        lcd.print("Please keep away"); 
        lcd.setCursor(0, 1); 
        lcd.print("Distance:"); 
        lcd.print(rounded); 
        lcd.print(" cm"); 
      } else if (distance <= 100 && distance > 50) { 
        backlightOn(); 
        lcd.clear(); 
        lcd.print("Keep away"); 
        lcd.setCursor(0, 1); 
        lcd.print("Distance:"); 
        lcd.print(rounded); 
        lcd.print(" cm"); 
        delay(200); 
        buzz(); 
        backlightOff(); 
        delay(100); 
        unbuzz(); 
        backlightOn(); 
        delay(100); 
      } else if (distance <= 50) { 
        backlightOn(); 
        lcd.clear(); 
        lcd.print("STAY AWAY!"); 
        lcd.setCursor(0, 1); 
        lcd.print("Distance:"); 
        lcd.print(rounded); 
        lcd.print(" cm"); 
        delay(200); 
        buzz(); 
        backlightOff(); 
        delay(200); 
        unbuzz(); 
        backlightOn(); 
        delay(200); 
        buzz(); 
        backlightOff(); 
        delay(200); 
        unbuzz(); 
        backlightOn(); 
      } 
    } 
  } 
  delay(700); 
} 

void changeMode() { 
  modes = !modes; 
}

void backlightOn() { 
  digitalWrite(backlight, HIGH); 
} 

void backlightOff() { 
  digitalWrite(backlight, LOW); 
} 

void buzz() { 
  digitalWrite(buzzer, HIGH); 
} 

void unbuzz() { 
  digitalWrite(buzzer, LOW); 
}