#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define GREEN 12
#define RED 11
#define ZOOMER 10 
#define HALL 2
#define KY A3

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
byte upsideDownDegree[8]= {
    B00000, 
    B00000, 
    B00000, 
    B00000,     
    B00000, 
    B10001, 
    B01010, 
    B00100,
};
byte filledSpace[8]= {
    B11111, 
    B11111, 
    B11111, 
    B11111,     
    B11111, 
    B11111, 
    B11111, 
    B11111,
};
byte unFilledSpace[8]= {
    B11111, 
    B10001, 
    B10001, 
    B10001, 
    B10001, 
    B10001, 
    B10001, 
    B11111,
};

boolean showPassword = true;
boolean sound = false;
volatile unsigned long globalTimeBufferMillis = 0;

void setup(){

    Serial.begin(9600);

    lcd.init();
    lcd.backlight();
    lcd.createChar(1,upsideDownDegree);
    lcd.createChar(2,filledSpace);
    lcd.createChar(3,unFilledSpace);

    pinMode(GREEN, OUTPUT);
    pinMode(RED, OUTPUT);
    doorIsOpened();

    attachInterrupt(0, doorAction, FALLING);
  
    // tryToOpen();

}

void doorAction() {
    if (digitalRead(HALL)) {
        Serial.println("Door closed");
        doorIsOpened();
    } else {
        Serial.println("Attention");
        doorIsOpened();
    }
    waitMicros();
}

void waitMicros() {
    for (int i = 0; i < 300; i++) {
        
        unsigned int microsTimerBuffer = micros();
        
        boolean exitState = true;
        while (exitState) {
            if (micros() - microsTimerBuffer > 50000) {
                exitState = false;
            }
        }
    }
}

boolean doorIsOpened() {
    if (digitalRead(HALL)) {
        digitalWrite(GREEN, 0);  
        digitalWrite(RED, 1);
        return false;
    } else {
        digitalWrite(GREEN, 1);  
        digitalWrite(RED, 0);
        return true;
    }
}

void loop() {
    openMenu();
    delay(1000);
}

void openMenu() {
    drawMenu();
    listenKeyboardAndChanegeLcd();
}

void drawMenuHeader() {
    lcd.setCursor(0, 0);
    lcd.print("Choose password type");

    lcd.setCursor(0, 1);
    for (int i = 0; i < 20; i++) {
        lcd.print("-");
    }

    lcd.setCursor(0, 2);
    lcd.print("^");

    lcd.setCursor(0, 3);
    lcd.write(1);
}

void drawMenu() {
    lcd.setCursor(0, 0);
    lcd.print("Choose password type");

    lcd.setCursor(0, 1);
    for (int i = 0; i < 20; i++) {
        lcd.print("-");
    }

    lcd.setCursor(0, 2);
    lcd.print("^");

    lcd.setCursor(0, 3);
    lcd.write(1);

    keyAndRFIDScene();

    lcd.setCursor(1, 2);
    lcd.blink_on();
    
}

void keyAndRFIDScene() {
    lcd.clear();
    drawMenuHeader();

    lcd.setCursor(2, 2);
    lcd.print("Keyboard");

    lcd.setCursor(2, 3);
    lcd.print("RFID");
}

void ConfigAndRFIDScene() {
    lcd.clear();
    drawMenuHeader();

    lcd.setCursor(2, 2);
    lcd.print("RFID");

    lcd.setCursor(2, 3);
    lcd.print("Settings");
}


void listenKeyboardAndChanegeLcd() {
    // drawMenu();
    byte type = 2;

    while (true) {
        char customKey = customKeypad.getKey();

        if (customKey) {
        if (sound) tone(ZOOMER, 220, 50);


            if (customKey == '2') {
                if (type == 2) {
                    type = 4;
                    drawScene(type, false);
                } else if (type == 4) {
                    type--;
                    drawScene(type, true);
                } else {
                    type--;
                    drawScene(type, false);
                }

                
            } else if (customKey == '5') {
                if (type == 2) {
                    openUsingKeypad();
                    return;
                } else if (type == 3) {
                    openUsingRFID();
                    return;
                } else if (type == 4) {
                    openSettings();
                    return;
                }
            } else if (customKey == '8') {

                if (type == 4) {
                    type = 2;
                } else {
                    type++;
                }

                drawScene(type, false);

            }
            
        }

    }

}

void openUsingRFID() {}



void openSettings() {
    drawMenuSettings();
    listenKeyboardOnSettings();
}

void drawMenuSettings() {
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("Settings");

    lcd.setCursor(0, 1);
    if (showPassword) lcd.write(2);
    if (!showPassword) lcd.write(3);
    lcd.setCursor(2, 1);
    lcd.print("Show password");

    lcd.setCursor(0, 2);
    if (sound) lcd.write(2);
    if (!sound) lcd.write(3);
    lcd.setCursor(2, 2);
    lcd.print("Sound");

    lcd.setCursor(0, 3);
    lcd.print("Update configuration");

    lcd.setCursor(1, 1);
}

void listenKeyboardOnSettings() {
    byte typeSetting = 1;
    while (true) {
        char customKey = customKeypad.getKey();
        if (customKey) {
            if (customKey == '2') {
                if (typeSetting == 1) {
                    typeSetting = 3;
                } else {
                    typeSetting--;
                }
                setSettingsCursor(typeSetting);
            } else if (customKey == '5') {
                if (typeSetting == 1) {
                    showPassword = !showPassword;
                    printSuccesSettingsUpdate();
                    return;
                } else if (typeSetting == 2) {
                    sound = !sound;
                    printSuccesSettingsUpdate();
                    return;
                } else if (typeSetting == 3) {
                    updateConfiguration();
                }
            } else if (customKey == '8') {
                if (typeSetting == 3) {
                    typeSetting = 1;
                } else {
                    typeSetting++;
                }
                setSettingsCursor(typeSetting);
            } else if (customKey == 'C') {
                return;
            }
        }
    }
}

void setSettingsCursor(byte typeSettings) {
    if (typeSettings == 3) {
        lcd.setCursor(0, typeSettings);
    } else {
        lcd.setCursor(1, typeSettings);
    }
}

void updateConfiguration() {}

void printSuccesSettingsUpdate() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Settings updated");
}

void drawScene(byte type, boolean isPrevious) {
    if (type == 2) {
        keyAndRFIDScene();
        lcd.setCursor(1, type);
    } 

    if (type == 3) {
        if (isPrevious) {
            ConfigAndRFIDScene();
            lcd.setCursor(1, 2);
        } else {
            keyAndRFIDScene();
            lcd.setCursor(1, type);
        }
    }

    if (type == 4) {
        ConfigAndRFIDScene();
        lcd.setCursor(1, 3);
    }
    
}

void tryToOpen() {
    openUsingKeypad();
}

String pass = "";
String key = "";
String enteringPassword = "";

void openUsingKeypad() {
    
    lcd.clear();
    drawCodeHeader();

    while (true) {
        char customKey = customKeypad.getKey();
        if (customKey) {
            if (sound) tone(ZOOMER, 220, 50);
            if (customKey == 'A') {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Password check");
                break;
            } else if (customKey == 'C') {
                enteringPassword = enteringPassword.substring(0, enteringPassword.length() - 1);
                drawPassword();
            } else  {
                enteringPassword += customKey;
                drawPassword();
            }
        }
    }
    
    checkOnEsp(enteringPassword);

    checkPassword(enteringPassword);

    enteringPassword = "";
}

char constPassword[71]; 

void checkOnEsp(String enteringPassword) {
    for (int i = 0; i < sizeof(constPassword); i++) {
        constPassword[i] = '-';
    }

    String sendingData = "check:keys:" + enteringPassword;
    
    for (int i = 0; i < sendingData.length(); i++) {
        constPassword[i] = sendingData.charAt(i);
    }
    Serial.write(constPassword);
}

void checkPassword(String enteredPassword) {
    if (enteredPassword == key) {
        confirmed();
    } else {
        key = getPasswords();
        if (enteredPassword == key) {
            confirmed();
            detachInterrupt(0);
            improvedDelay(30000);
            attachInterrupt(0, doorAction, FALLING);
        } else {
            denied();
        }
    }
}

String getPasswords() {
    Serial.write("getPasswords");

    improvedDelay(2000);
    Serial.println(Serial.available());
    String buffer = "-1";
    
    if (Serial.available()) {
        buffer = Serial.readString();
        Serial.println(buffer);
    }

    Serial.println(buffer);

    return buffer;
}

void drawCodeHeader() {
    lcd.setCursor(0, 0);
    lcd.print("Enter your password:");
    lcd.setCursor(0, 1);
}

void drawPassword() {
    lcd.clear();
    drawCodeHeader();
    lcd.setCursor(0, 1);
    if (showPassword) {
        lcd.print(enteringPassword);
    } else {
        for (int i = 0; i < enteringPassword.length(); i++) {
            lcd.print("*");
        }
    }
}


void confirmed() {
    drawConfirm();
    openLock();
    if (sound) confirmSound();
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
    noTone(ZOOMER);
    tone(ZOOMER, 220, 100);
    improvedDelay(200);
    noTone(ZOOMER);
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
    noTone(ZOOMER);
    tone(ZOOMER, 320, 100);
    improvedDelay(200);
    noTone(ZOOMER);
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