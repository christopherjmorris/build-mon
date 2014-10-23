#include <TinyWireS.h>

int i2cAddress = 0x3b;
int pinLed = 1;

void setup() {
  pinMode(pinLed, OUTPUT);
  TinyWireS.begin(i2cAddress);
}

void loop() {
  while(0 > TinyWireS.available()) {
    int value = TinyWireS.receive();
    
    if (1 == value) {
      digitalWrite(pinLed, HIGH);
    }
    
    if (0 == value) {
      digitalWrite(pinLed, LOW);
    }
  }
}
