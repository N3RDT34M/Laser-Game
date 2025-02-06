#include <Adafruit_SSD1306.h>
#include "DetectionLumiere.h"
#include "EmissionLaser.h"

#define nombreDePixelsEnLargeur 128
#define nombreDePixelsEnHauteur 64
#define brocheResetOLED -1
#define adresseI2CecranOLED 0x3C

#define boutonPinUltime 8
#define boutonPinSurchauffe 9

bool BitsDuJoueur1[4] = {false, true, true, true};

const int PORT11 = A0;

Adafruit_SSD1306 oled(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

// ----- Compte à rebours avant début de partie -----
bool compteReboursActif = true;
unsigned long debutCompteRebours;
const unsigned long dureeCompteRebours = 10000; // 10 secondes
bool messageDepartAffiche = false;
bool attenteFinMessage = false;

// ----- Layout Temps restant de la partie + Chargement ultime -----
unsigned long dureeTotale = 1200000; // 20 minutes en ms
unsigned long tempsRestant = dureeTotale;
unsigned long tempsPrecedent = 0;
unsigned long dureeChargeUltime = 300000; // 5 minutes en ms
unsigned long tempsRestantUltime = dureeChargeUltime;

// ----- Layout Surchauffe -----
bool modeSurchauffe = false;
unsigned long dernierAppuiBouton = 0;
const unsigned long delaiEntreTirs = 200; // Temps minimum entre deux tirs
unsigned long tempsDebutSurchauffe = 0;
const unsigned long dureeSurchauffe = 5000; // 5 secondes

int niveauSurchauffe = 0; // Niveau actuel de surchauffe (0 à 100)
const int incrementSurchauffe = 7; // Ajout de 10% à chaque appui
const int largeurMaxBarre = 100; // Largeur max de la barre (100%)

bool afficherBarre = true; // Contrôle du clignotement
unsigned long dernierClignotement = 0;
const unsigned long intervalleClignotement = 100; // 100 ms pour alterner

unsigned long debutMessageDepart = 0;

void setup() {
    pinMode(boutonPinUltime, INPUT);
    pinMode(boutonPinSurchauffe, INPUT);

    Serial.begin(9600);
    Serial.println(F("Initialisation de l'OLED..."));

    SETUPDetectionLumiere();
    SETUPEmissionLaser(BitsDuJoueur1);
    gachettePresse(false);

    if (!oled.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)) {
        Serial.println(F("Erreur OLED."));
        while (true);
    }

    oled.clearDisplay();
    oled.display();

    debutCompteRebours = millis(); // Démarrer le compte à rebours
}

void loop() {
    unsigned long tempsActuel = millis();

    if (compteReboursActif) {
        unsigned long tempsEcoule = tempsActuel - debutCompteRebours;

        if (tempsEcoule < dureeCompteRebours) {
            afficherCompteRebours((dureeCompteRebours - tempsEcoule) / 1000);
        } 
        else if (!messageDepartAffiche) {
            afficherMessageDepart();
            messageDepartAffiche = true;
            debutMessageDepart = tempsActuel; // Définir le début de l'affichage du message
            attenteFinMessage = true;
        } 
        else if (attenteFinMessage && (tempsActuel - debutMessageDepart >= 3000)) {
            compteReboursActif = false;
            attenteFinMessage = false;
        }
        return;
    }


    // Mise à jour du minuteur et de l'ultime
    if (tempsActuel - tempsPrecedent >= 1000) {
        tempsPrecedent = tempsActuel;
        if (tempsRestant > 0) tempsRestant -= 1000;
        if (tempsRestantUltime > 0) tempsRestantUltime -= 1000;
    }

    // Lire l'état du bouton de surchauffe (appuyé = HIGH)
    bool boutonSurchauffeAppuye = digitalRead(boutonPinSurchauffe) == HIGH;

    // Empêcher l'activation du laser si la surchauffe est maximale
    if (niveauSurchauffe >= 100) {
        boutonSurchauffeAppuye = false;
    }

    // Activer le laser lorsque le bouton est pressé
    gachettePresse(boutonSurchauffeAppuye);

    // Si le bouton est appuyé et que le délai entre deux tirs est écoulé
    if (boutonSurchauffeAppuye && (tempsActuel - dernierAppuiBouton >= delaiEntreTirs)) {
        modeSurchauffe = true;
        tempsDebutSurchauffe = tempsActuel;
        dernierAppuiBouton = tempsActuel;

        // Augmenter la surchauffe tant qu'on peut
        if (niveauSurchauffe < 100) {
            niveauSurchauffe += incrementSurchauffe;
        }
    }

    // Gestion du clignotement si la surchauffe est à 100%
    if (niveauSurchauffe >= 100 && (tempsActuel - dernierClignotement >= intervalleClignotement)) {
        afficherBarre = !afficherBarre;
        dernierClignotement = tempsActuel;
    }

    // Réinitialisation après 5 secondes
    if (modeSurchauffe && (tempsActuel - tempsDebutSurchauffe >= dureeSurchauffe)) {
        modeSurchauffe = false;
        niveauSurchauffe = 0;
        afficherBarre = true;
    }

    // Mise à jour de l'affichage
    oled.clearDisplay();
    if (modeSurchauffe) {
        afficherSurchauffe();
    } else {
        afficherTemps(tempsRestant);
        afficherChargementUltime(tempsRestantUltime, dureeChargeUltime);
    }
    oled.display();
}

void afficherCompteRebours(int secondes) {
    oled.clearDisplay();
    oled.setTextSize(3);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(40, 20);
    oled.print(secondes);
    oled.display();
}

void afficherMessageDepart() {
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(10, 20);
    oled.print("La partie commence !");
    oled.display();
}


// Fonction pour afficher le temps restant 
void afficherTemps(unsigned long tempsEnMs) {
    unsigned long secondes = (tempsEnMs / 1000) % 60;
    unsigned long minutes = (tempsEnMs / 60000) % 60;

    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(35, 0);
    char buffer[9];
    sprintf(buffer, "%02lu:%02lu", minutes, secondes);
    oled.print(buffer);
}

// Fonction pour afficher le pourcentage de chargement de l'ultime
void afficherChargementUltime(unsigned long tempsRestantUltime, unsigned long dureeChargeUltime) {
    float pourcentage = ((float)(dureeChargeUltime - tempsRestantUltime) / dureeChargeUltime) * 100.0;

    oled.setTextSize(4);
    oled.setCursor(30, 30);
    oled.print((int)pourcentage);
    oled.print("%");
}

// Fonction pour afficher l'écran de surchauffe avec une barre de progression clignotante
void afficherSurchauffe() {
    int largeurBarre = (niveauSurchauffe * largeurMaxBarre) / 100; // Convertir % en pixels

    // Titre "SURCHAUFFE"
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 5);
    oled.print("SURCHAUFFE");

    // Dessiner la barre seulement si elle doit être affichée (clignotement)
    if (niveauSurchauffe < 100 || afficherBarre) {
        oled.drawRect(14, 35, largeurMaxBarre, 10, SSD1306_WHITE); // Cadre
        oled.fillRect(14, 35, largeurBarre, 10, SSD1306_WHITE); // Remplissage
    }

    // Afficher le pourcentage à côté de la barre
    oled.setTextSize(1);
    oled.setCursor(120, 35);
}
