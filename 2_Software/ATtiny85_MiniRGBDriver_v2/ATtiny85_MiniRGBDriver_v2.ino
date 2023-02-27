/*
  - Developed by Manuel Lagarto, Lizard Electronics 2022
  - Simple program for ATtiny85 to control RGB LED with color modes:    
        +---------------+-----+-------+-------+
        | Color         | Red | Green | Blue  |
        +---------------+-----+-------+-------+
    (0) | Red     (000) | 100 | 0     | 0     | -> Red     = Red
    (1) | Yellow  (001) | 100 | 100   | 0     | -> Yellow  = Red   + Green
    (2) | Green   (010) | 0   | 100   | 0     | -> Green   = Green
    (3) | Cyan    (011) | 0   | 100   | 100   | -> Cyan    = Green + Blue
    (4) | Blue    (100) | 0   | 0     | 100   | -> Blue    = Blue
    (5) | Magenta (101) | 100 | 0     | 100   | -> Magenta = Red   + Blue
    (6) | White   (110) | 100 | 100   | 100   | -> White   = Red   + Green + Blue
        +---------------+-----+-------+-------+
*/

// Constant Variables
const int redPin = 2;
const int greenPin = 1;
const int bluePin = 0;

const int buttonPin = 3;

// Variables
unsigned char nextMode = 0;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(buttonPin, INPUT);
}

void loop() {
  label:
  checkButton();
  changeMode();
}

void checkButton() {
  unsigned char buttonState = 0;
  buttonState = readButton();
  if(buttonState == 1){
    ++nextMode;
    changeMode();
  }
}

unsigned char readButton(){
  // Reads current state of the button
  int reading = digitalRead(buttonPin);
  unsigned char buttonFlag = 0;
  static unsigned long lastDebounceTime = 0;
  static unsigned long debounceDelay = 50;    // the debounce time (ms)
  static unsigned char buttonState = 0;
  static unsigned char lastButtonState = 0;
  
  // If button changed, due to noise or pressing
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Whatever the reading is at, it's been there for longer than the 
    // debounce delay, so take it as the actual current state

    // If the button state has changed
    if (reading != buttonState) {
      buttonState = reading;

      // Action if button HIGH
      if (buttonState == HIGH) {
        buttonFlag = !buttonFlag;
      }
    }
  }
  lastButtonState = reading;
  return buttonFlag;
}

void changeMode() {
  if(nextMode>=3) {
    nextMode=0;
  }
  // RGB modes
  switch (nextMode) {
    case 0:
      fadeMode();
      break;

    case 1:
      sequenceMode();
      break;

    case 2:
      singleColorMode();
      break;
    
    default:
      redColor();
      break;
  }
}

//========================================================
// Fade mode (start)
void fadeMode() {
  redToYellow();
  yellowToGreen();
  greenToCyan();
  cyanToBlue();
  blueToMagenta();
  magentaToRed();
}

void fadeUp (int colorPin){
  // Duty cycle, fade up colorPin
  for(byte a=0; a<=100; ++a) {
    byte on=a;
    byte off=100-a;
    for(byte b=0; b<=100; ++b) {
      digitalWrite(colorPin, HIGH);
      delayMicroseconds(on);
      digitalWrite(colorPin, LOW);
      delayMicroseconds(off);
    }
    checkButton();
  }
}

void fadeDown (int colorPin){
  // Duty cycle, fade down colorPin
  for(byte a=0; a<=100; ++a) {
    byte on=100-a;
    byte off=a;
    for(byte b=0; b<=100; ++b) {
      digitalWrite(colorPin, HIGH);
      delayMicroseconds(on);
      digitalWrite(colorPin, LOW);
      delayMicroseconds(off);
    }
    checkButton();
  }
}

void redToYellow() {
  digitalWrite(redPin, HIGH);
  digitalWrite(bluePin, LOW);

  fadeUp(greenPin);
}

void yellowToGreen() {
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);

  fadeDown(redPin);
}

void greenToCyan() {
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
  
  fadeUp(bluePin);
}

void cyanToBlue() {
  digitalWrite(bluePin, HIGH);
  digitalWrite(redPin, LOW);

  fadeDown(greenPin);
}

void blueToMagenta() {
  digitalWrite(bluePin, HIGH);
  digitalWrite(greenPin, LOW);

  fadeUp(redPin);
}

void magentaToRed() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);

  fadeDown(bluePin);
}
// Fade mode (end)
//--------------------------------------------------------

//========================================================
// Sequence mode (start)
void sequenceMode() {
  unsigned char button;
  int currentTime=millis();
  for (int colorSelected = 1; colorSelected<=7; ++colorSelected){
    switch (colorSelected) {
      case 1:
        redColor();
        break;

      case 2:
        yellowColor();
        break;

      case 3:
        greenColor();
        break;

      case 4:
        cyanColor();
        break;

      case 5:
        blueColor();
        break;

      case 6:
        magentaColor();
        break;

      case 7:
        whiteColor();
        break;

      default:
        redColor();
        break;
    }
//    while((millis() - currentTime) < 1000){
//      checkButton();
//    }
    if(digitalRead(buttonPin) == 1)
      checkButton();
    checkButton();
    delay(1000);
    checkButton();
  }  
}
// Sequence mode (end)
//--------------------------------------------------------

//========================================================
// Single color mode (start)
void singleColorMode() {
  int colorSelected=1;

  while(colorSelected < 8) {
    unsigned char buttonState = 0;
    buttonState = readButton();
    if(buttonState == 1){
      ++colorSelected;
      if(colorSelected >= 8){
        ++nextMode;
        changeMode();
      }
    }

    switch (colorSelected) {
      case 1:
        redColor();
        break;

      case 2:
        yellowColor();
        break;

      case 3:
        greenColor();
        break;

      case 4:
        cyanColor();
        break;

      case 5:
        blueColor();
        break;

      case 6:
        magentaColor();
        break;

      case 7:
        whiteColor();
        break;

      default:
        redColor();
        break;
    }
  }
}
// Single color mode (end)
//--------------------------------------------------------

//========================================================
// Colors (start)
void redColor() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

void yellowColor() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
}

void greenColor() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
}

void cyanColor() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
}

void blueColor() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
}

void magentaColor() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
}

void whiteColor() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
}
// Colors (end)
//--------------------------------------------------------
