#include <TinyWireS.h>

int i2cAddress = 0x3b;
int pinR = 1;
int pinG = 3;
int pinB = 4;

void setup() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  TinyWireS.begin(i2cAddress);
}

void loop() {
  while(0 < TinyWireS.available()) {
    int value = TinyWireS.receive();
    
    if (1 == value) {
      digitalWrite(pinR, LOW);
      digitalWrite(pinG, HIGH);
      digitalWrite(pinB, LOW);
    }
    
    if (0 == value) {
      digitalWrite(pinR, HIGH);
      digitalWrite(pinG, LOW);
      digitalWrite(pinB, LOW);
    }
  }
}
