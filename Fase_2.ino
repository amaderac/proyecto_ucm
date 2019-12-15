//Librerías a utilizar
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
 
#define SS_PIN 10 //RFID
#define RST_PIN 9 //RFID
#define LED_Verde 4 //LED Verde
#define LED_Rojoojo 5 //LED Rojo
#define ZUMBADOR 7 //Zumbador

MFRC522 LECTOR(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo MiServo; //define servo name

const int controlPin = 2;

int pulsacion = 0;
int estado_boton;
int estado_boton_anterior;
 
void setup() 
{
  Serial.begin(9600);   // Puerto serie a 9600 baudios
  SPI.begin();      // Bus SPI
  LECTOR.PCD_Init();   // Lector RFID MFRC522 
  MiServo.attach(3); // Servo
  MiServo.write(20); // Servo posicion inicial
  pinMode(LED_Verde, OUTPUT);
  pinMode(LED_Rojo, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(controlPin, INPUT_PULLUP);
  noTone(BUZZER);
  estado_boton_anterior = digitalRead(controlPin);
  Serial.println("Esperando tarjeta...");
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
  tone(BUZZER, 500);
  delay(300);
  noTone(BUZZER);
  MiServo.write(120); // El servo baja
  delay(10000);
  MiServo.write(20); // El servo sube
  digitalWrite(LED_Verde, LOW); // El LED Verde se apaga
  delay(200);          
  }
  }
  estado_boton_anterior = LOW;
  pulsacion == 0;

  if ( ! LECTOR.PICC_IsNewCardPresent()) // Buscando tarjetas...
  {
    return;
  }
  if ( ! LECTOR.PICC_ReadCardSerial()) // Seleccionando tarjeta...
  {
    return;
  }
  Serial.print("UID de tarjeta:"); // Muestra el UID por el puerto serie
  String content= "";
  byte letter;
  for (byte i = 0; i < LECTOR.uid.size; i++) 
  {
     Serial.print(LECTOR.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(LECTOR.uid.uidByte[i], HEX);
     content.concat(String(LECTOR.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(LECTOR.uid.uidByte[i], HEX)); // Lo pasamos a Hexadecimal para mostrarlo
  }
  Serial.println();
  Serial.print("Tarjeta : ");
  content.toUpperCase();
  if (content.substring(1) == "92 2F E2 37") //Sólo se habilita una tarjeta
  {
    Serial.println("Autorizada. El puente se baja");
    Serial.println();
    delay(500);
    digitalWrite(LED_Verde, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    MiServo.write(120);
    delay(10000);
    MiServo.write(20);
    digitalWrite(LED_Verde, LOW);
  }
 
 else   {
    Serial.println("Denegada.");
    digitalWrite(LED_Rojo, HIGH);
    tone(BUZZER, 300);
    delay(1000);
    digitalWrite(LED_Rojo, LOW);
    noTone(BUZZER);
  }
}
