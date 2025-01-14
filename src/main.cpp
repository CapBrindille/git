/*
  Code fait par mathis BENOIT
  Compileur : Microsoft Visual Studio Code v1.96.0
  IDE : PlateformIO v3.3.3

  Programme pour faire fonctionner un petit clavier en 4x4 en utilisant une carte Arduino Mega

  Câblage : 
    clavier --> Arduino
    C1 --> 9
    C2 --> 8
    C3 --> 7
    C4 --> 6
    R1 --> 5
    R2 --> 4
    R3 --> 3
    R4 --> 2

  dernière modification : mardi 17/12/2024
*/
#include <Arduino.h>          //Librairie pour utiliser les commandes de l'IDE Arduino sur Visual Studio Code
#include <Keypad.h>           //Librairie pour utiliser le clavier 

#define COLS 4                //Le clavier aura 4 colognes 
#define ROWS 4                //Le clavier aura 4 lignes
char keys[ROWS][COLS] = {
  {'1','2','3','F'},
  {'4','5','6','E'},          //Disposition des touches du clavier
  {'7','8','9','D'},
  {'A','0','B','C'}
};

byte rowPins[ROWS] = {5, 4, 3, 2};      //Broches de l'Arduino sur lesquelles les lignes du clavier seront connectées
byte colPins[COLS] = {9, 8, 7, 6};      //Broches de l'Arduino sur lesquelles les colognes du clavier seront connectées

Keypad clavier = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);      //On associe les broches des colognes comme étant pour les colognes, idem pour les lignes

char mdp[4] = {'6','4','2','0'};
int i = 0;
int fail = 3;
char essai[4];

void setup() {
  Serial.begin(9600);                         //Initialisation du moniteur série
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(10,LOW);
  digitalWrite(11,LOW); 
}

void loop() {
 bool clear = 0;
 Serial.print("Veuillez rentrer votre mot de passe : ");
 for (int i = 0; i <= 3; i++){    //On demande le code à 4 chiffres
 char touche = clavier.getKey();
 while (touche == false){         //Du moment qu'aucune touche n'est pressée, on reste dans la boucle en attendant
  touche = clavier.getKey();    
 }
 if (touche == 'C'){     //Création d'une touche "clear" qui réinitialise le mot de passe
  i = 3;                 //On met i à 3 pour sortir de l boucle for
  clear = 1;             //On met la variable clear à 1 pour dire au programme qu'il doit recommencer
  Serial.println();
 }
 else {
  essai[i] = touche;       //On ajoute la touche pressé dans le tableau qui contient le code qui est en train d'être rentré
  Serial.print(touche);    //on affiche le code qui est rentré 
 }
 }

 if (clear != 1){       //On ne fait quelque chose que si la touche clear n'a pas été pressé
 if (essai[0] == mdp[0] && essai[1]==mdp[1] && essai[2]==mdp[2] && essai[3]==mdp[3]) { //vu que le tuple mdp se termine par un ETX (end of txt), il faut comparer les éléments 1 à 1
  digitalWrite(10, HIGH);     //On allume la led verte
  Serial.println("... Mot de passe correct");
  fail = 3 ;      //On remet le nombre d'essais à 3
  delay(5000);
  digitalWrite(10, LOW);    //On eteint la led verte après 5s
  Serial.println();
 }
 else {
  Serial.print("... Mot de passe incorrect, ");
  fail = fail - 1 ;       //Si le mdp est faux, on enlève un essai
  Serial.print(fail);     //On affiche le nombre d'essais restants 
  Serial.println(" essai(s) restant(s)");
  
  if (fail == 0) {            //Si le nombre d'essais tombre à 0, alors la carte se bloque
    digitalWrite(11, HIGH);   //On allume la led rouge
    Serial.println("Trop d'essais incorrects, blocage de la carte ");
    delay(5000);              //La carte attend sans rien faire pendant 5s
    digitalWrite(11, LOW);    //On éteint la led rouge
    fail = 3;                 //On remet le nombre d'essais à 3
    Serial.println();
  }
 }
 }
}
