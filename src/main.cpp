#include <Arduino.h>

// Définition des pins pour la LED RGB
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11
#define COMMON_PIN 12

// Structure pour les couleurs RGB
struct RGBColor {
    int red;
    int green;
    int blue;
};

// Variables globales
RGBColor storedColor = {255, 0, 0}; // Couleur par défaut (rouge)
bool isLedOn = false;

// Initialisation de la LED
void initRGBLed() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(COMMON_PIN, OUTPUT);
    
    digitalWrite(COMMON_PIN, HIGH);
    turnOffRGB();
}

// Fonction pour changer la couleur
void setRGBColor(RGBColor color) {
    storedColor = color; // Sauvegarde la nouvelle couleur
    if (isLedOn) {
        // Applique la couleur seulement si la LED est allumée
        analogWrite(RED_PIN, 255 - color.red);
        analogWrite(GREEN_PIN, 255 - color.green);
        analogWrite(BLUE_PIN, 255 - color.blue);
    }
}

// Éteindre la LED
void turnOffRGB() {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, HIGH);
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
}

void loop() {
    // Exemple d'utilisation
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        if (cmd == 's') {
            switchOnOff(); // Bascule ON/OFF quand 's' est reçu
        }
        else if (cmd == 'r') {
            setRGBColor({255, 0, 0}); // Rouge
        }
        else if (cmd == 'g') {
            setRGBColor({0, 255, 0}); // Vert
        }
        else if (cmd == 'b') {
            setRGBColor({0, 0, 255}); // Bleu
        }
    }
}