/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid     = "istudiofast";
const char* password = "ist123456*";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
//String output26State = "off";
//String output27State = "off";

// Assign output variables to GPIO pins
//const int output26 = 26;
//const int output27 = 27;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(2, OUTPUT);
  //pinMode(output27, OUTPUT);
  // Set outputs to LOW
 // digitalWrite(output26, LOW);
  //digitalWrite(output27, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
  digitalWrite(2, LOW);
 String request = client.readStringUntil('\r');
 Serial.println("********************************");
 Serial.println("From the user: " + request);
 client.flush();
 Serial.print("Byte sent to the user: ");
   if(request == "PANIC/USER1"){
 Serial.println(client.println("REQUESTRECEIVED"));
 digitalWrite(2, HIGH);
 Serial.println("akc sent success to client");
  }
  else{
    Serial.println("no ack sent");
    }
  }
  else{
        Serial.println("N0 Client.");          // print a message out in the serial port
//while(0);
    }
    delay(500);
}
