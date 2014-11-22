#include <Adafruit_NeoPixel.h>
#include <TinyWireS.h>


int i2cAddress = 0x3b;

int pin = 1;
int numPixels = 12;

int currentStatus = -1;

int animationState = 0;
int wait = 50;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  TinyWireS.begin(i2cAddress);
}

void loop() {
  int oldStatus = currentStatus;

  // Read the bus
  while(0 < TinyWireS.available()) {
    // update currentStatus if we have a new value
    int value = TinyWireS.receive();
    currentStatus = value;
  }

  switch(currentStatus) {
  case 1:
    // Build is good
    // NeoPixel shows constant green (only need to set it on entering this state)
    if (currentStatus != oldStatus)
    {
      setConstantColor(strip.Color(0,255,0));
    }
    break;
  case 0:
    // Build is broken
    // NeoPixel chases red
    if (currentStatus != oldStatus) {
      animationState = 0;
    }
    if (numPixels - 1 < animationState) {
      animationState = 0;
    }
    
    setChasingRed(animationState);
    animationState++;
    break;
  default:
    // Build is unknown
    // NeoPixel throbs blue
    if (currentStatus != oldStatus) {
      animationState = 0;
    }
    if (31 < animationState) {
      animationState = 0; 
    }
    if (16 > animationState) {
      setConstantColor(strip.Color(0, 0, animationState * 16));
    }
    else {
      setConstantColor(strip.Color(0, 0, (31 - animationState) * 16));
    }
    animationState++;
    break;
  }

  delay(wait);
}

void setConstantColor(uint32_t color) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void setChasingRed(int pixel) {
  for(int i=0; i< strip.numPixels(); i++) {
    if (i == pixel) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
    }
    else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
  }
}



