# Code Serveur

Ce code à pour but de créer un mini-serveur avec la carte ESP12 pour afficher la température via un DHT11/22

## Pré-requis

Il faut auparavant vérifier que l'on puisse téléverser du code sur les cartes ESP12. (Voir Section UTILISATION DE L’ESP12 SOUS ARDUINO dans le document PDF [Atelier Réseaux et Serveurs])

## Étapes

- Configurer les paramètres dans le code Arduino et vérifier que le nom du réseau et le mot de passe est le bon (Créafab-Invite dans les locaux, sinon votre nom de box chez vous) et le mot de passe associé : 

```

//########################### Début Paramètres 
#define PORT 80 //Port par défaut
#define LED D4  //Broche de la LED

#define REFRESH_PAGE_DELAY 5 //Temps de reafraichissement de la page en s

#define DHTPIN D2     //Broche du DHT11/22

#define DHTTYPE DHT11   
//#define DHTTYPE DHT22  
//#define DHTTYPE DHT21 

#define NB_DATA_TEMP 60

const char* ssid     = "Freebox-47689C";//Nom du routeur sur le réseau (par Exemple FReebox-44F45)
const char* password = "oshysi5ttialourtylop"; //Mot de passe du routeur


//###########################Fin paramètres
```

- Téléverser le code Serveur_Web_DHT11_Graph.ino
- Ouvrir le moniteur série et verifier que le module ESP12  se connecte au réseau en affichant son adresse IP

- Saisir son adresse IP dans la barre du haut de votre navigateur.

- L'interface se lance
