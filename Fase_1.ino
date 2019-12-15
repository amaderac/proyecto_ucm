#include <Servo.h>
Servo MiServo;  //Servo
const int controlPin = 2;
int pulsacion = 0;
int estado_boton;
int estado_boton_anterior;

void setup() {
  Serial.begin(9600); //Puerto serie a 9600 baudios
  MiServo.attach(9);  // Servo en el Pin 9
  MiServo.write(20); // Posición inicial
  pinMode(controlPin, INPUT_PULLUP);
  estado_boton_anterior = digitalRead(controlPin);
}
void loop() {
          estado_boton = digitalRead(controlPin);
          if (estado_boton != estado_boton_anterior) {
          pulsacion++;
          if (pulsacion <= 1) {
          Serial.println("El puente se baja");
          MiServo.write(120);              
          delay(10000);
          MiServo.write(20);              
          delay(200);          
          }
          }
          estado_boton_anterior = LOW;
          pulsacion = 0;   
  }
