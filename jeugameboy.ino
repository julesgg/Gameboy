#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <WiFi.h>
#include <WiFiUdp.h>


// Paramètres OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SDA_PIN 5
#define SCL_PIN 6
#define OLED_RESET -1

// Paramètres WiFi
const char* softAP_ssid = "ESP32-Access-Point";
const char* softAP_password = "12345678";

IPAddress serverIP;
// Paramètres UDP
WiFiUDP udp;
unsigned int localPort = 9999; // Port UDP local
char packetBuffer[255];


// Création de l'affichage OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Paramètres de la clé
#define O_1 20
#define O_2 10
#define O_3 0
#define I_1 7
#define I_2 8
#define I_3 9
const byte ROWS = 3;
const byte COLS = 3;
byte rowPins[ROWS] = {O_1, O_2, O_3};
byte colPins[COLS] = {I_1, I_2, I_3};
char keys[ROWS][COLS] = {
  {'U', 'L', 'D'},
  {'R', 'S', 'X'},
  {'A', 'B', 'Y'}
};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int pistoletWidth=11;
int pistoletHeight=17;
const unsigned char PROGMEM pistoletBitmap[] = {   0x60, 0x00, 
  0x60, 0x00, 
  0x60, 0x00, 
  0x60, 0x00, 
  0xf0, 0x00, 
  0xf8, 0x00, 
  0xf0, 0x00, 
  0xf6, 0x00, 
  0xfa, 0x00, 
  0xf2, 0x00, 
  0xfc, 0x00, 
  0xf8, 0x00, 
  0x7e, 0x00, 
  0x3f, 0x80, 
  0x1f, 0xc0, 
  0x0f, 0x80, 
  0x00, 0x00,
};

int manWidth=30;
int manHeight=23;
const unsigned char PROGMEM manBitmap[] = {  0x00, 0xfc, 0x00, 0x00, 
  0x01, 0xfe, 0x00, 0x00, 
  0x03, 0xff, 0x00, 0x00, 
  0x07, 0x00, 0x80, 0x00, 
  0x06, 0x00, 0x80, 0x00, 
  0x06, 0x3b, 0x80, 0x00, 
  0x06, 0x7b, 0x80, 0x00, 
  0x06, 0x7f, 0x8f, 0xf8, 
  0x0f, 0x39, 0x90, 0x04, 
  0x0f, 0x83, 0xfb, 0x04, 
  0x3f, 0xff, 0xfe, 0xf8, 
  0x7f, 0xff, 0xff, 0xf8, 
  0x7f, 0xff, 0xff, 0x00, 
  0x7f, 0xff, 0x7e, 0x00, 
  0x7f, 0xfe, 0x00, 0x00, 
  0x7f, 0xfe, 0x00, 0x00, 
  0x3f, 0xfe, 0x00, 0x00, 
  0x07, 0xff, 0x00, 0x00, 
  0x0f, 0xff, 0x80, 0x00, 
  0x1f, 0xdf, 0xc0, 0x00, 
  0x7f, 0x8f, 0xf0, 0x00, 
  0xff, 0x8f, 0xf8, 0x00, 
  0xff, 0x8f, 0xf8, 0x00, 
};

int mamieWidth=16;
int mamieHeight=25;
const unsigned char PROGMEM mamieBitmap[] = {   0x03, 0x80, 
  0x0c, 0x70, 
  0x10, 0x08, 
  0x20, 0x0c, 
  0x3c, 0x04, 
  0x12, 0x02, 
  0x11, 0x82, 
  0x34, 0x44, 
  0x21, 0x2c, 
  0x26, 0x78, 
  0x10, 0x7c, 
  0x1f, 0xfc, 
  0x3f, 0xbc, 
  0xff, 0x7e, 
  0xc8, 0x6e, 
  0xff, 0xee, 
  0x9c, 0x1e, 
  0x9f, 0xfe, 
  0xbb, 0xff, 
  0x9b, 0xff, 
  0x9b, 0xff, 
  0x93, 0xfc, 
  0x9f, 0xf8, 
  0xb9, 0x88, 
  0xbf, 0xf8, 
};

int giraffeWidth=20;
int giraffeHeight=31;
const unsigned char PROGMEM giraffeBitmap[] = {  0x00, 0x00, 0x00, 
  0x06, 0x80, 0x00, 
  0x03, 0x80, 0x00, 
  0x07, 0x80, 0x00, 
  0x18, 0x40, 0x00, 
  0x34, 0x20, 0x00, 
  0x6c, 0x60, 0x00, 
  0x40, 0xc0, 0x00, 
  0x9f, 0xc0, 0x00, 
  0x63, 0x40, 0x00, 
  0x03, 0xc0, 0x00, 
  0x03, 0xc0, 0x00, 
  0x03, 0x40, 0x00, 
  0x03, 0xc0, 0x00, 
  0x03, 0xc0, 0x00, 
  0x02, 0x60, 0x00, 
  0x03, 0x3f, 0x00, 
  0x02, 0x49, 0x80, 
  0x02, 0xec, 0xc0, 
  0x02, 0x4e, 0xe0, 
  0x03, 0x0c, 0xd0, 
  0x01, 0x32, 0xc0, 
  0x01, 0xff, 0x40, 
  0x01, 0xcf, 0x40, 
  0x01, 0xc7, 0xc0, 
  0x01, 0xc3, 0x40, 
  0x01, 0x42, 0x40, 
  0x01, 0x42, 0x40, 
  0x01, 0x42, 0x40, 
  0x01, 0x42, 0x40, 
  0x01, 0x42, 0x40,
};

int bonusWidth=20;
int bonusHeight=19;
const unsigned char PROGMEM bonusBitmap[] = {  0x00, 0x60, 0x00, 
  0x00, 0xf0, 0x00, 
  0x00, 0x90, 0x00, 
  0x01, 0x98, 0x00, 
  0x01, 0x08, 0x00, 
  0x02, 0x0e, 0x00, 
  0x7c, 0x07, 0xe0, 
  0xc0, 0x00, 0x10, 
  0x41, 0x10, 0x20, 
  0x21, 0x98, 0x40, 
  0x11, 0x98, 0x80, 
  0x08, 0x03, 0x00, 
  0x0d, 0x11, 0x00, 
  0x08, 0xe1, 0x00, 
  0x08, 0x01, 0x00, 
  0x08, 0x61, 0x80, 
  0x09, 0xb8, 0x80, 
  0x1b, 0x0e, 0x80, 
  0x0c, 0x03, 0x80, 
};

int beletteWidth=25;
int beletteHeight=17;
const unsigned char PROGMEM beletteBitmap[] = { 0x1a, 0xc0, 0x00, 0x00, 
  0x2f, 0xa0, 0x00, 0x00, 
  0x3f, 0xe0, 0x00, 0x00, 
  0x16, 0xe0, 0x00, 0x00, 
  0x1d, 0xc0, 0x00, 0x00, 
  0x0f, 0xdf, 0xe0, 0x00, 
  0x0f, 0xff, 0xf0, 0x00, 
  0x08, 0xff, 0xf8, 0x00, 
  0x18, 0x7f, 0xfc, 0x00, 
  0x08, 0x7f, 0xfc, 0x00, 
  0x0c, 0x3f, 0xfe, 0x00, 
  0x06, 0x32, 0xff, 0x00, 
  0x03, 0xf0, 0x7f, 0x00, 
  0x03, 0x7f, 0x9b, 0x80, 
  0x07, 0x70, 0x33, 0x80, 
  0x0f, 0xf0, 0x39, 0x80, 
  0x06, 0xe0, 0x38, 0x80,
};

int rabbitWidth=30;
int rabbitHeight=31;
const unsigned char PROGMEM rabbitBitmap[] = { 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xc1, 0xc0, 
  0x00, 0x03, 0x32, 0x20, 
  0x00, 0x03, 0x32, 0x20, 
  0x00, 0x03, 0x32, 0x20, 
  0x00, 0x03, 0x32, 0x20, 
  0x00, 0x03, 0x32, 0x20, 
  0x00, 0x03, 0x32, 0x20, 
  0x00, 0x03, 0x3f, 0xa0, 
  0x00, 0x03, 0x3f, 0xa0, 
  0x00, 0x03, 0x00, 0x20, 
  0x00, 0x04, 0x00, 0x18, 
  0x00, 0x04, 0x00, 0x18, 
  0x00, 0x04, 0x08, 0x58, 
  0x00, 0x04, 0x08, 0x58, 
  0x00, 0x04, 0x08, 0x58, 
  0x00, 0x3c, 0x08, 0x58, 
  0x00, 0x64, 0x00, 0x18, 
  0x00, 0x44, 0x00, 0x18, 
  0x03, 0x83, 0x00, 0x20, 
  0x06, 0x80, 0x7f, 0xc0, 
  0x06, 0x80, 0x03, 0x00, 
  0x03, 0x80, 0x02, 0x00, 
  0x01, 0x80, 0x02, 0x00, 
  0x00, 0x80, 0x32, 0x00, 
  0x00, 0x8f, 0xb2, 0x00, 
  0x00, 0x9b, 0x3a, 0x00, 
  0x00, 0xbb, 0x3a, 0x00, 
  0x00, 0x70, 0xce, 0x00, 
};

// Structure pour stocker les informations sur chaque bitmap
struct BitmapInfo {
  const unsigned char* bitmap;
  int width;
  int height;
};

int bitmapIndex=0;

// Tableau de 54 aleatoire
BitmapInfo visuels[] = {
  {rabbitBitmap, rabbitWidth, rabbitHeight},
  {beletteBitmap, beletteWidth, beletteHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {manBitmap, manWidth, manHeight},
  {mamieBitmap, mamieWidth, mamieHeight},
  {bonusBitmap, bonusWidth, bonusHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {mamieBitmap, mamieWidth, mamieHeight},
  {beletteBitmap, beletteWidth, beletteHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {manBitmap, manWidth, manHeight},
  {beletteBitmap, beletteWidth, beletteHeight},
  {rabbitBitmap, rabbitWidth, rabbitHeight},
  {bonusBitmap, bonusWidth, bonusHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {manBitmap, manWidth, manHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {beletteBitmap, beletteWidth, beletteHeight},
  {mamieBitmap, mamieWidth, mamieHeight},
  {manBitmap, manWidth, manHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {bonusBitmap, bonusWidth, bonusHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {mamieBitmap, mamieWidth, mamieHeight},
  {beletteBitmap, beletteWidth, beletteHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {manBitmap, manWidth, manHeight},
  {beletteBitmap, beletteWidth, beletteHeight},
  {rabbitBitmap, rabbitWidth, rabbitHeight},
  {bonusBitmap, bonusWidth, bonusHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {manBitmap, manWidth, manHeight},
  {rabbitBitmap, rabbitWidth, rabbitHeight},
  {beletteBitmap, beletteWidth, beletteHeight},
  {rabbitBitmap, rabbitWidth, rabbitHeight},
  {mamieBitmap, mamieWidth, mamieHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {mamieBitmap, mamieWidth, mamieHeight},
  {bonusBitmap, bonusWidth, bonusHeight},
  {manBitmap, manWidth, manHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {mamieBitmap, mamieWidth, mamieHeight},
  {beletteBitmap, beletteWidth, beletteHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {manBitmap, manWidth, manHeight},
  {beletteBitmap, beletteWidth, beletteHeight},
  {rabbitBitmap, rabbitWidth, rabbitHeight},
  {bonusBitmap, bonusWidth, bonusHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {mamieBitmap, mamieWidth, mamieHeight},
  {manBitmap, manWidth, manHeight},
  {manBitmap, manWidth, manHeight},
  {giraffeBitmap, giraffeWidth, giraffeHeight},
  {beletteBitmap, beletteWidth, beletteHeight},
  
};


// Positions et mouvements
int cross1X = SCREEN_WIDTH / 4;
int cross1Y = SCREEN_HEIGHT ;
int cross2X = SCREEN_WIDTH * 3 / 4;
int cross2Y = SCREEN_HEIGHT ;
int radius = 5;
int bulletSpeed = 5;
bool shoot1 = false;
bool shoot2 = false;
int bullet1Y = SCREEN_HEIGHT;
int bullet2Y = SCREEN_HEIGHT;
int scorePlayer1 = 0;
int scorePlayer2 = 0;

struct Circle {
  int x;
  int y;
  int direction;  // 1: de gauche à droite, -1: de droite à gauche
  int speed;       // Vitesse du cercle
  const unsigned char* bitmap;
  int bitmapWidth;
  int bitmapHeight;
};

Circle targetCircle;


enum Mode { MODE_SERVER, MODE_CLIENT };
Mode currentMode = MODE_SERVER; // Déclaration de la variable currentMode

void setup() {
    Serial.begin(9600);
    Wire.begin(SDA_PIN, SCL_PIN);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Appuyez sur start");
    Serial.println("SETUP");

    // Attendre l'appui sur 'Start'
    char key = 0;
    while (key != 'X') {
        key = keypad.getKey();
        delay(100); // Petite pause pour éviter la lecture en boucle trop rapide
    }
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);

  bool modeSelected = false;
  while (!modeSelected) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Choisir mode:");
    display.println(currentMode == MODE_SERVER ? "> Serveur" : "  Serveur");
    display.println(currentMode == MODE_CLIENT ? "> Client" : "  Client");
    display.display();

    char key = keypad.getKey();
    if (key == 'U') {
      currentMode = MODE_SERVER;
    } else if (key == 'D') {
      currentMode = MODE_CLIENT;
    } else if (key == 'A') {
      modeSelected = true;
    }
    delay(100);
  }

  if (currentMode == MODE_SERVER) {
    // Configurer le point d'accès SoftAP
    WiFi.softAP(softAP_ssid, softAP_password);
    Serial.println("SoftAP démarré");
    IPAddress IP = WiFi.softAPIP();
    Serial.print("IP du SoftAP: ");
    Serial.println(IP);
    display.println("SoftAP OK");

    udp.begin(localPort);
    Serial.printf("UDP server : %s:%i \n", WiFi.localIP().toString().c_str(), localPort);


  } else if (currentMode == MODE_CLIENT) {
    WiFi.begin(softAP_ssid, softAP_password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    // Enregistrer l'adresse IP du serveur
    serverIP = WiFi.gatewayIP(); // Utilisez gatewayIP() car en mode SoftAP, le serveur agit en tant que passerelle
    Serial.println("WiFi connecté en tant que client");
    display.println("Client WiFi OK");
    udp.begin(localPort);
    Serial.printf("UDP Client : %s:%i \n", WiFi.localIP().toString().c_str(), localPort);

  }

    display.clearDisplay();

    display.setCursor(16, 52); // Laisser un espace de 10 pixels avant l'affichage de l'IP
    display.print("IP: ");
    display.println(WiFi.localIP());


    display.display();

    delay(3000); // Afficher l'adresse IP et l'image pendant 2 secondes

      // Initialiser le premier cercle
    targetCircle.x = 0;
    targetCircle.y = (SCREEN_HEIGHT /2);
    targetCircle.direction = 1;
    targetCircle.speed = 2;  
    targetCircle.bitmap = visuels[bitmapIndex].bitmap;
    targetCircle.bitmapWidth = visuels[bitmapIndex].width;
    targetCircle.bitmapHeight = visuels[bitmapIndex].height;

    
}

void loop() {
  char key = keypad.getKey();
  handleKeypadInput(key,currentMode);
  moveCircles();
  moveBullets();

  String message="";

  if (currentMode == MODE_SERVER) {
    // envoie et reception des données en mode serveur 
    message=(String(shoot1) + " " + String(scorePlayer1) +" " + String(targetCircle.x) +" " + String(targetCircle.y) +" " + String(targetCircle.direction) +" " + String(targetCircle.speed) +" " + String(bitmapIndex) + "\n");
    server(message);

  }
  else{
    //envoie et receptioin des données en mode client 
    message=(String(shoot2) + " " + String(scorePlayer2)+ "\n");
    client(message, serverIP, localPort);
    
  }
  
  checkBulletCollision(currentMode);
  updateDisplay();
  delay(20);
}
void handleKeypadInput(char key,Mode currentMode) {
  if (key) {
    if(currentMode == MODE_SERVER){
      switch (key) {
      case 'B': shoot1 = true; break;  // Pressing A triggers shooting from cross1
      case 'A': shoot1 = true; break;  // Pressing B triggers shooting from cross2
      default: break;
      }
    }
    if(currentMode == MODE_CLIENT){
      switch (key) {
      case 'B': shoot2 = true; break;  // Pressing A triggers shooting from cross1
      case 'A': shoot2 = true; break;  // Pressing B triggers shooting from cross2
      default: break;
      }
    }   
  }
}
void moveCircles() {
  // Move circles from left to right or right to left
  targetCircle.x += targetCircle.direction * targetCircle.speed;
  if ((targetCircle.x > SCREEN_WIDTH && targetCircle.direction == 1) || (targetCircle.x < 0 && targetCircle.direction == -1)) {
    // Regénérer un nouveau cercle aléatoire
    if(currentMode == MODE_SERVER){
      Regeneratecircle();
    }
    
  }
}
void Regeneratecircle() {
  // Générer une nouvelle position y aléatoire
  targetCircle.x = targetCircle.direction == 1 ? 0 : SCREEN_WIDTH;
  targetCircle.y = random(SCREEN_HEIGHT / 4, SCREEN_HEIGHT * 3 / 4);
  targetCircle.direction = random(0, 2) == 0 ? -1 : 1;
  targetCircle.speed = random(2, 5);  // Vous pouvez ajuster la plage de vitesse selon vos besoins
  // Attribuer les propriétés du bitmap sélectionné
  targetCircle.bitmap = visuels[bitmapIndex].bitmap;
  targetCircle.bitmapWidth = visuels[bitmapIndex].width;
  targetCircle.bitmapHeight = visuels[bitmapIndex].height;
  bitmapIndex = (bitmapIndex + 1) % 54;
}
void moveBullets() {
  // Move bullets upward
  if (shoot1) {
    bullet1Y -= bulletSpeed;
    if (bullet1Y < 0) {
      bullet1Y = SCREEN_HEIGHT;
      shoot1 = false;
    }
  }

  if (shoot2) {
    bullet2Y -= bulletSpeed;
    if (bullet2Y < 0) {
      bullet2Y = SCREEN_HEIGHT;
      shoot2 = false;
    }
  }
}
void client (const String message, IPAddress serverIP, unsigned int localPort) {
 	int packetSize = udp.parsePacket();
 	//Serial.print(" Received packet from : "); Serial.println(udp.remoteIP());
 	//Serial.print(" Size : "); Serial.println(packetSize);
 	if (packetSize) {
 			int len = udp.read(packetBuffer, 255);
 			if (len > 0) packetBuffer[len - 1] = 0;
      int receivedShoot1, receivedScorePlayer1, receivedX, receivedY, receivedDirection, receivedSpeed, receivedBitmapIndex;
      if(sscanf(packetBuffer, "%d %d %d %d %d %d %d %d", &receivedShoot1, &receivedScorePlayer1, &receivedX, &receivedY, &receivedDirection, &receivedSpeed, &receivedBitmapIndex) == 8) {
        shoot1 = (receivedShoot1 == 1); 
        scorePlayer1=receivedScorePlayer1;
        targetCircle.x=receivedX;
        targetCircle.y= receivedY;
        targetCircle.direction= receivedDirection;
        targetCircle.speed= receivedSpeed;
        bitmapIndex= receivedBitmapIndex;
        Serial.println("Valeurs CLIENT recues ");
    }
 	}
 	Serial.println("\n");
 	delay(10);
 	//Serial.print("[Client Connected] ");
 	//Serial.println(WiFi.localIP());
 	udp.beginPacket(serverIP, localPort);
 	udp.printf(message.c_str());
 	udp.printf("\r\n");
 	udp.endPacket();
}
void server(const String message) {
 	int packetSize = udp.parsePacket();
 	//Serial.print(" Received packet from : "); Serial.println(udp.remoteIP());
 	//Serial.print(" Size : "); Serial.println(packetSize);
 	if (packetSize) {
 			int len = udp.read(packetBuffer, 255);
 			if (len > 0) packetBuffer[len - 1] = 0;
      int receivedShoot2, receivedScorePlayer2;
      if(sscanf(packetBuffer, "%d %d", &receivedShoot2, &receivedScorePlayer2) == 2) {
        shoot2 = receivedShoot2;
        scorePlayer2 = receivedScorePlayer2;
        Serial.println("Valeurs SERVEUR recues  ");
      }
 			//Serial.printf("Data : %s\n", packetBuffer);
 			udp.beginPacket(udp.remoteIP(), udp.remotePort());
 			udp.printf(message.c_str());
      udp.printf("\r\n");
 			udp.endPacket();
 	}
 	Serial.println("\n");
 	delay(10);
 	//Serial.print("[Server Connected] ");
 	//Serial.println (WiFi.localIP());
}
void checkBulletCollision(Mode currentMode) {
  // Vérifier la collision avec le cercle cible
  if (bullet1Y <= targetCircle.y + radius && bullet1Y >= targetCircle.y - radius &&
      abs(cross1X - targetCircle.x) <= radius) {
    handleCollision(targetCircle,1);
    if(currentMode == MODE_SERVER){
      Regeneratecircle();
    }
  }

  if (bullet2Y <= targetCircle.y + radius && bullet2Y >= targetCircle.y - radius &&
      abs(cross2X - targetCircle.x) <= radius) {
    handleCollision(targetCircle,2);
    if(currentMode == MODE_SERVER){
      Regeneratecircle();
    }
  }
}
void handleCollision(Circle& circle,int joueur) {
  if(circle.bitmap==giraffeBitmap || circle.bitmap==beletteBitmap ||circle.bitmap==rabbitBitmap)
  {
    if(joueur==1){
      scorePlayer1--;
    }
    if(joueur==2){
      scorePlayer2--;
    }

  }
  if(circle.bitmap==manBitmap || circle.bitmap==mamieBitmap)
  {
    if(joueur==1){
      scorePlayer1++;
    }
    if(joueur==2){
      scorePlayer2++;
    }

  }
}
void updateDisplay() {
  display.clearDisplay();

  // Afficher les scores
  display.setCursor(0, 0);
  display.print("P1: ");
  display.print(scorePlayer1);
  display.setCursor(SCREEN_WIDTH *2/3, 0);
  display.print("P2: ");
  display.print(scorePlayer2);
  display.setTextWrap(true);

  // // Draw crosses at the bottom left and bottom right
  display.drawLine(cross1X - radius, cross1Y - radius, cross1X + radius, cross1Y + radius, SSD1306_WHITE);
  display.drawLine(cross1X - radius, cross1Y + radius, cross1X + radius, cross1Y - radius, SSD1306_WHITE);
  display.drawLine(cross2X - radius, cross2Y - radius, cross2X + radius, cross2Y + radius, SSD1306_WHITE);
  display.drawLine(cross2X - radius, cross2Y + radius, cross2X + radius, cross2Y - radius, SSD1306_WHITE);

  // Draw bullets 
  display.drawCircle(cross1X, bullet1Y, 2, SSD1306_WHITE);
  display.drawCircle(cross2X, bullet2Y, 2, SSD1306_WHITE);
  display.drawBitmap(targetCircle.x - (targetCircle.bitmapWidth / 2), targetCircle.y - (targetCircle.bitmapHeight / 2), targetCircle.bitmap, targetCircle.bitmapWidth, targetCircle.bitmapHeight, SSD1306_WHITE);

  display.display();
}