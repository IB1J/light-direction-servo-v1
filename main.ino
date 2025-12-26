#include <Servo.h>
Servo myservo;

// Used to filter enviromental light noise
double enviroment_magnitude;
double enviroment_offset = 50; // How much in magnitude brighter should the source be

// debug
bool debug_print = true;
bool debug_delay = true;
bool servo_activated = true;

void setup() {
  pinMode(11, OUTPUT);
  myservo.attach(9);
  Serial.begin(9600);
}

void loop() {
  analogWrite(11, 40);

  int left_resistor_value = analogRead(A0) <= 0 ? 1 : analogRead(A0);
  int right_resistor_value = analogRead(A1) <= 0 ? 1 : analogRead(A1);

  // Set envrioment magnitude on first iteration
  if (!enviroment_magnitude) enviroment_magnitude = sqrt(pow(left_resistor_value, 2) + pow(right_resistor_value, 2));

  double magnitude = sqrt(pow(left_resistor_value, 2) + pow(right_resistor_value, 2));
  float angle = atan2(left_resistor_value, right_resistor_value) * (180 / PI) + 35; // Angle in Radians -> Angle in Degrees -> Offset of 45 Degrees

  // Filter enviromental noise
  if (magnitude >= enviroment_magnitude + enviroment_offset) {
    if (servo_activated) {
      myservo.write(angle);
    }
  } else {
      myservo.write(80);
  }
  
  /* \/ \/ \/ Debug \/ \/ \/ */
  if (debug_print) {
    Serial.print("Left: "); Serial.print(left_resistor_value);
    Serial.print(" | Right: "); Serial.print(right_resistor_value);
    Serial.print(" | Angle: "); Serial.print(angle);
    Serial.print(" | Magnitude: "); Serial.print(magnitude); Serial.print(" | "); Serial.println(enviroment_magnitude + enviroment_offset);
  }

  //Serial.println(magnitude);

  if (debug_delay) {
    delay(100);
  }
}
