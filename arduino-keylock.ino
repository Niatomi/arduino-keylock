#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define GREEN 13
#define RED 12
#define ZOOMER 2

const byte ROWS = 3; //число строк у нашей клавиатуры
const byte COLS = 4; //число столбцов у нашей клавиатуры
char hexaKeys[ROWS][COLS] = {// здесь мы располагаем названия наших клавиш, как на клавиатуре,для удобства пользования
{'1','2','3','A'}, 
{'4','5','6','0'},
{'7','8','9','C'}
};

byte rowPins[ROWS] = {5, 4, 3}; //к каким выводам подключаем управление строками
byte colPins[COLS] = {9, 8, 7, 6}; //к каким выводам подключаем управление столбцами

//передаем все эти данные библиотеке:
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd (0x27, 20, 4);

String key = "1234";
String enteringPassword = "";

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



void loop(){
}

void tryToOpen() {

    openUsingKeypad();

}
String pass = "";
void openUsingKeypad() {
    lcd.clear();
    drawCodeHeader();
    boolean enteryPoint = true;
    while (enteryPoint) {
        char customKey = customKeypad.getKey();
        if (customKey) {
            if (customKey == 'A') {
                enteryPoint = !enteryPoint;
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

void drawPassword() {
    lcd.clear();
    drawCodeHeader();
    lcd.setCursor(0, 1);
    lcd.print(enteringPassword);
}

void drawCodeHeader() {
    lcd.setCursor(0, 0);
    lcd.print("Enter your password:");
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