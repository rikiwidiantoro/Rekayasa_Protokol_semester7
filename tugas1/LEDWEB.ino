/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "Realmi7";
const char* password = "imu99999";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

String led_list[8] = {"lampu_jalan_1", "lampu_jalan_2", "lampu_jalan_3", "lampu_jalan_4", "lampu_jalan_5", "lampu_jalan_6", "lampu_jalan_7", "lampu_jalan_8"};
String led_state[8];

bool running_right = false;

// Assign output variables to GPIO pins
#define lampu_jalan_1 D0
#define led_1 D1
#define led_2 D2
#define led_3 D4
#define led_4 D5
#define led_5 D6
#define led_6 D7
#define led_7 D8

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(lampu_jalan_1, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);
  pinMode(led_5, OUTPUT);
  pinMode(led_6, OUTPUT);
  pinMode(led_7, OUTPUT);
  
  digitalWrite(lampu_jalan_1, LOW);
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);
  digitalWrite(led_4, LOW);
  digitalWrite(led_5, LOW);
  digitalWrite(led_6, LOW);
  digitalWrite(led_7, LOW);

  for(int i=0;i < 8;i++){
    led_state[i] = "off";
  }
  

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
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            for(int i=0;i < 8;i++){
              if (header.indexOf("GET /" + led_list[i] + "/on") >= 0) {
                Serial.println(led_list[i] + " on");
                led_state[i] = "on";
              } else if (header.indexOf("GET /" + led_list[i] + "/off") >= 0) {
                Serial.println(led_list[i] + " off");
                led_state[i] = "off";
              }
            }

            if (header.indexOf("GET /running/on") >= 0) {
              running_right = true;
            } else if (header.indexOf("GET /running/off") >= 0) {
              running_right = false;
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #FF0000; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1><h1>LAMPU JALAN DESA</h1>");

            for(int i=0;i < 8;i++){
              // Display current state, and ON/OFF buttons for GPIO 27  
              client.println("<p>" + led_list[i] + " [" + led_state[i] + "] " + "</p>");
              // If the output27State is off, it displays the ON button       
              if (led_state[i] == "off") {
                client.println("<p><a href=\"/" + led_list[i] + "/on\"><button class=\"button button2\">OFF</button></a></p>");
              } else {
                client.println("<p><a href=\"/" + led_list[i] + "/off\"><button class=\"button\">ON</button></a></p>");
              }
            }

//            client.println("<p>Loading - State</p>");
//            if (!running_right) {
//              client.println("<p><a href=\"/running/on\"><button class=\"button button2\">OFF</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/running/off\"><button class=\"button\">ON</button></a></p>");
//            }
            
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  if(running_right){
    for(int i=0;i < 6;i++){
      led_state[i] = "on";
//      switch_led();
      delay(75);
    }
    for(int i=5;i >= 0;i-=1){
      led_state[i] = "off";
//      switch_led();
      delay(75);
    }
  }

  if(led_state[0] == "on"){
    digitalWrite(lampu_jalan_1, HIGH);
  }
  else{
    digitalWrite(lampu_jalan_1, LOW);
  }

  if(led_state[1] == "on"){
    digitalWrite(led_1, HIGH);
  }
  else{
    digitalWrite(led_1, LOW);
  }

  if(led_state[2] == "on"){
    digitalWrite(led_2, HIGH);
  }
  else{
    digitalWrite(led_2, LOW);
  }

  if(led_state[3] == "on"){
    digitalWrite(led_3, HIGH);
  }
  else{
    digitalWrite(led_3, LOW);
  }

  if(led_state[4] == "on"){
    digitalWrite(led_4, HIGH);
  }
  else{
    digitalWrite(led_4, LOW);
  }

  if(led_state[5] == "on"){
    digitalWrite(led_5, HIGH);
  }
  else{
    digitalWrite(led_5, LOW);
  }
  if(led_state[6] == "on"){
    digitalWrite(led_6, HIGH);
  }
  else{
    digitalWrite(led_6, LOW);
  }
  if(led_state[7] == "on"){
    digitalWrite(led_7, HIGH);
    led_state[0] == "on";
    digitalWrite(lampu_jalan_1, HIGH);
    led_state[1] == "on";
    digitalWrite(led_1, HIGH);
    led_state[2] == "on";
    digitalWrite(led_2, HIGH);
    led_state[3] == "on";
    digitalWrite(led_3, HIGH);
    led_state[4] == "on";
    digitalWrite(led_4, HIGH);
    led_state[5] == "on";
    digitalWrite(led_5, HIGH);
    led_state[6] == "on";
    digitalWrite(led_6, HIGH);
  }
  else{
    digitalWrite(led_7, LOW);
  }
}

//void switch_led(){
//  if(led_state[0] == "on"){
//    digitalWrite(led_0, HIGH);
//  }
//  else{
//    digitalWrite(led_0, LOW);
//  }
//
//  if(led_state[1] == "on"){
//    digitalWrite(led_1, HIGH);
//  }
//  else{
//    digitalWrite(led_1, LOW);
//  }
//
//  if(led_state[2] == "on"){
//    digitalWrite(led_2, HIGH);
//  }
//  else{
//    digitalWrite(led_2, LOW);
//  }
//
//  if(led_state[3] == "on"){
//    digitalWrite(led_3, HIGH);
//  }
//  else{
//    digitalWrite(led_3, LOW);
//  }
//
//  if(led_state[4] == "on"){
//    digitalWrite(led_4, HIGH);
//  }
//  else{
//    digitalWrite(led_4, LOW);
//  }
//
//  if(led_state[5] == "on"){
//    digitalWrite(led_5, HIGH);
//  }
//  else{
//    digitalWrite(led_5, LOW);
//  }
//}
