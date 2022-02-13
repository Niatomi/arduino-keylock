#include <Keypad.h>

const byte ROWS = 4; //число строк у нашей клавиатуры
const byte COLS = 4; //число столбцов у нашей клавиатуры
char hexaKeys[ROWS][COLS] = {// здесь мы располагаем названия наших клавиш, как на клавиатуре,для удобства пользования
{'1','4','7','d'}, 
{'2','5','8','0'},
{'3','6','9','#'},
{'A','B','C','D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2}; //к каким выводам подключаем управление строками
byte colPins[COLS] = {9, 8, 7, 6}; //к каким выводам подключаем управление столбцами

//передаем все эти данные библиотеке:
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup(){
  Serial.begin(9600);//запускаем отладочный вывод в монитор порта

  pinMode(10, OUTPUT);

  digitalWrite(10, 0);

}

String key = "1234";
String enteringKey = "";

void loop(){
    char customKey = customKeypad.getKey();//записывем нажатый символ

    if ()
    
    if (customKey){//если что-то нажато
    enteringKey += customKey;
    }

    if (enteringKey == key) {
        digitalWrite(10, 1);
    } else {
        digitalWrite(10, 0);
    }
    Serial.println(enteringKey);
}