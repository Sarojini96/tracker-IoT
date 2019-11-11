 

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(9, 8); // RX, TX
#endif

char ssid[] = "istudiofast";            // your network SSID (name)
char pass[] = "ist123456*";        // your network password
//const char* ssid     = "ESP32-Access-Point";
//const char* pass = "123456789";
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int onstatus = 1;
int offstatus = 0;
//IPAddress ip(192,168,1,11);//static IP
char server[] = "192.168.1.101";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 2000L; // delay between updates, in milliseconds

// Initialize the Ethernet client object
WiFiEspClient client;
RingBuffer buf(8);

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  pinMode(5, OUTPUT); //GREEN ON
pinMode(13,OUTPUT);
  pinMode(7, OUTPUT); //GREEN ON
  pinMode(6, OUTPUT); //RED  PROBLEM
  pinMode(2,INPUT_PULLUP);//switch
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");

 printWifiStatus();

}

void loop()
{
      digitalWrite(13, HIGH);

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
   httpRequest();
   String answer = client.readStringUntil('\r');
  if(answer == "REQUESTRECEIVED"){
        Serial.println("From Station " + answer);
      digitalWrite(5,LOW);
    digitalWrite(13, HIGH);
    }
    
  }
    
  }


// this method makes a HTTP connection to the server
void httpRequest()
{
  int button = digitalRead(2);
    Serial.println();
   client.flush();
  client.stop();
  // if there's a successful connection
 if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    
    if (button == 0) 
    {
    
   client.connect(server, 80);
      Serial.println("Connecting to station for sending panic ALERT...");
      
      client.print("PANIC/1");
      digitalWrite(5,HIGH);
          digitalWrite(13, LOW);

       String answer = client.readStringUntil('\r');
    Serial.println("From Station " + answer);
  if(answer == "REQUESTRECEIVED"){
    digitalWrite(5,LOW);
    digitalWrite(13, HIGH);
    }
      
 }
     // note the time that the connection was made
    lastConnectionTime = millis();
    //} 
  }
  
}
void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
 IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
 
