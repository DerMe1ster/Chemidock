#define rele 12

uint64_t time;
bool flag;

void setup() {
  pinMode(rele, OUTPUT);
}

void loop() {
  if(millis() - time > 500) {
    time = millis();
    flag = !flag;
    digitalWrite(rele, flag);
  }
}
