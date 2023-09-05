/*

  Olivier Nadeau
  Gestion de la maison connectée

  29/08/2023 - Version 1

*/

// Librairies
//#include <Servo.h>
//#include <Wire.h>  Maintenant Inclu dans la librairie par defaut
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>  // Necessaire pour détourbée les communications Bluetooth
// Adresse I2C de l'écran LCD est 0x27, affiche 16 charactères sur 2 ligne
LiquidCrystal_I2C mylcd(0x27, 16, 2);

//set ports of two servos to digital 9 and 10
//Servo servo_10;
//Servo servo_9;

// Definition des Pin
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

bool gazBoul = false;
bool solBool = false;
bool boutonDroit;
bool boutonGauche;
const int gazMax = 300;
const int lumiereMax = 300;
const int eauMax = 800;
const int solHumidMax = 50;
const int solHumidMin = 10;

// ---

const int version = 1;

String buffer;
int btn1_num;      //set variable btn1_num
int btn2_num;      //set variable btn2_num
int button1;       //set variable button1
int button2;       //set variable button2
String fans_char;  //string type variable fans_char
int fans_val;      //set variable fans_char
int flag;          //set variable flag
int flag2;         //set variable flag2
int flag3;         //set variable flag3
int gas;           // defini la variable gas
int infrar;        //set variable infrar
String led2;       //string type variable led2
int lumiere;       //set variable light
String pass;       //string type variable pass
String passwd;     //string type variable passwd
int pluieBool = false;
String reception;
bool lumiereActManuel;

String servo1;     //string type variable servo1
int servo1_angle;  //set variable light
String servo2;     //string type variable servo2
int servo2_angle;  //set variable servo2_angle

int sol;         //set variable sol
int val;         //set variable val
int value_led2;  //set variable value_led2
int eau;         //set variable eau

int length;
int tonepin = 3;  //set the signal end of passive buzzer to digital 3

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

  //servo_9.attach(9);    //make servo connect to digital 9
  //servo_10.attach(10);  //make servo connect to digital 10
  //servo_9.write(0);     //set servo connected digital 9 to 0°
  //servo_10.write(0);    //set servo connected digital 10 to 0°
  //delay(300);

  // Definition des pines en mode sortie
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

  // length = sizeof(tune) / sizeof(tune[0]);  // Paramètre des tounes
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
    case 'a':
      Serial.println(reception + "DEL blanche allumée");
      digitalWrite(DELBlanchePin, HIGH);
      lumiereActManuel = true;
      break;
    case 'b':
      Serial.println(reception + "DEL blanche éteint");
      digitalWrite(DELBlanchePin, LOW);
      lumiereActManuel = false;
      break;
    case 'c':
      Serial.println(reception + "Relais allumé");
      digitalWrite(relaisPin, HIGH);
      break;
    case 'd':
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
    case 'h':
      if (reception = "2iem port série : ") {
        Serial.println(reception + "Valeur de la lumière");
        Serial.println(lumiere);  //output the value of variable light in new lines
      }
      delay(100);
      break;
    case 'i':
      if (reception = "2iem port série : ") {
        Serial.println(reception + "Valeur de la lumière");
        Serial.println(gas);  //output the value of variable gas in new lines
      }
      delay(100);
      break;
    case 'j':
      if (reception = "2iem port série : ") {
        Serial.println(reception + "Valeur de la lumière");
        Serial.println(sol);  //output the value of variable sol in new lines
      }
      delay(100);
      break;
    case 'k':
      if (reception = "2iem port série : ") {
        Serial.println(reception + "Valeur de la lumière");
        Serial.println(eau);  //output the value of variable eau in new lines
      }
      delay(100);
      break;
    case 'l':
      //servo_9.write(180);  //set servo connected to digital 9 to 180°
      delay(500);
      break;
    case 'm':
      //servo_9.write(0);   //set servo connected to digital 9 to 0°
      delay(500);
      break;
    case 'n':
      //servo_10.write(180);  //set servo connected to digital 10 to 180°
      delay(500);
      break;
    case 'o':
      //servo_10.write(0);  //set servo connected to digital 10 to 0°
      delay(500);
      break;
    case 'p':
      digitalWrite(DELJaunePin, HIGH);  //set digital 5 to high level, LED is on
      break;
    case 'q':
      digitalWrite(DELJaunePin, LOW);  // set digital 5 to low level, LED is off
      break;
    case 'r':
      digitalWrite(moteurDirectionPin, LOW);
      digitalWrite(moteurVitessePin, HIGH);  //fan rotates anticlockwise at the fastest speed
      break;
    case 's':
      digitalWrite(moteurDirectionPin, LOW);
      digitalWrite(moteurVitessePin, LOW);  //fan stops rotating
      break;
  }
  val = "";
}

void lireSenseurs() {
  gas = analogRead(gazPin);  //assign the analog value of A0 to gas
  if (gas > gazMax && !gazBoul) {
    Serial.println("Danger : Gaz Élevé");  // Pour débugger sur la console
    deuxiemePortSerie.println("Danger : Gaz Élevé");
    gazBoul = true;
  } else {
    if (gas <= gazMax) {
      gazBoul = false;
    }
  }

  lumiere = analogRead(lumierePin);  ////Assign the analog value of lumiere to lumiere
  if (!lumiereActManuel) {
    if (lumiere < lumiereMax)  //if variable lumiere is less than 300
    {
      Serial.println("lumière insuffisante");
      infrar = digitalRead(detectMouvPin);  //assign the value of digital 2 to infrar

      if (infrar) {
        digitalWrite(DELBlanchePin, HIGH);  // Allumer la DEL
      } else {
        digitalWrite(DELBlanchePin, LOW);  // Éteindre la DEL
      }

    } else {
      digitalWrite(DELBlanchePin, LOW);
    }
  }

  eau = analogRead(detectEauPin);  //assign the analog value of detectEauPin to variable eau
  if (eau > eauMax) {
    Serial.println("il pleut");
    deuxiemePortSerie.println("rain");  // Doit être en anglais pour le programe sur le telephone
    pluieBool = true;
  } else {
    if (eau <= eauMax) {
      pluieBool = true;
    }
  }

  sol = analogRead(detectHumidPin);
  if (sol > solHumidMax) {
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
    delay(10);                 //delay in 10ms
    while (boutonGauche == 0)  //if variableboutonGauche is 0，program will circulate
    {
      boutonGauche = digitalRead(4);  // assign the value of digital 4 to button1
      btn1_num = btn1_num + 1;        //variable btn1_num plus 1
      delay(100);                     // delay in 100ms
    }
  }
  if (btn1_num >= 1 && btn1_num < 5)  //1≤if variablebtn1_num<5
  {
    Serial.print(".");
    Serial.print("");
    passwd = String(passwd) + String(".");  //set passwd
    pass = String(pass) + String(".");      //set pass
    //LCD shows pass at the first row and column
    mylcd.setCursor(1 - 1, 2 - 1);
    mylcd.print(pass);
  }
  if (btn1_num >= 5)
  //if variablebtn1_num ≥5
  {
    Serial.print("-");
    passwd = String(passwd) + String("-");  //Set passwd
    pass = String(pass) + String("-");      //set pass
    //LCD shows pass at the first row and column
    mylcd.setCursor(1 - 1, 2 - 1);
    mylcd.print(pass);
  }
  if (boutonDroit == 0)  //if variableboutonDroit is 0
  {
    delay(10);
    if (boutonDroit == 0)  //if variableboutonDroit is 0
    {
      if (passwd == ".--.-.")  //if passwd is ".--.-."
      {
        mylcd.clear();  //clear LCD screen
        //LCD shows "open!" at first character on second row
        mylcd.setCursor(1 - 1, 2 - 1);
        mylcd.print("open!");
        //servo_9.write(100);  //set servo connected to digital 9 to 100°
        delay(300);
        delay(5000);
        passwd = "";
        pass = "";
        mylcd.clear();  //clear LCD screen
        //LCD shows "password:"at first character on first row
        mylcd.setCursor(1 - 1, 1 - 1);
        mylcd.print("password:");

      } else  //Otherwise
      {
        mylcd.clear();  //clear LCD screen
        //LCD shows "error!"at first character on first row
        mylcd.setCursor(1 - 1, 1 - 1);
        mylcd.print("error!");
        passwd = "";
        pass = "";
        delay(2000);
        //LCD shows "again" at first character on first row
        mylcd.setCursor(1 - 1, 1 - 1);
        mylcd.print("again");
      }
    }
  }

  infrar = digitalRead(2);  //assign the value of digital 2 to infrar
  if (infrar == 0 && (val != 'l' && val != 't'))
  //if variable infrar is 0 and val is not 'l' either 't'
  {
    //servo_9.write(0);  //set servo connected to digital 9 to 0°
    delay(50);
  }
  if (boutonDroit == 0)  //if variableboutonDroit is 0
  {
    delay(10);
    while (boutonDroit == 0)  //if variableboutonDroit is 0，program will circulate
    {
      boutonDroit = digitalRead(8);  //assign the value of digital 8 to boutonDroit
      btn2_num = btn2_num + 1;       //variable btn2_num plus 1
      delay(100);
      if (btn2_num >= 15)  //if variablebtn2_num ≥15
      {
        tone(3, 532);
        delay(125);
        mylcd.clear();  //clear LCD screen
        //LCD shows "password:" at the first character on first row
        mylcd.setCursor(1 - 1, 1 - 1);
        mylcd.print("password:");
        //LCD shows "wait" at the first character on first row
        mylcd.setCursor(1 - 1, 1 - 1);
        mylcd.print("wait");
      } else  //Otherwise
      {
        noTone(3);  //digital 3 stops playing music
      }
    }
  }

  btn1_num = 0;  //set btn1_num to 0
  btn2_num = 0;  //set btn2_num to 0
}

//PWM control
void pwm_control() {
  switch (val) {
    case 't':
      buffer = Serial.readStringUntil('#');  // Vider le buffer (tampon) du port serie
      // servo1_angle = String(servo1).toInt();
      //servo_9.write(servo1_angle);  //set the angle of servo connected to digital 9 to servo1_angle
      // delay(300);
      break;
    case 'u':
      buffer = Serial.readStringUntil('#');  // Vider le buffer (tampon) du port serie
      // servo2_angle = String(servo2).toInt();
      // servo_10.write(servo2_angle);  //set the angle of servo connected to digital 10 to servo2_angle
      delay(300);
      break;
    case 'v':  // Point supplementaire pour valider les points reçus
      led2 = Serial.readStringUntil('#');
      value_led2 = String(led2).toInt();
      analogWrite(DELJaunePin, value_led2);  //PWM value of digital 5 is value_led2v
      break;
    case 'w':  // Point supplementaire pour valider les points reçus
      fans_char = Serial.readStringUntil('#');
      fans_val = String(fans_char).toInt();
      digitalWrite(moteurDirectionPin, LOW);
      analogWrite(moteurVitessePin, fans_val);  //set PWM value of digital 6 to fans_val，the larger the value, the faster the fan
      break;
  }
}
