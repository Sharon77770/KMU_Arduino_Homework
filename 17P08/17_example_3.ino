#include <Servo.h>

// Arduino pin assignment
#define PIN_IR    0         // IR sensor pin A0
#define PIN_LED   9
#define PIN_SERVO 10

#define _DUTY_MIN 500       // Servo motor at 0 degree position
#define _DUTY_NEU 1500      // Servo motor at neutral position (90 degrees)
#define _DUTY_MAX 2500      // Servo motor at 180 degree position

#define _DIST_MIN  100.0    // Minimum distance 100mm
#define _DIST_MAX  250.0    // Maximum distance 250mm

#define EMA_ALPHA 0.2       // EMA filter coefficient

#define LOOP_INTERVAL 20    // Loop interval (unit: ms)

Servo myservo;
unsigned long last_loop_time = 0;  // unit: ms

float dist_prev = _DIST_MIN;
float dist_ema = _DIST_MIN;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(1000000);    // 1,000,000 bps
}

void loop()
{
  unsigned long time_curr = millis();
  int duty;
  float a_value, dist_raw;

  // Wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  a_value = analogRead(PIN_IR);
  dist_raw = ((6762.0 / (a_value - 9.0)) - 4.0) * 10.0;

  if (dist_raw >= _DIST_MIN && dist_raw <= _DIST_MAX) {
    digitalWrite(PIN_LED, LOW);  // Turn on LED if in range
  } else {
    digitalWrite(PIN_LED, HIGH); 
    dist_raw = dist_prev;        
  }
  dist_prev = dist_raw;

  dist_ema = (EMA_ALPHA * dist_raw) + ((1 - EMA_ALPHA) * dist_ema);
      
  duty = _DUTY_MIN + (int)((dist_ema - _DIST_MIN) * (_DUTY_MAX - _DUTY_MIN) / (_DIST_MAX - _DIST_MIN));
  
  myservo.writeMicroseconds(duty);

  Serial.print("_DUTY_MIN:");  Serial.print(_DUTY_MIN);
  Serial.print("_DIST_MIN:");  Serial.print(_DIST_MIN);
  Serial.print(",IR:");        Serial.print(a_value);
  Serial.print(",dist_raw:");  Serial.print(dist_raw);
  Serial.print(",ema:");       Serial.print(dist_ema);
  Serial.print(",servo:");     Serial.print(duty);
  Serial.print(",_DIST_MAX:"); Serial.print(_DIST_MAX);
  Serial.print(",_DUTY_MAX:"); Serial.print(_DUTY_MAX);
  Serial.println("");
}
