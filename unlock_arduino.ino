// Étape 1: Collez le code de base .ino (#include <Arduino.h> , void setup, void loop)

// Étape 2: Ajout des bibliothèques Arduino, Chrono, Bounce et MicroOsc
#include <Arduino.h>

#include <Bounce2.h>
// Étape 4: Instancier un Bounce2::Button nommé bouton
Bounce2::Button but0;

#include <MicroOscSlip.h>
// Étape 3: Initialiser MicroOsc
MicroOscSlip<128> monOsc(&Serial); 

#include <Chrono.h>
// Étape 5: Instancier Chrono
Chrono chronoPot;

//Étape 7:  Ajouter une fonction qui sera appelée lorsqu’un message OSC est reçu.
void traiterMessageOsc(MicroOscMessage & message) {
  // SI L’ADRESSE DU MESSAGE EST "/lumiere" ET QUE LE MESSAGE CONTIENT UN ARGUMENT ENTIER ("i") :
   if (message.checkOscAddressAndTypeTags("/lumiere", "i")) {  

      int valeur = message.nextAsInt();  // RÉCUPÉRER L’ENTIER ("i")

      digitalWrite(3, valeur); // ASSIGNER LA VALEUR A LA DEL
 } 
}


void setup() {
// Étape 3: Initialiser MicroOsc
  Serial.begin(115200);

// Étape 4: Initialiser le bouton
  but0.attach(2, INPUT_PULLUP);
  but0.setPressedState(LOW);

//Étape 6: Initialiser la DEL du bouton 
  pinMode(3,OUTPUT);

}

void loop() {

//Étape 7:  Appel à la fonction onOscMessageReceived qui va déclencher traiterMessageOsc si un message est reçu
  monOsc.onOscMessageReceived(traiterMessageOsc);

// Étape 4: Mettre à jour le bouton
  but0.update();

// Étape 4: Si le bouton est appuyé, envoyer le message OSC /valider 1
  if (but0.pressed()) { // Bouton vient d’être appuyés
    monOsc.sendInt("/valider", 1);
  }

// Étape 5: Mettre à jour le chronomètre à chaque 20 millisecondes avec un message /tourner via OSC
  if ( chronoPot.hasPassed(20)) { // SI LE CHRONO DÉPASSE 20 MILLISECONDES
  chronoPot.restart(); // REPARTIR LE CHRONO

  // LE 2 CI_BAS CORRESPOND À LA BROCHE ANALOGIQUE A2
  int valeur = analogRead(1); // LECTURE DE LA TENSION DE LA BROCHE A2 ENTRE 0 ET 1023

  monOsc.sendInt("/tourner", valeur); // ENVOYER LA VALEUR
}

}
