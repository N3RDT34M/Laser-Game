#include <Arduino.h> // Nécessaire pour les types et fonctions Arduino
#include "DetectionLumiere.h" // Inclusion du fichier contenant la réception du laser et le traitement de l'information reçue
#include "EmissionLaser.h" //Inclusion du fichier contenant l'émission d'un tir laser

bool BitsDuJoueur1[4] = {false, false, false, false};
unsigned long dernierTempsDetec2;





void setup() {
  Serial.begin(9600);
  
  SETUPDetectionLumiere();
  
  //SETUPEmissionLaser(BitsDuJoueur1);
  //gachettePresse(true);

  //onPenseQueTueurEst = 3;

}

void loop() {
  //LOOPEmissionLaser();
  LOOPDetectionLumiere();

  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if ((cmd >= '0' && cmd <= '9') || (cmd >= 'a' && cmd <= 'f')) {
      int valeur = (cmd >= 'a') ? cmd - 'a' + 10 : cmd - '0';
      Serial.println(valeur);

      // Vérifie que la valeur est inférieure à 16 et différente de 6, 12, 13, 14
      if (valeur < 16 && valeur != 6 && valeur != 12 && valeur != 13 && valeur != 14) {
        // Ton code ici
        Serial.println("Chiffre accepté !");
        Serial.print("Le nouveau joueur tireur est le joueur : ");
        Serial.println(valeur);
        bool toSend[4];
        if (valeur < 0 || valeur > 15) {
          Serial.println("Erreur: nombre hors plage");
          return;
        }

        for (int i = 0; i < 4; i++) {
          toSend[3 - i] = (valeur & (1 << i)) != 0;
        }
        SETUPEmissionLaser(toSend);
      }
    }
    if (cmd == 'o')
    {
      afficherCalibrage = !afficherCalibrage;
    }
    if (cmd == 'i')
    {
      nextAffichage = !nextAffichage;
    }  
  }
}





