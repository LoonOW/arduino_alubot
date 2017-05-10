#include <IRremote.h>

#include <Servo.h>

Servo droite;
Servo gauche;

const int TrigPin = 4; 
const int EchoPin = 5;
float cm;


int RECV_PIN = 11;
int onoff = 0; //arret = 0, marche avant = 1, marche arriere = -1
const unsigned long T2 = 16718055; // FF18E7; // touche 2 = marche avant
const unsigned long T5 = 16726215; // FF38C7; // touche 5 = arret
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Initialise le recepteur
  droite.attach(2);
  gauche.attach(3);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    switch(results.value) {
      case T2:
        onoff = 1;
        break;
      case T5:
        onoff = 0;
        break;

    }
    irrecv.resume(); // Recoit la valeur suivante
  }

  delay(20);

  digitalWrite(TrigPin, LOW); //Low high and low level to send a short pulse to TrigPin
  delayMicroseconds(2); 
  digitalWrite(TrigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TrigPin, LOW); 
  
  cm = pulseIn(EchoPin, HIGH) / 58.0; //The echo time is converted into cm
  cm = (int(cm * 100.0)) / 100.0; //Keep two decimal places
  Serial.print(cm); 
  Serial.print("cm"); 
  Serial.println();

  switch(onoff) {
    case 1:
      if (cm > 20.00) {
        // avance
        droite.write(180);
        gauche.write(0);
        Serial.print("avance");
        delay(500);
        }
      else {
        // recule puis demi-tour
        Serial.print("recule puis demi-tour"); 
        droite.write(0);
        gauche.write(180);

        delay(500);

        droite.write(0);
        gauche.write(0);

        delay(700);
      }
      break;
    default:
      droite.write(90);
      gauche.write(90);      
      break;
  }
}
