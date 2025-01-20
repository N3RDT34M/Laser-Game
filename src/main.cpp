#include <Arduino.h>

// Définition des pins pour la LED RGB
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

// Structure pour les couleurs RGB
struct RGBColor {
    int red;
    int green;
    int blue;
};

// Variables globales
RGBColor storedColor = {255, 0, 0}; // Couleur par défaut (rouge)
bool isLedOn = true; // LED allumée par défaut

// Initialisation de la LED
void initRGBLed() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

// Fonction pour changer la couleur
void setRGBColor(RGBColor color) {
    storedColor = color; // Sauvegarde la nouvelle couleur
    if (isLedOn) {
        // Applique la couleur directement aux pins
        analogWrite(RED_PIN, color.red);
        analogWrite(GREEN_PIN, color.green);
        analogWrite(BLUE_PIN, color.blue);
    }
}

// Éteindre la LED
void turnOffRGB() {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
}

// Fonction pour allumer/éteindre la LED
void switchOnOff() {
    isLedOn = !isLedOn;
    if (isLedOn) {
        // Allume la LED avec la dernière couleur stockée
        setRGBColor(storedColor);
    } else {
        // Éteint la LED
        turnOffRGB();
    }
}

void setup() {
    initRGBLed();
    Serial.begin(9600);
    // Allume la LED avec la couleur par défaut au démarrage
    setRGBColor(storedColor);
}

void loop() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        switch(cmd) {
            case 's':
                switchOnOff(); // Bascule ON/OFF
                break;
            case 'r':
                setRGBColor({255, 0, 0}); // Rouge
                break;
            case 'g':
                setRGBColor({0, 255, 0}); // Vert
                break;
            case 'b':
                setRGBColor({0, 0, 255}); // Bleu
                break;
        }
    }
}