// https://openclassrooms.com/courses/perfectionnez-vous-dans-la-programmation-arduino/faites-reagir-votre-robot-avec-des-capteurs

#include <IRremote.h>
#include <Servo.h>

Servo droite;
Servo gauche;

const int PROCHE = 30;
const int DANGER = 15;
const int AFOND = 200;
const int PRUDENT = 50;
const int NBMESURE = 5;
const int VSON = 58.0;
// Correction à effectuer sur les mesures ultrason à cause d'une imcompatibilité entre le recepteur télécommande et le capteur ultrason ?
const float CORRECTION_ULTRASON_IREMOTE = 1.3;

// constantes de calibrage d'une diminution de vitesse des servo
const int DIMINUDROITE = 15;
const int DIMINUGAUCHE = 18;

// pins numériques utilisés pour le capteur ultrasons.
const int TrigPin = 4; 
const int EchoPin = 5;

// interrupteur via telecommande
int onoff = 0; //arret = 0, marche avant = 1, marche arriere = -1

// gestion de la telecommande
const int TLC_PIN = 11; // telecommande
const unsigned long T2 = 16718055; // FF18E7; // touche 2 = marche avant
const unsigned long T5 = 16726215; // FF38C7; // touche 5 = arret
IRrecv irrecv(TLC_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Initialise le recepteur TLC
  // initialisation capteur ultrason
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  // intialisation des 2 servos
  droite.attach(2);
  gauche.attach(3);

  //avance(AFOND); //on démarre le robot
}

void loop() {

  lire_telecommande();
  
  switch(onoff) {
    case 1:
      switch (mesure()){ //utilisation de la condition switch
        case 0: //l'obstacle est dans la zone DANGER
          Serial.println("*DANGER*");
          arret(); // on arrête le robot
          delay(200);
          recule(PRUDENT); // on le fait reculer un peu
          delay(800);
          recherche(); // on recherche la bonne voie
          break;
        case 1: //l'obstacle est dans la zone PROCHE
          Serial.println("Attention...");
          avance(PRUDENT); // on ralentit la vitesse
          break;
        default:
          avance(AFOND);
          break;
      }
      break;
    default:
      arret();
      delay(500);
      break;
  }
}

//fonctions
void lire_telecommande() {
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
}

int mesure(){
  //fonction qui mesure une distance avec le capteur
  unsigned long mesure = 0; // variable de mesure
  unsigned long cumul = 0; //variable pour la moyenne
  for (int t = 0; t < NBMESURE; t++) { // boucle pour effectuer les mesures
    digitalWrite(TrigPin, LOW); //Low high and low level to send a short pulse to TrigPin
    delayMicroseconds(2); 
    digitalWrite(TrigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(TrigPin, LOW); 
    mesure = pulseIn(EchoPin, HIGH)*CORRECTION_ULTRASON_IREMOTE; // fonction pulseIn qui attend un état haut et renvoie le temps d'attente
    cumul+=mesure; //on cumule les mesures
    Serial.println(mesure/VSON);
    delay(50); //attente obligatoire entre deux mesures
  }
  mesure=cumul/NBMESURE; //on calcule la moyenne des mesures
  mesure=mesure/VSON;//on transforme en cm

  Serial.print(mesure);
  Serial.println(" cm");

  if (mesure<=DANGER){//on teste si l'obstacle est dans la zone DANGER
    return 0; //si oui, on retourne le code de danger
  }
  else if (mesure>DANGER && mesure<=PROCHE){//on teste s'il est dans la zone PROCHE
    return 1; //si oui, on retourne le code de proche
  }
  return 2; // on retourne le code de sans risque
}

void recherche(){
  //fonction pour rechercher un passage
  tourneGauche(AFOND); //on positionne le robot à gauche (facultatif)
  delay (300);
  int etat=0; //variable de test de possibilité d'avancer
  etat=mesure(); // on effectue la mesure
  while (etat!=2){ // tant que la distance n'est pas suffisante
    etat=mesure(); // on effectue la mesure
    tourneDroite(PRUDENT); //on fait tourner le robot
  }
  Serial.println("La voie est libre !");
  //retour au programme principal
}

void arret(){
  Serial.println("Je stoppe");
  //fonction d'arrêt des moteurs
  droite.write(90);
  gauche.write(90);
}

void avance(int v){
  Serial.print("J'avance");
  affiche(v);
  //fonctin de mise en route des deux moteurs dans le sens avant
  // on utilise la variable v pour le pilotage PWM
  switch(v) {
    case AFOND:
      droite.write(180);
      gauche.write(0);
      break;
    case PRUDENT:
      droite.write(90+DIMINUDROITE);
      gauche.write(90-DIMINUGAUCHE);
      break;
    default: //arret
      arret();
      break;
  }  
}

void recule(int v){
  Serial.print("Je recule");
  affiche(v);
  //fonctin de mise en route des deux moteurs dans le sens arrière
  // on utilise la variable v pour le pilotage PWM
  // on utilise la variable v pour le pilotage PWM
  switch(v) {
    case AFOND:
      droite.write(0);
      gauche.write(180);
      break;
    case PRUDENT:
      droite.write(90-DIMINUDROITE);
      gauche.write(90+DIMINUGAUCHE);
      break;
    default: //arret
      arret();
  }  
}

void tourneDroite(int v){
  Serial.print("Je tourne a droite");
  affiche(v);
  //fonction de rotation à droite : un moteur dans un sens, l'autre dans le sens opposé
  // on utilise la variable v pour le pilotage PWM
  switch(v) {
    case AFOND:
      droite.write(180);
      gauche.write(180);
      break;
    case PRUDENT:
      droite.write(90+DIMINUDROITE);
      gauche.write(90+DIMINUGAUCHE);
      break;
  }
}

void tourneGauche(int v){
  Serial.print("Je tourne a gauche");
  affiche(v);
  //fonction de rotation à gauche : un moteur dans un sens, l'autre dans le sens opposé
  // on utilise la variable v pour le pilotage PWM
  switch(v) {
    case AFOND:
      droite.write(0);
      gauche.write(0);
      break;
    case PRUDENT:
      droite.write(90-DIMINUDROITE);
      gauche.write(90+DIMINUGAUCHE);
      break;
  }
}

void affiche(int v){
  //fonction complémentaire d'affichage
  if (v==AFOND){
    Serial.println(" a fond !");
  }
  else{
    Serial.println(" prudemment...");
  }
}

