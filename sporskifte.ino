//      .-----.                                              .-----.
//        \ /                                                  \X/
//         Y                                                    |
//      ___|___                                            .---------.
//   .-/___=___\-.         +-------------------+          /___________\
//   ||_|_______||         |    Snobbernes     |         ||     |     ||
//   ||         ||         | Modeljernbaneklub |         ||_____|_____||
//   ||         ||         |-------------------|         |     [O]     |
//   ||_________||         |                   |         |             |
//   ||         ||         |                   |         | [OO]===[OO] |
//   ||:OO___OO:||  _______|___________________|________ |`==========='|
//   ||()[<X>]__|| | Program made by Mikkel Soede 2017  | \           /
//     =|===\=|=   |                                    |  `-.-----.-'
//   _==^=====^==__|                                    |__==^=====^==__
//
//
// Program functionality
// Setup()
//  - Init serial commination
//  - load all data from EEPROM
//
// Loop()
//  - listen button input's
//     - when button is pressed then set_relay() is called
//
// set_relay()
//  - change the sate of a relay
//  - save the sate to EEPROM

#include <EEPROM.h>

// set pin numbers:
int buttons[16] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};
int leds[16] =    { 2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 22, 24, 26, 28};
int relays[16] =  {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 46, 47, 48, 49};

int relay_state[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int button_time[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int count = 0;


struct RELAY{
  int button[2];
  int led[2];
  int relay[2];
};

RELAY r1 = {{0,0},{0,0},{0,0}};  
RELAY r2 = {{0,0},{0,0},{0,0}};  
RELAY r3 = {{0,0},{0,0},{0,0}};  
RELAY r4 = {{0,0},{0,0},{0,0}};  

//RELAY relays[] ;

void setup() {
  Serial.begin(115200);
  Serial.println("Snobbernes modeljernbaneklub");
  Serial.println("Sporskifte program started!");


  for (int i = 0; i < 16; i++) {
    Serial.println(i);
    //Setup pin connections
    pinMode(buttons[i], INPUT);
    pinMode(leds[i], OUTPUT);
    pinMode(relays[i], OUTPUT);
    relay_state[i] = EEPROM.read(i); //load values from EEMPROM
    if (relay_state[i] == HIGH) {
      set_relay(i); //Set relays from relay_state
    }
  }
  Serial.println("Setup done");
}

void set_relay(int relay) {
  int tmp = relay % 2 == 0 ? relay + 1 : relay - 1; //Calculate the relay in the same group

  digitalWrite(relay, HIGH); //turn on relay
  relay_state[relay] = HIGH; //change relay state
  EEPROM.write(relay, HIGH); //save relay state to EEPROM
  EEPROM.write(tmp, LOW); //clear other track IN EEPROM
  relay_state[tmp] = LOW; //Clear other relay state

  //set leds
  digitalWrite(leds[relay], HIGH);
  digitalWrite(leds[tmp], LOW);

  Serial.print("led ");
  Serial.print(leds[relay]);
  Serial.print(" : ");
  Serial.println(leds[tmp]);

  delay(100); //wait 1/10 sec
  digitalWrite(relay, LOW); //turn off relay
}

void loop() {
  for (int i = 0; i < 16; i++) {
    int val = analogRead(buttons[i]);
    if (val >= 500) {
      if (button_time[i] == 0) {
        set_relay(i);
        button_time[i] = 1;
      } else {
        button_time[i] = button_time[i] + 1;

        if (button_time[i] > 10)
          button_time[i] = 0;

        delay(100);
      }
    } else {
      button_time[i] = 0;
    }
  }
}
