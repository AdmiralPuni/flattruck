#include <Servo.h>

#define IN_FORWARD   8
#define IN_REVERSE   9
#define IN_LEFT      10
#define IN_RIGHT     11

#define OUT_FORWARD  2
#define OUT_REVERSE  3
#define OUT_SERVO    4

unsigned long int prev_time = 0;
unsigned long int print_time = 0;


Servo turn_servo;

//update 60 times per second
const int tickrate = 60;
const int ticktime = 1000 / tickrate;
const int turn_degree = 40;
const int init_degree = 75;

int servo_pos = init_degree;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:

  pinMode(IN_FORWARD, INPUT);
  pinMode(IN_REVERSE, INPUT_PULLUP);
  pinMode(IN_LEFT,    INPUT_PULLUP);
  pinMode(IN_RIGHT,   INPUT_PULLUP);

  pinMode(OUT_FORWARD, OUTPUT);
  pinMode(OUT_REVERSE, OUTPUT);

  turn_servo.attach(OUT_SERVO);
  turn_servo.write(init_degree);

  
  prev_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - prev_time > ticktime) {
    prev_time = millis();
    update();
  }

  //print input conditions every 500ms
  if (millis() - print_time > 500) {
    print_time = millis();
    Serial.print("Forward: ");
    Serial.println(!digitalRead(IN_FORWARD));
    Serial.print("Reverse: ");
    Serial.println(!digitalRead(IN_REVERSE));
    Serial.print("Left: ");
    Serial.println(digitalRead(IN_LEFT));
    Serial.print("Right: ");
    Serial.println(digitalRead(IN_RIGHT));
  }
}

void update() {
  //read inputs
  bool forward = !digitalRead(IN_FORWARD);
  bool reverse = !digitalRead(IN_REVERSE);
  bool left    = digitalRead(IN_LEFT);
  bool right   = digitalRead(IN_RIGHT);

  //set outputs
  digitalWrite(OUT_FORWARD, forward);
  digitalWrite(OUT_REVERSE, reverse);

  //set servo
  if (left) {
    servo_pos = init_degree - turn_degree;
  } else if (right) {
    servo_pos = init_degree + turn_degree;
  } else {
    servo_pos = init_degree;
  }
  
  turn_servo.write(servo_pos);
}
