/*

  Olivier Nadeau
  Gestion de la maison connectée

  29/08/2023 - Version 1
  05/09/2023 - Version 2
*/

// Librairies
//#include <Servo.h>
//#include <Wire.h>  Maintenant Inclu dans la librairie par defaut
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>  // Necessaire pour détourbée les communications Bluetooth
// Adresse I2C de l'écran LCD est 0x27, affiche 16 charactères sur 2 ligne
LiquidCrystal_I2C mylcd(0x27, 16, 2);


// Definition des Pin ---
const int moteurDirectionPin = 7;
const int moteurVitessePin = 6;
const int boutonGauchePin = 4;
const int boutonDroitePin = 8;
const int detectMouvPin = 2;
const int buzzerPin = 3;
const int lumierePin = A1;     // Pin analogue ne lecture
const int DELBlanchePin = 13;  // DEL sur le paneau Arduino et la blanche à l'extérieur de la maison
const int detectEauPin = A3;
const int detectHumidPin = A2;  // Détecteur d'humidité dans le sol
const int gazPin = A0;
const int relaisPin = 12;   // Relais devant la maison
const int DELJaunePin = 5;  // DEL près du toit

// ---

// Definition des variables
bool gazBoul = false;
bool solBool = false;
bool boutonDroit;
bool boutonGauche;
const int gazMax = 300;
const int lumiereMax = 300;
const int eauMax = 800;
const int solHumidMax = 50;
const int solHumidMin = 10;

const int version = 2;

String buffer;
int btn1_num;
int btn2_num;
int button1;
int button2;
String fans_char;
int fans_val;
int flag;
int flag2;
int flag3;
int gas;
int infrar;
String led2;
int lumiere;
String pass;
String passwd;
int pluieBool = false;
String reception;
bool lumiereActManuel;

String servo1;
int servo1_angle;
String servo2;
int servo2_angle;

int sol;
int val;
int value_led2;
int eau;

int length;
int tonepin = 3;

// ---

const byte rxPin = 9;
const byte txPin = 10;
SoftwareSerial deuxiemePortSerie(rxPin, txPin);

void setup() {
  Serial.begin(9600);  // Initialiser la console
  Serial.println("Senseurs Maison Keyes Version " + version);
  deuxiemePortSerie.begin(9600);  // Initialiser le port série pour le Bluetooth ou une autre module en série

  mylcd.init();              // Initialiser le paneau LCD
  mylcd.backlight();         // Allumer la DEL du paneau LCD
  mylcd.setCursor(0, 0);     // En informatique, on commence à compter à 0
  mylcd.print("passcord:");  // Afficher sur le LCD "passcord:" sur la première ranger de la première colone

  // Definition des pines en mode sortie ---
  pinMode(moteurDirectionPin, OUTPUT);
  pinMode(moteurVitessePin, OUTPUT);
  digitalWrite(moteurDirectionPin, HIGH);
  digitalWrite(moteurVitessePin, HIGH);

  pinMode(boutonGauchePin, INPUT);
  pinMode(boutonDroitePin, INPUT);
  pinMode(detectMouvPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(gazPin, INPUT);
  pinMode(lumierePin, INPUT);
  pinMode(DELBlanchePin, OUTPUT);
  pinMode(detectEauPin, INPUT);
  pinMode(detectHumidPin, INPUT);

  pinMode(relaisPin, OUTPUT);
  pinMode(DELJaunePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // ---
}

void loop() {
  lireSenseurs();  // Pour toujours avoir l'état de la maison

  if (deuxiemePortSerie.available() > 0) {
    val = deuxiemePortSerie.read();
    reception = ("2iem port série : ");
  }
  if (Serial.available() > 0)  //serial reads the characters
  {
    val = Serial.read();  //set val to character read by serial    Serial.println(val);//output val character in new lines
    reception = ("Console : ");
  }
  switch (val) {
    case 'a':  // Allumer la DEL blanche
      Serial.println(reception + "DEL blanche allumée");
      digitalWrite(DELBlanchePin, HIGH);
      lumiereActManuel = true;
      break;
    case 'b':  // Éteindre la DEL blanche
      Serial.println(reception + "DEL blanche éteint");
      digitalWrite(DELBlanchePin, LOW);
      lumiereActManuel = false;
      break;
    case 'c':  // Allumer le relais
      Serial.println(reception + "Relais allumé");
      digitalWrite(relaisPin, HIGH);
      break;
    case 'd':  // Éteindre le relais
      Serial.println(reception + "Relais Éteint");
      digitalWrite(relaisPin, LOW);
      break;
    case 'e':
      //music1();
      break;
    case 'f':
      //music2();
      break;
    case 'g':
      //noTone(3);
      break;
    case 'h':  // Output la valeur de lumière
      if (reception = "2iem port série : ") {
        Serial.println(reception + "Valeur de la lumière");
        Serial.println(lumiere);  //output the value of variable light in new lines
      }
      delay(100);
      break;
    case 'i':  // Output la valeur du gas
      if (reception = "2iem port série : ") {
        Serial.println(reception + "Valeur du gas");
        Serial.println(gas);
      }
      delay(100);
      break;
    case 'j':  // Output la valeur du sol
      if (reception = "2iem port série : ") {
        Serial.println(reception + "Valeur du sol");
        Serial.println(sol);
      }
      delay(100);
      break;
    case 'k':  // Output la valeur de l'eau
      if (reception = "2iem port série : ") {
        Serial.println(reception + "Valeur de l'eau");
        Serial.println(eau);
      }
      delay(100);
      break;
    case 'l':
      //servo_9.write(180);
      //delay(500);
      break;
    case 'm':
      //servo_9.write(0);
      //delay(500);
      break;
    case 'n':
      //servo_10.write(180);
      //delay(500);
      break;
    case 'o':
      //servo_10.write(0);
      //delay(500);
      break;
    case 'p':  // Allumer la LED jaune
      digitalWrite(DELJaunePin, HIGH);
      break;
    case 'q':  // Eteindre la LED jaune
      digitalWrite(DELJaunePin, LOW);
      break;
    case 'r':  // Allumer la fan
      digitalWrite(moteurDirectionPin, LOW);
      digitalWrite(moteurVitessePin, HIGH);
      break;
    case 's':  // Eteindre la fan
      digitalWrite(moteurDirectionPin, LOW);
      digitalWrite(moteurVitessePin, LOW);  //fan stops rotating
      break;
    case 't':
      //buffer = Serial.readStringUntil('#');  // Vider le buffer (tampon) du port serie
      // servo1_angle = String(servo1).toInt();
      //servo_9.write(servo1_angle);
      // delay(300);
      break;
    case 'u':
      //buffer = Serial.readStringUntil('#');  // Vider le buffer (tampon) du port serie
      // servo2_angle = String(servo2).toInt();
      // servo_10.write(servo2_angle);  //set the angle of servo connected to digital 10 to servo2_angle
      //delay(300);
      break;
    case 'v':  // Allumer la DEL jaune selon une certaine intensité
      led2 = Serial.readStringUntil('#');
      value_led2 = String(led2).toInt();     // transforme la valeur en int
      analogWrite(DELJaunePin, value_led2);  // Allume la DEL jaune
      break;
    case 'w':  // Allumer le fan selon une certaine intensité
      fans_char = Serial.readStringUntil('#');
      fans_val = String(fans_char).toInt();     // Sauvegarde la valeur
      digitalWrite(moteurDirectionPin, LOW);    // Set la direction de rotation du fan
      analogWrite(moteurVitessePin, fans_val);  // allume le fan à la vitesse choisi
      break;
  }
  val = "";  // Reset la val
}

// Lire Sensor
void lireSenseurs() {
  gas = analogRead(gazPin);                           // lis la variable du gas
  if (gas > gazMax && !gazBoul) {                     // si gas est plus grand que gasMax
    Serial.println("Danger : Gaz Élevé");             // Pour débugger sur la console
    deuxiemePortSerie.println("Danger : Gaz Élevé");  // écrire sir le deuxième port série "Danger : Gaz Élevé"
    gazBoul = true;
  } else {
    if (gas <= gazMax) {
      gazBoul = false;
    }
  }

  // ------------ Lumière ------------
  lumiere = analogRead(lumierePin);
  if (!lumiereActManuel) {     // Si la lumière n'a pas été activer manuelement
    if (lumiere < lumiereMax)  // Si la lumière est plus base que la lumière max
    {
      Serial.println("lumière insuffisante");
      infrar = digitalRead(detectMouvPin);  // sauvegarde la valeur du détecteur de mouvement

      if (infrar) {
        digitalWrite(DELBlanchePin, HIGH);  // Allumer la DEL blanche
      } else {
        digitalWrite(DELBlanchePin, LOW);  // Éteindre la DEL blanche
      }

    } else {
      digitalWrite(DELBlanchePin, LOW);  // Éteind la DEL blanche
    }
  }

  // ------------ Eau ------------
  eau = analogRead(detectEauPin);  // sauvegarde la valeur du detecteur d'eau
  if (eau > eauMax) {              // Si eau est plsu grande que la valeur max
    Serial.println("il pleut");
    deuxiemePortSerie.println("rain");  // Doit être en anglais pour le programe sur le telephone
    pluieBool = true;
  } else {
    if (eau <= eauMax) {
      pluieBool = true;
    }
  }

  // ------------ Humidité du sol ------------
  sol = analogRead(detectHumidPin);  // sauvegarde la valeur du détecteur d'humidité du sol
  if (sol > solHumidMax) {           // si sol est plus grand que la valeur de sol max
    Serial.println("sol trop humide");
    deuxiemePortSerie.println("hydroponia ");
    solBool = true;
  } else {
    if (sol <= solHumidMax && sol >= solHumidMin) {
      solBool = false;
    } else {
      if (sol < solHumidMin) {
        // Serial.println("sol trop sec");    // Mis en commantaire pour ne pas spam la consol lors du dev
        solBool = true;
      }
    }
  }
  //door();  //run subroutine
}

void door() {
  boutonGauche = digitalRead(boutonGauchePin);
  boutonDroit = digitalRead(boutonDroitePin);

  if (boutonGauche == 0) {
    delay(10);
    while (boutonGauche == 0) {
      boutonGauche = digitalRead(4);
      btn1_num = btn1_num + 1;
      delay(100);
    }
  }
  if (btn1_num >= 1 && btn1_num < 5) {
    Serial.print(".");
    Serial.print("");
    passwd = String(passwd) + String(".");
    pass = String(pass) + String(".");
    mylcd.setCursor(1 - 1, 2 - 1);
    mylcd.print(pass);
  }
  if (btn1_num >= 5) {
    Serial.print("-");
    passwd = String(passwd) + String("-");
    pass = String(pass) + String("-");
    mylcd.setCursor(1 - 1, 2 - 1);
    mylcd.print(pass);
  }
  if (boutonDroit == 0) {
    delay(10);
    if (boutonDroit == 0) {
      if (passwd == ".--.-.") {
        mylcd.clear();
        mylcd.setCursor(1 - 1, 2 - 1);
        mylcd.print("open!");
        //servo_9.write(100);
        delay(300);
        delay(5000);
        passwd = "";
        pass = "";
        mylcd.clear();
        mylcd.setCursor(1 - 1, 1 - 1);
        mylcd.print("password:");

      } else {
        mylcd.clear();
        mylcd.setCursor(1 - 1, 1 - 1);
        mylcd.print("error!");
        passwd = "";
        pass = "";
        delay(2000);
        mylcd.setCursor(1 - 1, 1 - 1);
        mylcd.print("again");
      }
    }
  }

  infrar = digitalRead(2);
  if (infrar == 0 && (val != 'l' && val != 't')) {
    delay(50);
  }
  if (boutonDroit == 0) {
    delay(10);
    while (boutonDroit == 0) {
      boutonDroit = digitalRead(8);
      btn2_num = btn2_num + 1;
      delay(100);
      if (btn2_num >= 15) {
        tone(3, 532);
        delay(125);
        mylcd.clear();
        mylcd.setCursor(1 - 1, 1 - 1);
        mylcd.print("password:");
        mylcd.setCursor(1 - 1, 1 - 1);
        mylcd.print("wait");
      } else {
        noTone(3);
      }
    }
  }

  btn1_num = 0;
  btn2_num = 0;
}

//PWM control
void pwm_control() {
  switch (val) {
  }
}
