/*
  Line Follower - ATTiny85 project

  This project will use an ATTiny85 and 3 TCRT5000L sensors to build a line follower.

                    +-\/-+
   Ain0 (D 5) PB5  1|    |8  Vcc
   Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1
   Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
              GND  4|    |5  PB0 (D 0) pwm0
                    +----+
*/

// Constants
#define EMITTER_LED PB0            // This will control all 3 TCRT5000L emitter LEDs.
#define PHOTOTRANSISTOR_LEFT PB2   // The left TCRT5000L phototransistor input.
#define PHOTOTRANSISTOR_CENTER PB3 // The center TCRT5000L phototransistor input.
#define PHOTOTRANSISTOR_RIGHT PB4  // The right TCRT5000L phototransistor input.
#define TOGGLE_READ_INTERVAL 500   // The time in milliseconds to read values between toggling the EMITTER_LED.

// Variables
boolean perform_action;
boolean phototransistor_on;
int phototransistor_left_value;
int phototransistor_center_value;
int phototransistor_right_value;
unsigned long last_time_check;

void setup() {
  pinMode(EMITTER_LED, OUTPUT);
  // pinMode(PB1, OUTPUT); // LED_BUILTIN // Have not figured out what to do with this one yet.
  pinMode(PHOTOTRANSISTOR_LEFT, INPUT);
  pinMode(PHOTOTRANSISTOR_CENTER, INPUT);
  pinMode(PHOTOTRANSISTOR_RIGHT, INPUT);
  
  perform_action = false;
  turn_emitter_led_on();
  last_time_check = millis();
}

void loop() {
  if(perform_action) {
    perform_action = false;
  }
  read_detectors();
}

void read_detectors() {
  if(last_time_check + TOGGLE_READ_INTERVAL > millis()) { return; }
  
  read_phototransistor_state_values();  
  last_time_check = millis();
}

void read_phototransistor_state_values() {
  if(phototransistor_on) {
    phototransistor_left_value = analogRead(PHOTOTRANSISTOR_LEFT);
    phototransistor_center_value = analogRead(PHOTOTRANSISTOR_CENTER);
    phototransistor_right_value = analogRead(PHOTOTRANSISTOR_RIGHT);
  } else {
    phototransistor_left_value -= analogRead(PHOTOTRANSISTOR_LEFT);
    phototransistor_center_value -= analogRead(PHOTOTRANSISTOR_CENTER);
    phototransistor_right_value -= analogRead(PHOTOTRANSISTOR_RIGHT);  
    perform_action = true;
  }
  phototransistor_on = !phototransistor_on;
}

void turn_emitter_led_on() {
  phototransistor_on = true;
  digitalWrite(EMITTER_LED, HIGH);
}

void turn_emitter_led_off() {
  phototransistor_on = false;
  digitalWrite(EMITTER_LED, LOW);
}
