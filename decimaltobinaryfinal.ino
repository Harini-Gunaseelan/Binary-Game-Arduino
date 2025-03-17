#define BUTTON_0 2  
#define BUTTON_1 3  
#define BUTTON_BACKSPACE 4  
#define BUTTON_ENTER 5  
#define GREEN_LED 6  
#define RED_LED 7  
#define BUZZER 8  

#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int targetNumber = 0;  
int userInput = 0;
int bitPosition = 3;  
unsigned long startTime;  
const unsigned long timeLimit = 60000;  

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_0, INPUT_PULLUP);
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_BACKSPACE, INPUT_PULLUP);
    pinMode(BUTTON_ENTER, INPUT_PULLUP);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        for(;;);
    }
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    
    randomSeed(analogRead(0));
    generateNewNumber();
}

void loop() {
    if (millis() - startTime > timeLimit) {  
        displayMessage("Time Up!", "New number:");
        generateNewNumber();
        return;  
    }

    if (digitalRead(BUTTON_0) == LOW) {  
        processInput(0);
        delay(300);  
    }

    if (digitalRead(BUTTON_1) == LOW) {  
        processInput(1);
        delay(300);  
    }

    if (digitalRead(BUTTON_BACKSPACE) == LOW) {  
        backspace();
        delay(300);  
    }

    if (digitalRead(BUTTON_ENTER) == LOW) {  
        checkAnswer();
        delay(300);  
    }
}

void processInput(int bit) {
    if (bitPosition >= 0) {  
        userInput |= (bit << bitPosition);  
        bitPosition--;  
        updateDisplay();
    }
}

void backspace() {
    if (bitPosition < 3) {  
        bitPosition++;  
        userInput &= ~(1 << (bitPosition + 1));  
        updateDisplay();
    }
}

void checkAnswer() {
    if (userInput == targetNumber) {
        displayMessage("Correct!", "Generating new...");
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
        tone(BUZZER, 1000);  
        delay(300);
        noTone(BUZZER);
    } else {
        displayMessage("Wrong!", "Try again.");
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, HIGH);
        tone(BUZZER, 1000);  
        delay(300);
        noTone(BUZZER);
        delay(300);
        tone(BUZZER, 1000);
        delay(300);
        noTone(BUZZER);
    }
    delay(1000);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    generateNewNumber();
}

void generateNewNumber() {
    targetNumber = random(1, 16);  
    userInput = 0;     
    bitPosition = 3;   
    startTime = millis();  
    updateDisplay();
}

void updateDisplay() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Convert: ");
    display.println(targetNumber);
    display.print("Your input: ");
    for (int i = 3; i >= 0; i--) {
        display.print((userInput >> i) & 1);
    }
    display.display();
}

void displayMessage(String line1, String line2) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(line1);
    display.println(line2);
    display.display();
}
