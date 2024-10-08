#define PIN_LED 13
unsigned int count, toggle;


inline int toggle_state(int toggle);



void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);

  while(!Serial){;}

  Serial.println("Hello World!");
  count = 0;
  toggle = 0;
  digitalWrite(PIN_LED, toggle);
}



void loop() {
  Serial.println(++count);
  
  toggle = toggle_state(toggle);
  digitalWrite(PIN_LED, toggle);
  
  delay(1000);
}



inline int toggle_state(int toggle) {
  return !toggle;
}
