#include <BleKeyboard.h>

#define ENCODER_PIN_A_1 22       // Pin connected to encoder 1 pin A
#define ENCODER_PIN_A_2 14       // Pin connected to encoder 2 pin A
#define BUTTON_PIN 23            // Pin connected to the button
#define SCROLL_DELAY 50          // Delay between each scroll event (in milliseconds)
#define LETTER_DELAY 500         // Delay after printing each letter (milliseconds)
#define RESET_DELAY 1300         // Delay for resetting letter 
#define LONG_PRESS_DELAY 500    // Duration to detect as a long press (in milliseconds)
#define DEBOUNCE_DELAY 50        // Button debounce delay (in milliseconds)

BleKeyboard bleKeyboard;
bool isRotating1 = false;           // Flag to track encoder 1 rotation
bool isRotating2 = false;           // Flag to track encoder 2 rotation
char currentLetter1 = 'A';          // Initial letter for encoder 1
char currentLetter2 = 'F';          // Initial letter for encoder 2
unsigned long previousPrintTime1 = 0; // Time of last letter printing for encoder 1
unsigned long previousPrintTime2 = 0; // Time of last letter printing for encoder 2
bool buttonPressed = false;         // Flag to indicate button press
unsigned long buttonPressTime = 0;  // Time of button press
bool longPressHandled = false;      // Flag to indicate if long press action has been handled

void setup() {
  Serial.begin(115200);
  pinMode(ENCODER_PIN_A_1, INPUT_PULLUP);
  pinMode(ENCODER_PIN_A_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  bleKeyboard.begin();
}

void loop() {
  // Check for button press
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    delay(DEBOUNCE_DELAY);  // Button debounce delay
    if (digitalRead(BUTTON_PIN) == LOW) {  // Make sure the button is still pressed after debounce delay
      buttonPressed = true;
      buttonPressTime = millis();
      // Print space character when the button is pressed
      bleKeyboard.write(' ');
    }
  }
  
  // Check for button press
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    delay(DEBOUNCE_DELAY);  // Button debounce delay
    if (digitalRead(BUTTON_PIN) == LOW) {  // Ensure button is still pressed after debounce
      buttonPressed = true;
      buttonPressTime = millis();
      bleKeyboard.write(' ');  // Print space on initial press
    }
  }

  // Check for button release or long press duration
  if (buttonPressed) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
      // Button released
      buttonPressed = false;
    } else if (millis() - buttonPressTime >= LONG_PRESS_DELAY) {
      // Long press threshold reached, start continuous backspaces
      bleKeyboard.write('\b');
      delay(150);  // Delay between backspaces
    }
  }

  // Handling for encoder 1
  int currentEncoderStateA1 = digitalRead(ENCODER_PIN_A_1);

  if (!isRotating1 && currentEncoderStateA1 == LOW) {
    isRotating1 = true;

    unsigned long currentTime1 = millis();

    if (currentTime1 - previousPrintTime1 >= RESET_DELAY) {
      // Reset current letter to 'A' after the specified delay
      currentLetter1 = 'A';
    }

    if (currentTime1 - previousPrintTime1 <= 1200) {
      bleKeyboard.write('\b'); // Backspace to remove previous letter
    }

    bleKeyboard.write(currentLetter1);
    previousPrintTime1 = currentTime1;
    delay(LETTER_DELAY);

    currentLetter1 = (currentLetter1 == 'E') ? 'A' : currentLetter1 + 1;
  } else if (isRotating1 && currentEncoderStateA1 == HIGH) {
    isRotating1 = false;
  }

  // Handling for encoder 2
  int currentEncoderStateA2 = digitalRead(ENCODER_PIN_A_2);

  if (!isRotating2 && currentEncoderStateA2 == LOW) {
    isRotating2 = true;

    unsigned long currentTime2 = millis();

    if (currentTime2 - previousPrintTime2 >= RESET_DELAY) {
      // Reset current letter to 'F' after the specified delay
      currentLetter2 = 'F';
    }

    if (currentTime2 - previousPrintTime2 <= 1200) {
      bleKeyboard.write('\b'); // Backspace to remove previous letter
    }

    bleKeyboard.write(currentLetter2);
    previousPrintTime2 = currentTime2;
    delay(LETTER_DELAY);

    currentLetter2 = (currentLetter2 == 'J') ? 'F' : currentLetter2 + 1;
  } else if (isRotating2 && currentEncoderStateA2 == HIGH) {
    isRotating2 = false;
  }
}
