



//Programme chargé de détecter quel joueur vous tue, en gros
//    Mettre void LOOPDetectionLumiere() dans la loop principale
//    Mettre void SETUPDetectionLumiere() dans la loop principale
//    EVENTKilledBy(int IDTueur) est appellée à chaque fois qu'un joueur vous touche, IDTueur est l'id du joueur qui est à l'origine du tir. L'info est transmise par le laser. Vous pouvez rajouter du code dedans

//    Je ne pense pas que ca vous soit utile, mais si jamais EVENTFinReceptionLumiere() EVENTDebutReceptionLumiere() sont appellés quand l'état du capteur passe respectivement de LOW a HIGH et de HIGH a LOW



bool afficherCalibrage = false;
int onPenseQueTueurEst = 0;
bool nextAffichage = false;

static bool dernierEtat = LOW; //Dernier état lu du capteur de lumière
unsigned long debutPartition = 0; //Pour stocker le timestamp ou on a commencé à lire la séquence, on commence à lire quand on recoit true
const int tailleSequenceLecture = 14;
bool partitionLue[tailleSequenceLecture];

const int entreeDetectionLumiere = A0; // Port du capteur de lumière
int seuilConsiderationReception = 70;
void calibrerSeuilLumiere(int nouveauSeuil)//debug
{
  seuilConsiderationReception = nouveauSeuil;
}
int nbDetection = 0; //Debug, nombre de ON/OFF par seconde
bool startAtNextReception = false;
bool lectureSequence = false;

int goodgood = 0;//debug
int badbad = 0;//debug
int verybad = 0;

//Event appelé quand un joueur vous a touché, IDTueur est l'ID du joueur à l'origine du tir (transmis par le laser)
void EVENTKilledBy(int IDTueur)
{
  Serial.print("J ai été tué par le joueur ");
  Serial.println(IDTueur);
}

//Fonction utilisée par la loop mais elle ne sert à rien pour vous
int atEndOfReadingSequence()
{
  if(nextAffichage)
  {
    for(int i=0;i<=tailleSequenceLecture;i++)
    {
      Serial.print(partitionLue[i]);
      Serial.print(" ");
    }
    Serial.println("");
  }
  //Identifier dans la séquence de 10 bits la position du premier 1 de la séquence 110 
  int indexPremier1 = 14; // Par défault, réglé sur une erreur 
  for(int i=0; i <= 5; i++)
  {
    if(partitionLue[i] && partitionLue[i+1] && (!partitionLue[i+2]))
    {
      indexPremier1 = i;
      break;
    }
  }
  if(indexPremier1 == 14)
  {
    //On a lu une séquence de merde 
    return 6;
  }
  for(int i = 0; i < 4; i++)
  {
    int firstIndex = indexPremier1 + 3 + i;
    if(firstIndex >= tailleSequenceLecture)
    {
      firstIndex -= tailleSequenceLecture;
    }
    int deuxiemeIndex = indexPremier1 + 10 + i;
    if(deuxiemeIndex>= tailleSequenceLecture)
    {
      deuxiemeIndex -= tailleSequenceLecture;
    }

    //On compare que les 2 sequence soit identiques
    if(partitionLue[firstIndex] != partitionLue[deuxiemeIndex])
    {
      //Un des bits n'est pas identiques dans les 2 sequence donc on retourne une erreur
      return 6;
    }
  }
  bool toReturn[4];
  for(int i = 0; i < 4; i++)
  {
    int vraiIndex = indexPremier1 + 3 + i;
    if(vraiIndex >= tailleSequenceLecture)
    {
      vraiIndex -= tailleSequenceLecture;
    }
    toReturn[i] = partitionLue[vraiIndex];
  }

  return toReturn[0] * 8  + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
  //Si on a pas déjà return 6, ca veut dire qu'on a 2 fois la même séquence, on peut donc return la séquence
}

/*switch(indexPremier1)
    {
      case 0:
        if(partitionLue[7] && partitionLue[8] && (!partitionLue[9]))
        {
          toReturn[0]=partitionLue[3];
          toReturn[1]=partitionLue[4];
          toReturn[2]=partitionLue[5];
          toReturn[3]=partitionLue[6];
          return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        }
        else return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        break;
      case 1:
        if((partitionLue[7]==partitionLue[0]) && partitionLue[8] && partitionLue[9])
        {
          toReturn[0]=partitionLue[4];
          toReturn[1]=partitionLue[5];
          toReturn[2]=partitionLue[6];
          toReturn[3]=partitionLue[7];
          return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        }
        else return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        break;
      case 2:
        if((partitionLue[8]==partitionLue[1]) && (partitionLue[0]==partitionLue[7]) && partitionLue[9])
        {
          toReturn[0]=partitionLue[5];
          toReturn[1]=partitionLue[6];
          toReturn[2]=partitionLue[7];
          toReturn[3]=partitionLue[8];
          return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        }
        else return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        break;
      case 3:
        if((partitionLue[8]==partitionLue[1]) && (partitionLue[0]==partitionLue[7]) && (partitionLue[2]==partitionLue[9]))
        {
          toReturn[0]=partitionLue[6];
          toReturn[1]=partitionLue[7];
          toReturn[2]=partitionLue[8];
          toReturn[3]=partitionLue[9];
          return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        }
        else return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        break;
      case 4:
        if((partitionLue[8]==partitionLue[1]) && (partitionLue[0]==partitionLue[7]) && (partitionLue[2]==partitionLue[9]))
        {
          toReturn[0]=partitionLue[0];
          toReturn[1]=partitionLue[1];
          toReturn[2]=partitionLue[2];
          toReturn[3]=partitionLue[3];
          return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        }
        else return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        break;
      case 6:
        if(partitionLue[0] && (!partitionLue[1]) && (partitionLue[2]==partitionLue[9]))
        {
          toReturn[0]=partitionLue[2];
          toReturn[1]=partitionLue[3];
          toReturn[2]=partitionLue[4];
          toReturn[3]=partitionLue[5];
          return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        }
        else return toReturn[0] * 8 + toReturn[1] * 4 + toReturn[2] * 2 + toReturn[3];
        break;
    }*/

//Fonction de setup
void SETUPDetectionLumiere(){
  pinMode(entreeDetectionLumiere, INPUT); // Configuration du pin 13 comme entrée
}

//EVENT Quand l'état du capteur vallait on et maintenant l'état vaut off
void EVENTFinReceptionLumiere(){
  //Serial.println("Je ne suis plus éclairé");
}
//EVENT Quand l'état du capteur vallait off et maintenant l'état vaut on
void EVENTDebutReceptionLumiere(){
  //Serial.println("Je viens de détecter de la lumière");
  nbDetection = nbDetection + 1;
  //Commencer la lecture d'une série de bits
  if(startAtNextReception)
  {
    lectureSequence = true;
    debutPartition = millis();
    for(int i = 0;i < tailleSequenceLecture; i++)
    {
      partitionLue[i] = false;
    }
    partitionLue[0] = true;
    startAtNextReception = false;
  }
}

// Fonction pour lire l'état du capteur de lumière
void LOOPDetectionLumiere() {
  unsigned long tempsActuel = millis();
  //bool etatActuel = digitalRead(entreeDetectionLumiere);
  bool etatActuel = (analogRead(A0) > seuilConsiderationReception);
  if(afficherCalibrage)
  {
    Serial.println(analogRead(A0));
  }
  // Actualisation des etats, si le capteur de lumiere n'a pas changé, sachez que c'est normal que HIGH et LOW paraissent inversés
  if (etatActuel == HIGH && dernierEtat == LOW) {
    EVENTFinReceptionLumiere();
  }
  else if (etatActuel == LOW && dernierEtat == HIGH){
    EVENTDebutReceptionLumiere();
  } 

  //Si on est en train de lire une sequence (normalement c'est a peu près tout le temps le cas)
  if(lectureSequence)
  {
    unsigned long tempsDepuisDebutSequence = tempsActuel - debutPartition;
    int indexCalculeDansPartition = tempsDepuisDebutSequence / 10;
    if(etatActuel == HIGH && dernierEtat == LOW)
    {
      partitionLue[indexCalculeDansPartition] = true;
    }
    if(indexCalculeDansPartition >= tailleSequenceLecture)
    {
      int noJoueurTueur = atEndOfReadingSequence();
      lectureSequence = false;
      startAtNextReception = true;//Faire en sorte que le gilet recommence à lire une prochaine séquence de 10 bits
      if(noJoueurTueur == 6 || noJoueurTueur == 14 || noJoueurTueur == 13 || noJoueurTueur == 12 )
      {
        verybad += 1;
      }
      else
      {
        if(noJoueurTueur == onPenseQueTueurEst)
        {
          goodgood += 1;
        }
        else
        {
          badbad += 1;
        }
        EVENTKilledBy(noJoueurTueur);
      }
    }
  }
  dernierEtat = etatActuel;
}

