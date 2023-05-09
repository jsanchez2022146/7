#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Keypad.h>
#include <RTClib.h>
// Definición de constantes
const int servoPin = 11;
const int numRows = 4;
const int numCols = 4;
const byte rowPins[numRows] = {6, 7, 8, 9};
const byte colPins[numCols] = {2, 3, 4, 5}
char keypadKeys[numRows][numCols] = { 
{'1', '2', '3', 'A'},

{'4', '5', '6', 'B'},

{'7', '8', '9', 'C'},

{'*', '0', '#', 'D'},
};
const int lcdColumns = 16;
const int lcdRows = 2;
const int lcdAddr = 0x27;
RTC_DS1307 rtc;
// Variables globales
Servo servo;
LiquidCrystal_I2C lcd(lcdAddr, lcdColumns, lcdRows);
Keypad keypad = Keypad(makeKeymap(keypadKeys), rowPins, colPins, numRows, numCols);
int pin[4] = {1, 2, 3, 4};
int pinIndex = 0;
// Función para mostrar el mensaje en la pantalla LCD
void mostrarMensaje(String mensaje) {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(mensaje);
lcd.setCursor(0, 1);
lcd.print(rtc.now().toString("YYYY/MM/DD hh:mm:ss"));
}
// Función para mover el servo a la posición de cerrado
void cerrarPuerta() {
servo.write(0);
mostrarMensaje("Puerta cerrada");
}
// Función para mover el servo a la posición de abierto
void abrirPuerta() {
servo.write(90);
mostrarMensaje(" Puerta abierta");
}
// Función para verificar si el pin ingresado es correcto
void verificarPin() {
if (pinIndex == 4) {
// Se ingresó todo el pin, se verifica si es correcto
if (keypad.getState() == PRESSED && keypad.isPressed('#')) {
// Se ingresó la tecla de confirmación
int ingresado[4];
for (int i = 0; i < 4; i++) {
ingresado[i] = keypad.waitForKey() - '0';
lcd.print(ingresado[i]);
}
bool esCorrecto = true;
for (int i = 0; i < 4; i++) {
if (ingresado[i] != pin[i]) {
 esCorrecto = false;
}
}
if (esCorrecto) {
abrirPuerta();
} else {
}
delay(2000);
mostrarMensaje("Puerta cerrada");
pinIndex = 0;
}
}
}
void setup() {
// Inicialización de módulos
Wire.begin();
lcd.init();
lcd.backlight();
servo.attach(servoPin);
rtc.begin();
mostrarMensaje("Iniciando");
if (!rtc.isrunning()) {
mostrarMensaje("Error: RTC no encontrado");
while (true);
}
cerrarPuerta();
}
void loop() {
// Leer la tecla presionada en el teclado matricial
char key = keypad.getKey();
if (key != NO_KEY) {
// Se presionó una tecla, se muestra en la pantalla LCD
lcd.print(key);
// Se verifica si es un número para construir el pin
if (key >= '0' && key <= '9') {
pin[pinIndex] = key - '0';
pinIndex++;
verificarPin();
} else {
pinIndex = 0;
mostrarMensaje("");
}
}
}