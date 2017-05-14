#include <IRremote.h>

#include <Servo.h>

Servo droite; //On nomme le servomoteur droit du robot
Servo gauche; //On nomme le servomoteur gauche du robot

const int TrigPin = 4; //On initialise le pin triger du capteur à ultrason
const int EchoPin = 5; //On initialise le pin echo du capteur à ultrason
float cm; //On initialise la grandeur de mesure en centimètres


int RECV_PIN = 11; //On initialise le pin sur lequel est branché le récepteur de la télécommande
int onoff = 0; //arret = 0, marche avant = 1, marche arriere = -1
const unsigned long T2 = 16718055; // FF18E7; // touche 2 = marche avant
const unsigned long T5 = 16726215; // FF38C7; // touche 5 = arret
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600); //On initialise la console série sur le port 9600
  irrecv.enableIRIn(); // Initialise le recepteur
  droite.attach(2); //Détermine le pin sur lequel est branché le servomoteur de droite
  gauche.attach(3); //Détermine le pin sur lequel est branché le servomoteur de gauche
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX); //Affiche la valeur reçu par le récepteur de la télécommande
    switch(results.value) {
      case T2:
        onoff = 1; //Met onoff à 1 si la télécommande envoie la touche 2
        break;
      case T5:
        onoff = 0; //Met onoff à 0 si la télécommande envoie la touche 5
        break;

    }
    irrecv.resume(); // Recoit la valeur suivante
  }

  delay(20);

  digitalWrite(TrigPin, LOW); //Envoi une pulsion électrique au capteur à ultrason
  delayMicroseconds(2); 
  digitalWrite(TrigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TrigPin, LOW); 
  
  cm = pulseIn(EchoPin, HIGH) / 58.0; //La valeur reçu du capteur à ultrason est convertie en cm
  cm = (int(cm * 100.0)) / 100.0;
  Serial.print(cm); //Affiche la distance captée par le capteur à ultrason sur la console série
  Serial.print("cm");  //Affiche le mot centimètre sur la console série
  Serial.println();

  switch(onoff) {
    case 1:
      if (cm > 20.00) { //Capte si la distance est inférieure à 20 cm
        // avance
        droite.write(180);
        gauche.write(0);
        Serial.print("avance");
        delay(500); //Pause de 0.5 secondes
        }
      else {
        // recule puis demi-tour
        Serial.print("recule puis demi-tour"); 
        droite.write(0);
        gauche.write(180);

        delay(500); //Pause de 0.5 secondes
        //Demi-tour
        droite.write(0);
        gauche.write(0);

        delay(700); //Pause de 0.7 secondes
      }
      break;
    default:
      droite.write(90);
      gauche.write(90);      
      break;
  }
}
