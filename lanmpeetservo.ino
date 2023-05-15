#include <ESP8266WiFi.h>
#include <Servo.h>
Servo servo;
 
const char* ssid = "YOUR WIFI";
const char* password = "YOUR PASS";  

WiFiServer server(80);

String output5State = "off";
String output4State = "off";

const int output5 = 5;
const int output4 = 4;

void setup() {
  Serial.begin(115200);
  delay(10);
  servo.attach(2); //Gpio-2 of nodemcu with pwm pin of servo motor
  
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);

  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");    //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

 int value = 0;
 // Match the request
 if (request.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (request.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            } else if (request.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 4 on");
              output4State = "on";
              digitalWrite(output4, HIGH);
            } else if (request.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 4 off");
              output4State = "off";
              digitalWrite(output4, LOW);
            }

if (request.indexOf("/Req=0") != -1)  {
  servo.write(0); //Moving servo to 0 degree

}
if (request.indexOf("/Req=90") != -1)  {  
  servo.write(90); //Moving servo to 90 degree
  
}


  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<h1 align=center>welcome to your home's control</h1><br><br>");
  client.println("<br><br>");
  client.println("<h2>Control your home's door</2>");
  client.println("<p><a href=\"/Req=0\"\"><button class=\"button\">Open the door</button></a></p>");
  client.println("<p><a href=\"/Req=90\"\"><button class=\"button\">Close the door</button></a></p><br>");
 client.println("<style>html { font-family: Helvetica;background-color:blue; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: black; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: black;}</style></head>");
            
 client.println("<p>Room's light - state " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p><br>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p><br>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 4  
            client.println("<p>kitchen's light - state " + output4State + "</p>");
            // If the output4State is off, it displays the ON button       
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p><br>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p><br>");
            }

  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
