#include "DFRobotDFPlayerMini.h"

#include <SoftwareSerial.h>
SoftwareSerial softSerial(10,11);
#define FPSerial softSerial

DFRobotDFPlayerMini myDFPlayer;

//Le prochain à lire est stocké à l'élément 0
int stackFichier[5] = {-1,-1,-1,-1,-1};
int stackDossier[5] = {-1,-1,-1,-1,-1};

enum Personnage {
  IMMORTEL = 1,
  BOURRIN = 2,
  NINJA = 3,
  ECLAIREUR = 4,
  CYBER_GNOME = 5,
  TAUPE = 6
};

enum ActionSpecifique {
  DEBUT_PARTIE = 1,
  LANCEMENT_ULTI = 2,
  IMPOSSIBLE_ULTI = 3,
  DESACTIVATION_ARME = 4,
  MORT = 5
};

int getNBSoundToPlayYetInQueue()
{
  for(int i = 0; i < 5; i++)
  {
    if(stackFichier[i] == -1)
    {
      return i;
    }
  }
  return 5;
}

void ajouterAuStack(int noDossier, int noMusique){
  int indexRetenu = 6;
  //on cherche le plus petit index ou s'insérer 
  for(int i = 0; i < 5; i++)
  {
    if(stackFichier[i]==-1)
    {
      indexRetenu = i;
      break;
    }
  }
  if(indexRetenu == 6)
  {
    Serial.println("LA STACK EST PLEINE, LE SON NE SERA PAS JOUE");
    return;
  }
  stackDossier[indexRetenu] = noDossier;
  stackFichier[indexRetenu] = noMusique;
}

//Si skipQueue vaut true, on lit le son instantanément (on coupe même le son actuellement en lecture)
void jouerActionSpecifique(int personnage, int action, bool skipQueue = false)
{
  if(personnage == IMMORTEL)
  {
    personnage = 1;
  }
  else if(personnage == BOURRIN)
  {
    personnage = 2;
  }
  else if(personnage == NINJA)
  {
    personnage = 3;
  }
  else if(personnage == ECLAIREUR)
  {
    personnage = 4;
  }
  else if(personnage == CYBER_GNOME)
  {
    personnage = 5;
  }
  else if(personnage == TAUPE)
  {
    personnage = 6;
  }
  else if(personnage > 6)
  {
    Serial.println("ERREUR, PERSONNAGE INCONNU, J AI DONC CHOISI DE NE JOUER AUCUN SON");
    return;
  }
  if(action == DEBUT_PARTIE)
  {
    action = 1;
  }
  else if(action == LANCEMENT_ULTI)
  {
    action = 2;
  }
  else if(action == IMPOSSIBLE_ULTI)
  {
    action = 3;
  }
  else if(action == DESACTIVATION_ARME)
  {
    action = 4;
  }
  else if(action == MORT)
  {
    action = 5;
  }
  else if(action > 5)
  {
    Serial.println("ERREUR, ACTION INCONNUE, J AI DONC CHOISI DE NE JOUER AUCUN SON");
    return;
  }
  if(skipQueue)
  {
    myDFPlayer.playFolder(personnage,action);
  }
  else
  {
    ajouterAuStack(personnage,action);
  }
}

//Si skipQueue vaut true, on lit le son instantanément (on coupe même le son actuellement en lecture)
void jouerLireImmortelBouclierBrise(bool skipQueue)
{
  if(skipQueue)
  {
    myDFPlayer.playFolder(1,6);
  }
  else
  {
    ajouterAuStack(1,6);
  }
}

//Si skipQueue vaut true, on lit le son instantanément (on coupe même le son actuellement en lecture)
void jouerEclaireurImpossibleCarEspion(bool skipQueue = false)
{
  if(skipQueue)
  {
    myDFPlayer.playFolder(3,6);
  }
  else
  {
    ajouterAuStack(3,6);
  }
}

void jouerPreventionActivationUltiDe(int Personnage, bool skipQueue = false)
{
  if(Personnage > 7)
  {
    Serial.println("Je n'ai pas pu lire de son car tu appel un personnage bizarre");
  }
  if(Personnage == 1)
  {
    Personnage = 5;
  }
  else if (Personnage ==2)
  {
    Personnage = 6;
  }
  else if (Personnage == 3)
  {
    Personnage = 2;
  }
  else if (Personnage == 4)
  {
    Personnage = 3;
  }
  else if (Personnage == 5)
  {
    Personnage = 4;
  }
  else if (Personnage == 6)
  {
    Personnage = 1;
  }
  if(skipQueue)
  {
    myDFPlayer.playFolder(7,Personnage);
  }
  else
  {
    ajouterAuStack(7,Personnage);
  }
}

void jouerPiouPiou(bool skipQueue = false){
  if(skipQueue)
  {
    myDFPlayer.playFolder(7,7);
  }
  else
  {
    ajouterAuStack(7,7);
  }
}

void jouerSurchauffe(int niveauSurchauffe1ou2, bool skipQueue = false)
{
  if(niveauSurchauffe1ou2 != 1 && niveauSurchauffe1ou2 != 2)
  {
    Serial.println("Tu me demandes de jouer un son qui n'existe pas");
    return;
  }
  if(skipQueue)
  {
    myDFPlayer.playFolder(7,niveauSurchauffe1ou2+7);
  }
  else
  {
    ajouterAuStack(7,niveauSurchauffe1ou2+7);
  }
}

void jouerEnnemiLegendaire(bool skipQueue = false)
{
  if(skipQueue)
  {
    myDFPlayer.playFolder(7,10);
  }
  else
  {
    ajouterAuStack(7,10);
  }
}

void jouerAllieLegendaire(bool skipQueue = false)
{
  if(skipQueue)
  {
    myDFPlayer.playFolder(7,26);
  }
  else
  {
    ajouterAuStack(7,26);
  }
}

//Attention, pas de voiceline pour "il reste 5 secondes"
void jouer6secondesAvantDesactivation(int progressionEntre1et5, bool skipQueue = false)
{
  if(progressionEntre1et5 == 1)
  {
    //6 secondes avant désactivation de l'arme
    progressionEntre1et5 = 11;
  }
  else if(progressionEntre1et5 == 2)
  {
    //4
    progressionEntre1et5 = 12;
  }
  else if(progressionEntre1et5 == 3)
  {
    //3
    progressionEntre1et5 = 13;
  }
  else if(progressionEntre1et5 == 4)
  {
    //2
    progressionEntre1et5 = 14;
  }
  else if(progressionEntre1et5 == 5)
  {
    //1
    progressionEntre1et5 = 15;
  }
  else
  {
    Serial.println("Je ne peux pas lire ce son");
    return;
  }
  if(skipQueue)
  {
    myDFPlayer.playFolder(7,progressionEntre1et5);
  }
  else
  {
    ajouterAuStack(7,progressionEntre1et5);
  }
}

void jouerRepereParEclaireur(bool skipQueue = false)
{
  if(skipQueue)
  {
    myDFPlayer.playFolder(7,16);
  }
  else
  {
    ajouterAuStack(7,16);
  }
}

void jouerSonMort(bool skipQueue = false)
{
  if(skipQueue)
  {
    myDFPlayer.playFolder(7,24);
  }
  else
  {
    ajouterAuStack(7,24);
  }
}

void jouerSonRespawn(bool skipQueue = false)
{
  if(skipQueue)
  {
    myDFPlayer.playFolder(7,25);
  }
  else
  {
    ajouterAuStack(7,25);
  }
}

void jouerEnnemiABouclier(bool skipQueue = false)
{
  if(skipQueue)
  {
    myDFPlayer.playFolder(7,23);
  }
  else
  {
    ajouterAuStack(7,23);
  }
}

void jouerBouclierDesactive(bool skipQueue = false)
{

}

void setupMP3()
{
  //Changer la vitesse de communication 
  byte setBaudCommand[] = {
    0x7E, 0xFF, 0x06, 0x02, 0x00, 0x01, 0x00, 0x00, 0xEF  // Commande pour 19200 baud (change 0x01 pour d'autres vitesses)
  };
  FPSerial.begin(4800);
  
  delay(100);

  Serial.begin(4800);

  Serial.println();
  Serial.println("DFRobot DFPlayer Mini Demo");
  Serial.println("Initializing DFPlayer ... (May take 3~5 seconds)");

  if (!myDFPlayer.begin(FPSerial,true,true)) {  //Use serial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0);
    }
  }
}

void loopMP3()
{
  unsigned long temps1 = millis();
  //int state = myDFPlayer.readState();
  //myDFPlayer.sendStack(0x42);
  unsigned long temps2 = millis();
  Serial.print("Variable 1 : ");
  Serial.println(temps2-temps1);
  /*if (state == 512) {
    if(stackFichier[0] != -1 && stackDossier[0] != -1)
    {
      //Lire le fichier son suivant dans la queue
      myDFPlayer.playFolder(stackDossier[0],stackFichier[0]);
      //Faire glisser la stack vers 0
      stackFichier[0] = stackFichier[1];
      stackFichier[1] = stackFichier[2];
      stackFichier[2] = stackFichier[3];
      stackFichier[3] = stackFichier[4];
      stackFichier[4] = -1;
      stackDossier[0] = stackDossier[1];
      stackDossier[1] = stackDossier[2];
      stackDossier[2] = stackDossier[3];
      stackDossier[3] = stackDossier[4];
      stackDossier[4] = -1;
    }
  }*/
}