#include <Servo.h>//démarre la biblihotèque des servos

Servo droite;//initialise le servomoteur de droite
Servo gauche;//initialise le servomoteur de gauche
// Décrire cette fonction
void tdroite() {
  droite.write(83);
  gauche.write(110);
}

// Décrire cette fonction
void tgauche() {
  droite.write(75);
  gauche.write(97);
}

// Décrire cette fonction
void arret() {
  droite.write(90);
  gauche.write(90);
}

// Décrire cette fonction
void avancer() {
  droite.write(0);
  gauche.write(180);
}


void setup() {
  droite.attach(3);//déclare le pin sur lequel est branché le servomoteur de droite
  gauche.attach(2);//déclare le pin sur lequel est branché le servomoteur de gauche
  pinMode(6, INPUT);//déclare le pin sur lequel est branché le capteur de ligne de gauche
  pinMode(7, INPUT);//déclare le pin sur lequel est branché le capteur de ligne de droite
  Serial.begin(9600);//enclenche la console de débeugage

}

void loop() {
    if (digitalRead(6) && !digitalRead(7)) {//si seulement le capteur de droite est actif
      tdroite();
      Serial.println("droite");
    }
    if (!digitalRead(6) && digitalRead(7)) {//si seulement le capteur de gauche est actif
      tgauche();
      Serial.println("gauche");
    }
    if (!digitalRead(6) && !digitalRead(7)) {//si les deux capteurs sont actifs
      arret();
      Serial.println("arret");
    }
    if (digitalRead(6) && digitalRead(7)) {//si aucun des deux capteurs est actif
      avancer();
      Serial.println("avance");
    }

}
