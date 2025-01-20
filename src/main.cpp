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
RGBColor storedColor = {0, 255, 255};
bool isLedOn = true;

// Initialisation des pins RGB
void initRGBLed() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

// Fonction pour changer la couleur
void setRGBColor(RGBColor color) {
    storedColor = color;
    if (isLedOn) {
        // Inverse les valeurs pour anode commune (0=allumé, 255=éteint)
        analogWrite(RED_PIN, 255 - color.red);
        analogWrite(GREEN_PIN, 255 - color.green);
        analogWrite(BLUE_PIN, 255 - color.blue);
    }
}

// Éteindre la LED
void turnOffRGB() {
    analogWrite(RED_PIN, 255); // HIGH pour éteindre en anode commune
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 255);
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
    initRGBLed();  // Configure les pins en OUTPUT
    Serial.begin(9600);
    setRGBColor(storedColor);
}

void loop() {
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        switch(cmd) {
            case 's':
                switchOnOff();
                break;
            case 'r':
                setRGBColor({255, 0, 0}); // Rouge (inversé dans setRGBColor)
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