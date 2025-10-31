/*
  Projet : Contrôle d'un relais via Bluetooth HC-05
  Compatible :
    - Commandes simples via boutons ('1' pour ON, '0' pour OFF)
    - Commandes vocales via speech-to-text ('turn on', 'start', 'turn off', 'stop', etc.)
  Matériel :
    - Arduino UNO
    - Module HC-05
    - Relais connecté à une lampe 220V
    - Optionnel : LED témoin sur pin 13 pour visualiser l'état du relais
*/

// Inclusion de la bibliothèque SoftwareSerial
#include <SoftwareSerial.h>

// ==========================
// Définition des pins
// ==========================
#define RX_BT 11     // Broche Arduino connectée au TX du HC-05
#define TX_BT 10     // Broche Arduino connectée au RX du HC-05
#define RELAY_PIN 8  // Broche Arduino connectée au relais
#define LED_PIN 13   // LED témoin

// ==========================
// Création de l'objet SoftwareSerial
// ==========================
SoftwareSerial bluetooth(RX_BT, TX_BT);

// ==========================
// Variables globales
// ==========================
String buffer = "";               
unsigned long lastReceiveTime = 0; 

// ==========================
// Fonction setup() : initialisation
// ==========================
void setup() {
  pinMode(RELAY_PIN, OUTPUT);     
  digitalWrite(RELAY_PIN, LOW);  

  pinMode(LED_PIN, OUTPUT);       
  digitalWrite(LED_PIN, LOW);     

  Serial.begin(9600);             
  bluetooth.begin(9600);          

  Serial.println("=== Bluetooth Relay Control (Voice + Button) ===");
  Serial.println("Send or say:");
  Serial.println("- '1' or 'turn on' or 'start' → ON");
  Serial.println("- '0' or 'turn off' or 'stop' → OFF");
}

// ==========================
// Fonction loop() : boucle principale
// ==========================
void loop() {
  while (bluetooth.available()) {  
    char c = bluetooth.read();     

    if (isPrintable(c)) {
      buffer += c;                
      lastReceiveTime = millis(); 
    }
  }

  // Détection de fin de commande (timeout 200 ms)
  if (buffer.length() > 0 && millis() - lastReceiveTime > 200) {
    processCommand(buffer); 
    buffer = "";            
  }
}

// ==========================
// Fonction processCommand() : analyse et exécute la commande
// ==========================
void processCommand(String cmd) {
  cmd.trim();       
  cmd.toLowerCase(); 

  Serial.print("Commande reçue : ");
  Serial.println(cmd); 

  // ==========================
  // Commandes ON strictes
  // ==========================
  if (cmd == "1" || cmd == "turn on" || cmd == "start" || cmd == "on") {
    digitalWrite(RELAY_PIN, HIGH);  
    digitalWrite(LED_PIN, HIGH);    
    bluetooth.println("Relay ON"); 
    Serial.println("Relais activé (lampe allumée)"); 
  }
  // ==========================
  // Commandes OFF strictes
  // ==========================
  else if (cmd == "0" || cmd == "turn off" || cmd == "stop" || cmd == "off") {
    digitalWrite(RELAY_PIN, LOW);   
    digitalWrite(LED_PIN, LOW);     
    bluetooth.println("Relay OFF"); 
    Serial.println("Relais désactivé (lampe éteinte)"); 
  }
  // ==========================
  // Commande non reconnue
  // ==========================
  else {
    bluetooth.println("Commande non reconnue : " + cmd);
    Serial.println("Commande non reconnue"); 
  }
}
