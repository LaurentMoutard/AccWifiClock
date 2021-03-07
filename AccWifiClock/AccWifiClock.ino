// ESP32 Dev Module

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
char ssid[] = "YourBoxSSID";
char pass[] = "YourPass";

// En utilisant le ST7735S TFT SPI LCD 128×160 1.8"
//3.3V<-> LED
//G18 <-> SCK
//G23 <-> SDA
//G2  <-> A0 (DC)
//G14 <-> RESET
//G17 <-> CS
//GND <-> GND
//5 V <-> VCC

//Généralement dans le dossier C:\Users\Your_Name\Documents\Arduino\libraries\TFT_eSPI\User_Setup.h
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip  128 x 160
#include <SPI.h>

// Une méthode pour charger de très nombreuses polices livrées avec TFT_eSPI sur cet afficheur que je laisse en commentaire en cas d'intérêt
// se reporter au fichier Free_Fonts.h associé
//#include "Free_Fonts.h" // Include the header file attached to this sketch

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h


String serverName = "YouURLPage";
unsigned long previousMillis = 0;
unsigned long previousSecondesMillis = 0;

int requestInterval = 600000; // 10 mn



String Time, Sec;
int switchM = 1;
int myH = 0;
int myM = 0;
int myS = 0;


String jsonFromServer, jsonToServer;

String getFromServer() {
  if (WiFi.status()== WL_CONNECTED) {
    
    HTTPClient http;
    
    Serial.println("[HTTP] begin...");
    http.begin(serverName+"get.php");
    
    Serial.println("[HTTP] GET...");
    // démarrer la connexion et envoyer les entêtes HTTP
    int httpCode = http.GET();
    
    // httpCode sera négatif si on rencontre une erreur
    if (httpCode > 0) {
      // les entêtes HTTP ont été envoyés et
      
      // si le serveur TOM répond par OK
      if (httpCode == HTTP_CODE_OK) {
        // alors on récupère la réponse sous forme de chaîne de caractères
        String payload = http.getString();
        //    Serial.println("[HTTP] GET... payload:");
        //    Serial.println(payload);
        return payload;
      }
    }
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      return ("");
    }
    http.end();
  }else{
    Serial.print("DECONNECTE");
    ESP.restart();
  }
}

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  Serial.println ("OK");
  
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Connected to wifi");
  printWifiStatus();

//  Wire.begin();
//  u8g2.begin();

  
  tft.init();
//  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

//  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background colour

  

}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis < 5000 || currentMillis - previousMillis > requestInterval) {
    previousMillis = currentMillis;   
    processServer();
    //tft.fillScreen(TFT_BLACK);
    for(int ye = 123; ye <= 125; ye++){
      tft.drawLine(0, ye, TFT_HEIGHT, ye, TFT_BLACK);
    }
   }
  currentMillis = millis();
  if(currentMillis - previousSecondesMillis >= 1000) {
    previousSecondesMillis = currentMillis;   
  
    myS++;
    for(int ye = 119; ye <= 121; ye++){
      tft.drawLine(0, ye, TFT_HEIGHT, ye, TFT_BLACK);
    }
   if(myS==60){
    switchM = 1;
    myS=0;
    myM=myM+1;
   }
   if(myM==60)
   {
    myM=0;
    myH=myH+1;
   }
   if(myH==25)
   {
    myH=1;
   }

  if(switchM == 1){
    switchM = 0;
    Time = getDigits(myH)+":"+getDigits(myM);//;+":"+getDigits(myS);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawCentreString(Time,80,5,7);
  }

    Sec = getDigits(myS);
    tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    tft.drawCentreString(Sec,80,60,7);
    int32_t xe = map(currentMillis - previousMillis, 0, requestInterval, 0, TFT_HEIGHT);
    for(int ye = 123; ye <= 125; ye++){
      tft.drawLine(0, ye, xe, ye, TFT_RED);
    }
   }

  int32_t xe = map(currentMillis - previousSecondesMillis, 0, 1000, 0, TFT_HEIGHT);
  for(int ye = 119; ye <= 121; ye++){
    tft.drawLine(0, ye, xe, ye, TFT_RED);
  }
//    tft.setTextColor(TFT_WHITE);
//    tft.setFreeFont(&Orbitron_Light_32); // En cas d'utilisation de Free_Fonts
//    tft.setFreeFont(&Yellowtail_32); // En cas d'utilisation de Free_Fonts
//    tft.drawCentreString(Time,80,20,GFXFF);
//    tft.setTextColor(TFT_SILVER);
//    tft.setFreeFont(FF6);
//    tft.drawCentreString(Sec,80,80,GFXFF);
  
  

  
    //oledPrint(Time);
  
  //// Default color definitions
  //#define TFT_BLACK       0x0000      /*   0,   0,   0 */
  //#define TFT_NAVY        0x000F      /*   0,   0, 128 */
  //#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
  //#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
  //#define TFT_MAROON      0x7800      /* 128,   0,   0 */
  //#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
  //#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
  //#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
  //#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
  //#define TFT_BLUE        0x001F      /*   0,   0, 255 */
  //#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
  //#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
  //#define TFT_RED         0xF800      /* 255,   0,   0 */
  //#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
  //#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
  //#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
  //#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
  //#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
  //#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F      
  //#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
  //#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
  //#define TFT_SILVER      0xC618      /* 192, 192, 192 */
  //#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
  //#define TFT_VIOLET      0x915C      /* 180,  46, 226 */
  



 }

String getDigits(int digits){
 // utility function for digital clock display: prints colon and leading 0
 String Str = String(digits);
 if(digits < 10){
  Str = '0'+ Str;
 }
  return Str;
}
void processServer(){
    jsonFromServer = getFromServer();
    Serial.print("json récupéré : ");
    Serial.println(jsonFromServer);
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonFromServer);
    if (error){
      return;
    }
//    myH = doc["myH"].as<String>(); // En cas de besoin de conversionn en String
    myH = doc["myH"];
    myM = doc["myM"];
    myS = doc["myS"];
    
    Serial.print("Heure : ");
    Serial.println(myH);
    Serial.print("Mn : ");
    Serial.println(myM);
//  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

 
