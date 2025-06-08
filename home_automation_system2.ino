#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

Servo myservo;

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#define Password_Lenght 5 // Give enough room for six chars + NULL char

int pos = 0;    // variable to store the servo position
int failedAttempts = 0; // variable to store the number of failed attempts

char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght] = "1234";
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool door = true;

byte rowPins[ROWS] = {1, 2, 3, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 6, 7, 8}; //connect to the column pinouts of the keypad

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad

void setup()
{
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  myservo.attach(9);
  ServoClose();
  lcd.begin(16, 2);
  lcd.print(" Arduino Door");
  lcd.setCursor(0, 1);
  lcd.print("--Look project--");
  delay(3000);
  lcd.clear();

}

void loop()
{
  if (door == 0)
  {
    customKey = customKeypad.getKey();

    if (customKey == '#')

    {
      lcd.clear();
      ServoClose();
      lcd.print("  Door is close");
      digitalWrite(10,LOW);
      delay(3000);
      door = 1;
    }
  }

  else Open();
}

void clearData()
{
  while (data_count != 0)
  { // This can be used for any array size,
    Data[data_count--] = 0; //clear array for new data
  }
  return;
}

void ServoOpen()
{
  for (pos = 180; pos >= 0; pos -= 5) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void ServoClose()
{
  for (pos = 0; pos <= 180; pos += 5) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password");
  
  customKey = customKeypad.getKey();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey; // store char into data array
    lcd.setCursor(data_count, 1); // move cursor to show each new char
    lcd.print(Data[data_count]); // print char at said cursor
    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if (data_count == Password_Lenght - 1) // if the array index is equal to the number of expected chars, compare data to master
  {
    if (!strcmp(Data, Master)) // equal to (strcmp(Data, Master) == 0)
    {
      lcd.clear();
      digitalWrite(10,LOW);
      digitalWrite(11,HIGH);
      ServoOpen();
      lcd.print("  Door is Open");
      delay(1000);
      door = 0;
      failedAttempts = 0; // Reset failed attempts counter
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Wrong Password");
      lcd.setCursor(2, 1);
      char attemptMsg[20];
      sprintf(attemptMsg, "Try number %d", failedAttempts+1);
      lcd.print(attemptMsg);
      delay(1000);
      lcd.clear();
      digitalWrite(10,HIGH);
      digitalWrite(11,LOW);
      delay(1000);
      door = 1;
      failedAttempts++;
      if (failedAttempts >= 3) // Check if reached maximum attempts
      {
        lcd.clear();
        lcd.print("  Locked");
        delay(2000);
        door = 0; // Lock the system
        digitalWrite(10,LOW);
        while (true) {};
      }
    }
    clearData();
    delay(1000);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
  }
}