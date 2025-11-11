#include <ESP32Servo.h> // Necesaria para el control angular preciso del Servomotor
// --- Definición de Pines de E/S ---
const int pinPIR = 2; // Pin de Entrada (Sensor)
const int pinServo = 13; // Pin de Salida (Actuador de Movimiento)
const int pinLED = 5; // Pin de Salida (Actuador de Luz/Indicador)
// --- Variables y Objetos del Comedero ---
Servo dispensador; // Crea un objeto llamado "dispensador"
int anguloDispensa = 140; // POSICIÓN 1: Ángulo al que se mueve para que CAIGA el alimento
int anguloCerrado = 40; // POSICIÓN 2: Ángulo de reposo (Tapa CERRADA)
// Bandera lógica: Es crucial para evitar que el servo gire sin parar
bool dispensado = false; // Inicialmente, no se ha servido comida (estado 'false')
void setup() {
 Serial.begin(115200); // Inicia el Monitor Serial para ver mensajes de estado

 // Configuración de Pines

 pinMode(pinPIR, INPUT); // El PIR es una ENTRADA que lee movimiento
 pinMode(pinLED, OUTPUT); // El LED es una SALIDA

 // Configuración del Servomotor
 dispensador.attach(pinServo); // Asocia el objeto 'dispensador' al pin13
 dispensador.write(anguloCerrado); // Coloca el servo en la posición inicial (CERRADO)
 digitalWrite(pinLED, LOW); // Asegura que el LED esté apagado

 Serial.println("Comedero Inteligente: Modo EN ESPERA...");
}
void loop() {
 // 1. Lee el estado del sensor PIR
 int hayGato = digitalRead(pinPIR);

 // Lógica: Solo actúa si hayGato es ALTO Y dispensado es FALSO
 if (hayGato == HIGH) {

 if (dispensado == false) {
 // --- INICIO DE LA SECUENCIA DE DISPENSACIÓN ---
 Serial.println(">>> ¡Gato detectado! Dispensando alimento...");

 // Actuador 1: Mueve el servo (la tapa) para servir la comida
 dispensador.write(anguloDispensa);

 // Actuador 2: Enciende el LED para indicar que está sirviendo
 digitalWrite(pinLED, HIGH);

 delay(1500); // Espera 1.5 segundos para que todo el alimento caiga

 // Actuador 1: Cierra el dispensador para detener el flujo de comida
 dispensador.write(anguloCerrado);

 // Actuador 2: Apaga el LED
 digitalWrite(pinLED, LOW);

 // Establece la bandera a TRUE: ¡Comida Servida! (Esto previene servir de nuevo)
 dispensado = true;

 Serial.println("Dispensación completada. Servo cerrado.");

 // Espera 10 segundos antes de volver a verificar el estado para evitar re-disparos
 delay(10000);
 }

 } else {
 // Si NO se detecta movimiento...
 if (dispensado == true) {
 // Si el gato se fue (ya no hay movimiento) Y ya había comido...

 // Reinicia la bandera a FALSE: El comedero está listo para la próxima vez
 dispensado = false;
 Serial.println("Sistema reiniciado. Listo para la próxima comida.");
 }
 delay(500); // Pausa breve para aligerar el procesador
 }
}
