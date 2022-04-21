#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define GREEN 13
#define RED 12
#define ZOOMER 2

// Keypad init
const byte ROWS = 3; 
const byte COLS = 4; 
char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'}, 
{'4','5','6','0'},
{'7','8','9','C'}
};
byte rowPins[ROWS] = {5, 4, 3}; 
byte colPins[COLS] = {9, 8, 7, 6}; 
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// LCD init
LiquidCrystal_I2C lcd (0x27, 20, 4);

volatile unsigned long globalTimeBufferMillis = 0;

void setup(){

    Serial.begin(9600);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Choose password type");

    pinMode(GREEN, OUTPUT);
    pinMode(RED, OUTPUT);
    digitalWrite(GREEN, 0);  
    digitalWrite(RED, 1);  

    tryToOpen();

}

void loop() {}

void tryToOpen() {
    openUsingKeypad();
}

String pass = "";
String key = "1234";
String enteringPassword = "";

void openUsingKeypad() {
    
    lcd.clear();
    drawCodeHeader();

    while (true) {
        char customKey = customKeypad.getKey();
        if (customKey) {
            if (customKey == 'A') {
                break;
                tone(ZOOMER, 220, 50);
            } else if (customKey == 'C') {
                enteringPassword = enteringPassword.substring(0, enteringPassword.length() - 1);
                tone(ZOOMER, 220, 50);
                drawPassword();
            } else  {
                enteringPassword += customKey;
                tone(ZOOMER, 220, 50);
                drawPassword();
            }
        }
    }

    if (enteringPassword == key) {
        confirmed();
    } else if (enteringPassword != key){
        denied();
        drawIncorrectPassword();         
    }

    enteringPassword = "";
}

void drawCodeHeader() {
    lcd.setCursor(0, 0);
    lcd.print("Enter your password:");
}

void drawPassword() {
    lcd.clear();
    drawCodeHeader();
    lcd.setCursor(0, 1);
    lcd.print(enteringPassword);
}


void confirmed() {
    drawConfirm();
    openLock();
    confirmSound();
}

void drawConfirm() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password confirmed");
}

void openLock() {
    digitalWrite(GREEN, 1);  
    digitalWrite(RED, 0); 
}

void confirmSound() {
    tone(ZOOMER, 220, 100);
    improvedDelay(200);
    tone(ZOOMER, 220, 100);
    improvedDelay(200);
    tone(ZOOMER, 220, 100);
}

void denied() {
    drawIncorrectPassword();
    deniedSound();
}

void drawIncorrectPassword() {
    digitalWrite(GREEN, 0);  
    digitalWrite(RED, 1); 
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password denied");
}

void deniedSound() {
    tone(ZOOMER, 320, 100);
    improvedDelay(200);
    tone(ZOOMER, 320, 100);
    improvedDelay(200);
    tone(ZOOMER, 320, 100);
}

void improvedDelay(unsigned int waitTime) {
    globalTimeBufferMillis = millis();
    boolean cooldownState = true;

    while (cooldownState) {
        if (millis() - globalTimeBufferMillis > waitTime) 
            cooldownState = false;
    }
}