//Librerías a utilizar
#include <SPI.h> 
#include <MFRC522.h>
#include <Servo.h>
 
#define SS_PIN 10 //RFID
#define RST_PIN 9 //RFID
#define LED_Verde 4 //LED Verde
#define LED_Rojo 5 //LED Rojo
#define ZUMBADOR 7 //Zumbador

MFRC522 LECTOR(SS_PIN, RST_PIN);   // Instancia del lector RFID
Servo SERVO_DER; //Servo derecho
Servo SERVO_IZQ; //Servo izquierda

const int controlPin = 2;

int pulsacion = 0;
int estado_boton;
int estado_boton_anterior;
 
void setup() 
{
  Serial.begin(9600);   //Puerto serie a 9600 baudios
  SPI.begin();      // Bus SPI
  LECTOR.PCD_Init();   // Lector RFID MFRC522 
  SERVO_DER.attach(3); //Servo Derecho
  SERVO_IZQ.attach(6); //Servo Izquierdo
  SERVO_DER.write(20); //Servo Derecho posicion inicial
  SERVO_IZQ.write(100); //Servo Izquierd posicion inicial
  pinMode(LED_Verde, OUTPUT);
  pinMode(LED_Rojo, OUTPUT);
  pinMode(ZUMBADOR, OUTPUT);
  pinMode(controlPin, INPUT_PULLUP);
  noTone(ZUMBADOR);
  estado_boton_anterior = digitalRead(controlPin);
  Serial.println("El paso no está habilitado");
  Serial.println();

}
void loop() 
{
  estado_boton = digitalRead(controlPin);
  if (estado_boton != estado_boton_anterior) {
  pulsacion++;
  if (pulsacion <= 1) {
  Serial.println("El puente se baja");
  digitalWrite(LED_Verde, HIGH); // El LED Verde se enciende
  tone(ZUMBADOR, 500);
  delay(300);
  noTone(ZUMBADOR);
  SERVO_DER.write(120); // El servo derecho baja
  SERVO_IZQ.write(10); // El servo izquierdo baja
  delay(10000);
  SERVO_DER.write(20); // El servo derecho sube
  SERVO_IZQ.write(100); // El servo izquierdo sube
  digitalWrite(LED_Verde, LOW); // El LED Verde se apaga
  delay(200);          
  Serial.println("El paso no está habilitado");
  }
  }
  estado_boton_anterior = LOW;
  pulsacion = 0;
  
  if ( ! LECTOR.PICC_IsNewCardPresent()) // Buscando tarjetas...
  {
    return;
  }
  if ( ! LECTOR.PICC_ReadCardSerial()) // Seleccionando tarjeta...
  {
    return;
  }
  Serial.print("UID de tarjeta:"); // Muestra el UID por el puerto serie
  String tarjeta= "";
  byte letter;
  for (byte i = 0; i < LECTOR.uid.size; i++) 
  {
     Serial.print(LECTOR.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(LECTOR.uid.uidByte[i], HEX);
     tarjeta.concat(String(LECTOR.uid.uidByte[i] < 0x10 ? " 0" : " "));
     tarjeta.concat(String(LECTOR.uid.uidByte[i], HEX)); // Lo pasamos a Hexadecimal para mostrarlo
  }
  Serial.println();
  Serial.print("Tarjeta : ");
  tarjeta.toUpperCase();
  if (tarjeta.substring(1) == ("92 2F E2 37") || tarjeta.substring(1) == ("E9 F1 DF 86") || tarjeta.substring(1) == ("64 14 DC AA")) // Añadidos los UID de las tarjetas con acceso a abrir el puente
  {
    Serial.println("Autorizada. El puente se baja");
    Serial.println();
    delay(500);
    digitalWrite(LED_Verde, HIGH);
    tone(ZUMBADOR, 500);
    delay(300);
    noTone(ZUMBADOR);
    SERVO_DER.write(120);
    SERVO_IZQ.write(10);
    delay(10000);
    SERVO_DER.write(20);
    SERVO_IZQ.write(100);
    digitalWrite(LED_Verde, LOW);
    delay(200);
    Serial.println("El paso no está habilitado"); 
  }
 
 else   {
    Serial.println("Denegada.");
    digitalWrite(LED_Rojo, HIGH);
    tone(ZUMBADOR, 300);
    delay(1000);
    digitalWrite(LED_Rojo, LOW);
    noTone(ZUMBADOR);
  }
} 
