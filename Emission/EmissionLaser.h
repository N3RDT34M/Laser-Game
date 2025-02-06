//AUTEUR : Hugo Faveyrial
//En gros:
//    Mettre void LOOPEmissionLaser() dans la loop principale
//    Mettre SETUPEmissionLaser(bool sequenceJoueur[4]) dans le setup principal (sequenceJoueur[4] correspond à une séquence de 4 bits qui code le joueur qui possède le laser)
//    void gachettePresse(bool presse) permet de communiquer au laser si il doit tirer ou non

const int PORTLedTest = 12; //PIN Arduino
bool etatLED = false; //cette variable vaut False = la LED est éteint
unsigned long dernierTemps;
const int tailleSequence = 14;
bool sequenceAEmettre[tailleSequence];
int indexSequence = 0;
bool etatGachette = false; //Si false, la gachette est relachée (donc on tire pas).

//Fonction setup, sequenceJoueur correspond à la séquence de 4 bits qui identifie le joueur
void SETUPEmissionLaser(bool sequenceJoueur[4]) {
  pinMode(PORTLedTest, OUTPUT);//Configuration du port Arduino comme une sortie
  digitalWrite(PORTLedTest,LOW);//Au début, on éteint la LED
  dernierTemps = micros();
  //On a définit que 110 était une séquence de contrôle
  sequenceAEmettre[0]=true;
  sequenceAEmettre[1]=true;
  sequenceAEmettre[2]=false;
  sequenceAEmettre[3] = sequenceJoueur[0];
  sequenceAEmettre[4] = sequenceJoueur[1];
  sequenceAEmettre[5] = sequenceJoueur[2];
  sequenceAEmettre[6] = sequenceJoueur[3];
  sequenceAEmettre[7]=true;
  sequenceAEmettre[8]=true;
  sequenceAEmettre[9]=false;
  sequenceAEmettre[10] = sequenceJoueur[0];
  sequenceAEmettre[11] = sequenceJoueur[1];
  sequenceAEmettre[12] = sequenceJoueur[2];
  sequenceAEmettre[13] = sequenceJoueur[3];
}

const int tempsLEDON = 5000;
//Cette fonction allume ou éteinds le laser.
//Paramètres:
  //temp, si temp vaut 10000 (microsecondes), ca veut dire que toutes les 10ms, le laser va s'allumer, puis s'éteindre
  //tempsLEDON, le temps que la LED reste allumé avant de s'éteindre à nouveau (en microsecondes)
  //tailleSequence, le nombre de BIT à emettre dans la séquence
  //sequenceAEmettre, la sequence de BIT a envoyer
void switchLEDAfter(unsigned long temp){
  //Mesure du timestamp actuel en microsecondes
  unsigned long tempsActuelEmission = micros();
  //Si actuellement la LED est ON, ou qu'elle devrait être ON mais que la séquence demande d'envoyer un bit "false", donc elle reste eteinte
  if(etatLED)
  {
    //Si ça fait tempsLEDON microsec qu'on est en train d'allumer la LED, alors on l'éteinds
    if(tempsActuelEmission - dernierTemps >= tempsLEDON)
    {
      digitalWrite(PORTLedTest,LOW);
      etatLED=false;
    }
  }
  //La LED est actuellement éteinte
  else
  {
    //On attend jusqu'à atteindre le paramètre temp pour éventuellement rallumer la LED (en fonction de la séquence)
    if(tempsActuelEmission - dernierTemps >= temp)
    {
      //On avance d'un cran dans la lecture de la séquence
      indexSequence = indexSequence + 1;
      //Si jamais on a atteind la fin de la séquence, on revient au début 
      if(indexSequence >= tailleSequence)
      {
        indexSequence = 0;
      }
      //On allume la LED si le bit à émettre est "true"
      if(sequenceAEmettre[indexSequence])
      {
        if(etatGachette)
        {
          digitalWrite(PORTLedTest,HIGH);
        }
        
      }
      //On remet les paramètres correctement pour que la boucle continue 
      etatLED = true;
      dernierTemps = tempsActuelEmission;
    }
  }
}

void LOOPEmissionLaser() {
  switchLEDAfter(10000);
}

//Changer la connaissance de l'état de la gachette par la classe d'emission de laser
void gachettePresse(bool presse)
{
  etatGachette = presse;
  if(presse)
  {
    startAtNextReception = true;
  }
}



