/*
  
   ESP 3.3V > GPS VCC
   ESP GND  > GPS GND
   ESP RX   > GPS TX
   ESP TX   > GPS RX
  
*/

#include <TinyGPS++.h> // Library über http://arduiniana.org/libraries/tinygpsplus/ downloaden und installieren
#include<SoftwareSerial.h>
// Load Wi-Fi library
#include <ESP8266WiFi.h>
SoftwareSerial Serial2; // The serial connection to the GPS device

/*
  Next, the program object for the GPS is created, we call
    the variable that points to it simply "gps"
*/
TinyGPSPlus gps;

/*
   Now we have to create a serial connection to the GPS module
    ESP32 supports up to 3 hardware serial connections. Therefore
    we also do not need to use the software serial library.
    The number in parenthesis is the uart_nr. This will be the three possible
    Distinguished connections. For ESP32 this value can be 0, 1 or 2
*/

//IPAddress ip(192,168,1,102);//static IP
char server1[] = "api.thingspeak.com";
String apiKey = "GEHY85X73M1TXBF7";     //  Enter your Write API key from ThingSpeak
int device1= 1; 

/*
 Next, we create a template object where we get all the data
    in a variable read by the GPS module
    Afterwards we create a new variable, "gpsState"
*/
struct GpsDataState_t {
  double originLat = 0;
  double originLon = 0;
  double originAlt = 0;
  double distMax = 0;
  double dist = 0;
  double altMax = -999999;
  double altMin = 999999;
  double spdMax = 0;
  double prevDist = 0;
};
GpsDataState_t gpsState = {};

/*
   The following constant defines the output speed
    in the serial monitor. This is specified in milliseconds.
    Including the associated variables to achieve this restriction
*/
#define TASK_SERIAL_RATE 1000 // ms
uint32_t nextSerialTaskTs = 0;
uint32_t nextOledTaskTs = 0;
// Replace with your network credentials
char ssid[] = "istudiofast";            // your network SSID (name)
char pass[] = "ist123456*";        // your network password
//const char* ssid     = "ESP32-Access-Point";
//const char* pass = "123456789";

// Set web server port number to 80
WiFiServer server(80);
WiFiClient client;

// Variable to store the HTTP request
String header;
void setup() {

  // Serial ist die Ausgabe im Serial Monitor
  Serial.begin(115200);
 //Serial.swap(); // Swap to Alternate pins -> EQG on "Serial" (RxD-nn, TxD-nn)

Serial2.begin(9600,13,15);

  pinMode(2, OUTPUT);
// Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 // Connect to Wi-Fi network with SSID and password
 // Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
 // WiFi.softAP(ssid, password);

  
  
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  //Serial.println(ip);
  Serial.println(WiFi.localIP());
  //IPAddress IP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(IP);
  server.begin();
  /*
     The connection with the GPS module. We
       void begin (unsigned long baud, uint32_t config = SERIAL_8N1, int8_t rxPin = -1, int8_t txPin = -1, bool invert = false, unsigned long timeout_ms = 20000UL);
       baud: baudrate according to the GPS module specification, in this case 9600
       config: default value
       rxPin: an RX pin e.g. 16
       txPin: an RX pin e.g. 17
  */

 
}

void loop() {
 WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
  digitalWrite(2, LOW);
 String request = client.readStringUntil('\r');
 Serial.println("********************************");
 Serial.println("From the user: " + request);
 //client.flush();
 Serial.print("Byte sent to the user: ");
   
   if(request == "PANIC/1"){
 Serial.println(client.println("REQUESTRECEIVED\r"));
 digitalWrite(2, HIGH);
 Serial.println("akc sent success to client");
 /*if (nextSerialTaskTs < millis()) {
    Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
    Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
    Serial.print("ALT=");  Serial.println(gps.altitude.meters());
    Serial.print("Sats=");  Serial.println(gps.satellites.value());
    Serial.print("DST: ");
    Serial.println(gpsState.dist, 1);
    nextSerialTaskTs = millis() + TASK_SERIAL_RATE;
   
 }*/
 delay(3000);
  Device1();
 }
 
  else{
    Serial.println("no ack sent");
    }
    
  //delay(5000);

  }

  
  //else{
     //   Serial.println("N0 Client.");          // print a message out in the serial port
//while(0);
 //   }
   // delay(500);
  static int p0 = 0;

  // GPS Koordinaten von Modul lesen
  gpsState.originLat = gps.location.lat();
  gpsState.originLon = gps.location.lng();
  gpsState.originAlt = gps.altitude.meters();

  //Aktuelle Position in nichtflüchtigen ESP32-Speicher schreiben
 long writeValue;
  writeValue = gpsState.originLat * 1000000;
//  EEPROM_writeAnything(0, writeValue);
  writeValue = gpsState.originLon * 1000000;
  //EEPROM_writeAnything(4, writeValue);
  writeValue = gpsState.originAlt * 1000000;
 // EEPROM_writeAnything(8, writeValue);
 // EEPROM.commit(); // erst mit commit() werden die Daten geschrieben

  gpsState.distMax = 0;
  gpsState.altMax = -999999;
  gpsState.spdMax = 0;
  gpsState.altMin = 999999;

  /*
   * Rohdaten von Serieller Verbndung zum GPS-Modul
   * einlesen. Die Daten werden mittels TinyGPS++ verarbeitet
   * Die Daten werden bewusst erst nach der Zuweisung der Variablen
   * gelesen, damit wir noch im nachfolgenden vereinfacht 
   * Berechnungen anstellen können.
   */
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }

  /*
   * Diverse Berechnungen von Maximum und Minimum-Werten und zurückgelegter Distanz
   * Diese werden aber erst gemacht, wenn mindestens ein Fix mit 4 Satelliten vorhanden
   * ist, allenfalls wäre die Genauigkeit nicht gegeben und es würden falsche
   * Werte berechnet werden.
   */
  if (gps.satellites.value() > 4) {
    gpsState.dist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), gpsState.originLat, gpsState.originLon);

    if (gpsState.dist > gpsState.distMax && abs(gpsState.prevDist - gpsState.dist) < 50) {
      gpsState.distMax = gpsState.dist;
    }
    gpsState.prevDist = gpsState.dist;

    if (gps.altitude.meters() > gpsState.altMax) {
      gpsState.altMax = gps.altitude.meters();
    }

    if (gps.speed.kmph() > gpsState.spdMax) {
      gpsState.spdMax = gps.speed.kmph();
    }

    if (gps.altitude.meters() < gpsState.altMin) {
      gpsState.altMin = gps.altitude.meters();
    }
  }

  /*
     Damit nicht zu viele Daten im Serial Monitor ausgegeben werden,
     beschränken wir die Ausgabe auf die Anzahl Millisekunden
     die wir in der Konstante "TASK_SERIAL_RATE" gespeichert haben
  */
  /*if (nextSerialTaskTs < millis()) {
    Serial.print("LAT=");  Serial.println(gps.location.lat(), 6);
    Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
    Serial.print("ALT=");  Serial.println(gps.altitude.meters());
    Serial.print("Sats=");  Serial.println(gps.satellites.value());
    Serial.print("DST: ");
    Serial.println(gpsState.dist, 1);
    nextSerialTaskTs = millis() + TASK_SERIAL_RATE;
       
  }
*/

}
void Device1()
{
   Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server1, 80)) {
    Serial.println("Connected to thingspeak server");
    // Make a HTTP request
    //client.print("GET https://api.thingspeak.com/update?api_key=GEHY85X73M1TXBF7&field1=");
    //client.println(gps.location.lat(), 6);
  // client.println();
   String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(gps.location.lat(), 6);
                             postStr +="&field2=";
                             postStr += String(gps.location.lng(), 6);
                             postStr += "\r\n\r\n";
                             postStr +="&field3=";
                             postStr += String(gps.satellites.value());
                            // postStr +="&field4=";
                            // postStr += String(gpsState.dist, 1);
                             postStr +="&field5=";
                             postStr += String(device1); 
  

                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
  }
  
  }
