#include "Arduino.h"
#include "FonctionsMP3Hugo.h"

bool previousState = LOW;

void setup()
{
  setupMP3();
  pinMode(9,INPUT);
  myDFPlayer.volume(14);  //Set volume value. From 0 to 30
  //jouerActionSpecifique(IMMORTEL,LANCEMENT_ULTI);
}


unsigned long last;
void loop()
{
  bool lectureProut= digitalRead(9);
  if(previousState == 0 && lectureProut == 1)
  {
    if(myDFPlayer.readState() != 512)
    {
      myDFPlayer.stop();
    }
    else
    {
      myDFPlayer.playFolder(7,random(34));
    }
  }
  previousState = lectureProut;
  


  unsigned long tempsActuel = millis();
  Serial.println(tempsActuel - last);
  loopMP3();
  last = tempsActuel;
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'p') {
      myDFPlayer.pause();
    }
    if (command == 's') {
      myDFPlayer.start();
    }
  }
}