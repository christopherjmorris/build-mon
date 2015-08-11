#include <Adafruit_NeoPixel.h>
//#include <TinyWireS.h>


//int i2cAddress = 0x3b;

int neopixelDataPin = 4;
int numPixels = 12;

int buttonPin = 12;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int currentStatus = -1;
int oldStatus;

int animationState = 0;
long lastAnimationTime = 0;
long animationDelay = 50;

long currentTime;
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neopixelDataPin, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(buttonPin, INPUT);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //TinyWireS.begin(i2cAddress);
  
  oldStatus = -99;
}

void loop() {
  currentTime = millis();
  
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  
  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = currentTime;
  } 
  
  if ((currentTime - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only run the button pressed function if the new button state is HIGH
      if (buttonState == HIGH) {
        onButtonPressed();
      }
    }
  }
  
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
  
/*
  // Read the bus
  while(0 < TinyWireS.available()) {
    // update currentStatus if we have a new value
    int value = TinyWireS.receive();
    currentStatus = value;
  }
*/

  // Check to see if it is time to do the next animation frame
  if ((currentTime - lastAnimationTime) > animationDelay) {
    onAnimationFrame();
    lastAnimationTime = currentTime;
  }
}

void onButtonPressed() {
  currentStatus++;
  if (currentStatus > 1) {
    currentStatus = -1;
  }
}

void onAnimationFrame() {
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
  
  oldStatus = currentStatus;
}


void setConstantColor(uint32_t color) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void setChasingRed(int pixel) {
  for(uint16_t i=0; i< strip.numPixels(); i++) {
    if (i == pixel) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
    }
    else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
  }
}



