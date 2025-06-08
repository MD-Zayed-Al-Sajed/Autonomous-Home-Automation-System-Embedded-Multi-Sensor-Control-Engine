//Arduino 1 
#include <Adafruit_LiquidCrystal.h>
int windowMoto1 = 12;
int windowMoto2 = 13;
int button1 = 6;
int button2 = 2; 


int smokeSensorPin = A0;
int fireSensorPin = A1;
int buzzerPin = 7;
int fireLEDblue = 4;
int fireLEDred = 5;

int waterPump = 8;
int waterLEDred = 10;
int waterLEDgreen = 9;

int roomBulb = 11; // Corrected from ledPin to roomBulb
int pirSensor = 3;
unsigned long lastMotionTime = 0; // Time when the motion stopped
bool isBulbOn = false; // Current state of the room bulb

int soil_moisture_sensor = 2;

// Define the display object with the name 'plant_water_display'
Adafruit_LiquidCrystal plant_water_display(0);

void setup() {
  pinMode(smokeSensorPin, INPUT);
  pinMode(fireSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(fireLEDblue, OUTPUT);
  pinMode(fireLEDred, OUTPUT);
  
  pinMode(roomBulb, OUTPUT);
  digitalWrite(roomBulb, HIGH);
  
  pinMode(A2, INPUT); //soil_moisture_sensor
  pinMode(waterPump, OUTPUT);
  pinMode(waterLEDred, OUTPUT);
  pinMode(waterLEDgreen, OUTPUT);
  plant_water_display.begin(16, 2); // Initialize the LCD display
  Serial.begin(9600);
  
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(windowMoto1, OUTPUT);
  pinMode(windowMoto2, OUTPUT);
}

void loop() {
  int smokeLevel = analogRead(smokeSensorPin);
  int fireLevel = analogRead(fireSensorPin);
  
  Serial.print("Smoke level: ");
  Serial.println(smokeLevel); // Print smoke level to serial monitor for testing
  Serial.print("Fire level: ");
  Serial.println(fireLevel); // Print fire level to serial monitor for testing

  if ((smokeLevel > 300 || fireLevel > 250) && fireLevel > 198) {
    // Generate a professional-sounding tone
    for (int i = 0; i < 2000; i++) { // Adjust the duration of the tone by changing the loop count
      digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
      delayMicroseconds(100); // Adjust the delay for the desired frequency
      digitalWrite(buzzerPin, LOW); // Turn the buzzer off
      delayMicroseconds(40); // Adjust the delay for the desired frequency
    }

    digitalWrite(fireLEDblue, HIGH);
    digitalWrite(roomBulb, HIGH);
    digitalWrite(fireLEDred, LOW);
    delay(300);
    digitalWrite(fireLEDblue, LOW);
    digitalWrite(roomBulb, LOW);
    digitalWrite(fireLEDred, HIGH);
    delay(300); // Delay before rechecking
    plant_water_display.setCursor(0, 0);
    plant_water_display.print("Fire! Fire!    ");
    delay(500);
  } else if ((smokeLevel > 200  || fireLevel > 198) && fireLevel > 198) {
    // Generate a professional-sounding tone
    for (int i = 0; i < 2000; i++) { // Adjust the duration of the tone by changing the loop count
      digitalWrite(buzzerPin, HIGH); // Turn the buzzer on
      delayMicroseconds(100); // Adjust the delay for the desired frequency
      digitalWrite(buzzerPin, LOW); // Turn the buzzer off
      delayMicroseconds(50); // Adjust the delay for the desired frequency
    }

    digitalWrite(fireLEDblue, HIGH);
    digitalWrite(roomBulb, HIGH);
    digitalWrite(fireLEDred, LOW);
    delay(500);
    digitalWrite(fireLEDblue, LOW);
    digitalWrite(roomBulb, LOW);
    digitalWrite(fireLEDred, HIGH);
    delay(500); // Delay before rechecking
    plant_water_display.setCursor(0, 0);
    plant_water_display.print("Fire! Fire!    ");
    delay(500);
  }
  delay(100); // Adjust delay as needed
  /////////////////////////////////////////////////////////////////
  // Read the analog value from the soil moisture sensor
  soil_moisture_sensor = analogRead(A2);
  // Print the soil moisture value to the serial monitor for debugging
  Serial.println(soil_moisture_sensor);
  // Check if the soil needs water or not
  if (soil_moisture_sensor > 150) {  // Changed from "<" to ">"
    // If moisture is sufficient, turn off the watering system and other devices
    digitalWrite(waterPump, LOW);
    digitalWrite(waterLEDred, LOW);
    digitalWrite(waterLEDgreen, HIGH);
    // Display a message on the LCD indicating that the soil is in good condition
    plant_water_display.setCursor(0, 0);
    plant_water_display.print("Soil is good.  ");
  } else if (soil_moisture_sensor <= 150) {  // Changed from ">" to "<="
    // If moisture is low, turn on the watering system
    digitalWrite(waterPump, HIGH);
    digitalWrite(waterLEDred, HIGH);
    digitalWrite(waterLEDgreen, LOW);
    // Display a message on the LCD indicating that watering is in process
    plant_water_display.setCursor(0, 0);
    plant_water_display.print("Watering!    ");
  }
  // Delay a little bit to improve simulation performance
  delay(100);
  
  ///////////////////////////////////////////////////////////////
  //digitalWrite(roomBulb, HIGH);
  
  bool val = digitalRead(pirSensor); // Read input value from the PIR sensor
  if (val == HIGH) { // If motion is detected
    if (!isBulbOn) {
      Serial.println("Motion detected!");
      digitalWrite(roomBulb, LOW); // Turn the room bulb ON
      isBulbOn = true;
      lastMotionTime = millis(); // Update the last motion time
    }
  } else {
    if (isBulbOn && (millis() - lastMotionTime > 10000)) {
      // If the room bulb is on and it has been more than 1 minute since motion was detected
      Serial.println("Motion ended! Turning off the bulb.");
      digitalWrite(roomBulb, HIGH); // Turn the room bulb OFF
      isBulbOn = false;
    }
  }
  if (digitalRead(button1) == LOW) { 
    digitalWrite(windowMoto1, HIGH); 
    digitalWrite(windowMoto2, LOW);
  } else if (digitalRead(button2) == LOW) { 
    digitalWrite(windowMoto1, LOW); 
    digitalWrite(windowMoto2, HIGH);
  } else {
    // If neither button is pressed, turn off the motors.
    digitalWrite(windowMoto1, LOW);
    digitalWrite(windowMoto2, LOW);
  }
}