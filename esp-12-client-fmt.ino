/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>

//byte ledPin = D0;
char ssid[] = "istudiofast";            // your network SSID (name)
char pass[] = "ist123456*";        // your network password

unsigned long askTimer = 0;

IPAddress server(192,168,1,101);       // the fix IP address of the server
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(D1,OUTPUT);// RED
    pinMode(D3,OUTPUT); //GREEN
        pinMode(D2,INPUT_PULLUP); // BUTTON


  digitalWrite(D3, HIGH);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void loop () {
      digitalWrite(D3, HIGH);

    int button = digitalRead(D2);
    Serial.print(button);
 if (client.connect(server, 80)) {
   if (button == 0) 
    {
    
   client.connect(server, 80);
      Serial.println("Connecting to station for sending panic ALERT...");
      
      client.print("PANIC/1");
      digitalWrite(D1,HIGH);
          digitalWrite(D3, LOW);

       String answer = client.readStringUntil('\r');
      Serial.println("*********************************************");

    Serial.println("From Station " + answer);
          Serial.println("*********************************************");

  if(answer == "REQUESTRECEIVED"){
    digitalWrite(D1,LOW);
    digitalWrite(D3, HIGH);
    }
      
 }
 }
 // String answer = client.readStringUntil('\r');   // receives the answer from the sever
 // Serial.println("from server: " + answer);
  client.flush();
  //digitalWrite(ledPin, HIGH);
  delay(2000);                  // client will trigger the communication after two seconds
}
