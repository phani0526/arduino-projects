#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "ssid";
const char* password = "pass";
IPAddress ip(172, 16, 1, 1);  //config static IP
IPAddress gateway(172, 16, 1, 1);
IPAddress subnet(255, 255, 252, 0);
LiquidCrystal_I2C lcd(0x27, 16, 2); //D1 SCL ; D2 SDA

ESP8266WebServer server(80);
void setup()
{
  Serial.begin(115200);
  pinMode(16, OUTPUT);//D0
  pinMode(5, OUTPUT);//D1
  pinMode(4, OUTPUT);//D2
  pinMode(0, OUTPUT);//D3
  pinMode(2, OUTPUT);//D4
  pinMode(14, OUTPUT);//D5
  pinMode(12, OUTPUT);//D6
  pinMode(13, OUTPUT);//D7 Buzzer
  pinMode(15, OUTPUT);//D8
  digitalWrite(16, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
  digitalWrite(14, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(15, LOW);
  lcd.init();
  lcd.noBacklight();
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
  server.begin(); // Start the server
  alert(1, 2);
  Serial.println();
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
void loop() {

  WiFiClient client = server.available();
  if (!client) {
    return; // Check if a client has connected
  }
  while (!client.available()) {
    delay(1); // Wait until the client sends some data
  }
  String request = client.readStringUntil('\r');  // Read the first line of the request
  client.flush();
  // Match the request & Set ledPin according to the request
  if (request.indexOf("/D1on") > 0)  {
    digitalWrite(16, HIGH);
    alert(1, 1);
  }
  if (request.indexOf("/D1off") > 0)  {
    digitalWrite(16, LOW);
    alert(2, 1);
  }
  if (request.indexOf("/D2on") > 0)  {
    digitalWrite(5, HIGH);
    alert(1, 1);
  }
  if (request.indexOf("/D2off") > 0)  {
    digitalWrite(5, LOW);
    alert(2, 1);
  }
  if (request.indexOf("/D3on") > 0)  {
    digitalWrite(4, HIGH);
    alert(1, 1);
  }
  if (request.indexOf("/D3off") > 0)  {
    digitalWrite(4, LOW);
    alert(2, 1);
  }
  if (request.indexOf("/D4on") > 0)  {
    digitalWrite(0, HIGH);
    alert(1, 1);
  }
  if (request.indexOf("/D4off") > 0)  {
    digitalWrite(0, LOW);
    alert(2, 1);
  }
  if (request.indexOf("/D5on") > 0)  {
    digitalWrite(2, HIGH);
    alert(1, 1);
  }
  if (request.indexOf("/D5off") > 0)  {
    digitalWrite(2, LOW);
    alert(2, 1);
  }
  if (request.indexOf("/D6on") > 0)  {
    digitalWrite(14, HIGH);
    alert(1, 1);
  }
  if (request.indexOf("/D6off") > 0)  {
    digitalWrite(14, LOW);
    alert(2, 1);
  }
  if (request.indexOf("/D7on") > 0)  {
    digitalWrite(12, HIGH);
    alert(1, 1);
  }
  if (request.indexOf("/D7off") > 0)  {
    digitalWrite(12, LOW);
    alert(2, 1);
  }
  if (request.indexOf("/D8on") > 0)  {
    digitalWrite(15, HIGH);
    alert(1, 1);
  }
  if (request.indexOf("/D8off") > 0)  {
    digitalWrite(15, LOW);
    alert(2, 1);
  }
  if (request.indexOf("/reset") > 0)  {
    reset();
  }
  if (request.indexOf("/display") > 0)  {
    lcd.backlight();
    String text;
    if (server.hasArg("message"))
    {
      text = server.arg("message");
      lcd.clear();
      alert();
      lcd.print(text);
      lcd.scrollDisplayLeft();
      alert(1,1);
    }
  }
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>Batch No. 1712</title></head>");
  client.println("<meta http-equiv=\"refresh\" content=\"30\" />");
  client.println("<body bgcolor = \"#ebf4fa\">");
  client.println("<hr/><hr>");
  client.println("<center>");
  client.println("<font face= \"bahamas\" color = \"red\" size=\"6\">SREE VIDYANIKETHAN ENGINEERING COLLEGE</font>");
  client.println("<br>");
  client.println("<font color=\"red\" size=\"4\">(AUTONOMOUS)</font><br>");
  client.println("<font face=\"bookman old\" color=\"008000\" size=\"5\">Sree Sainath Nagar, A.Rangampet, Tirupati - 517 102</font><br>");
  client.println("<font face= \"times new roman\" color = \"151b54\" size=\"5.5\">");
  client.println("Department of Electronics and Communication Engineering</font>");
  client.println("<hr/><hr><br>");
  client.println("<h2>IOT based NODE Control</h2><br>");
  client.println("Device 1 is ");
  if (digitalRead(16)) {
    client.print("ON  ");
    client.println("<a href=\"/D1off\"\"><button>Turn Off </button></a>");
  }
  else {
    client.print("OFF  ");
    client.println("<a href=\"/D1on\"\"><button>Turn On </button></a>");
  }
  client.println("<br><br>");
  client.println("Device 2 is ");
  if (digitalRead(5)) {
    client.print("ON  ");
    client.println("<a href=\"/D2off\"\"><button>Turn Off </button></a>");
  }
  else {
    client.print("OFF  ");
    client.println("<a href=\"/D2on\"\"><button>Turn On </button></a>");
  }
  client.println("<br><br>");
  client.println("Device 3 is ");
  if (digitalRead(4)) {
    client.print("ON  ");
    client.println("<a href=\"/D3off\"\"><button>Turn Off </button></a>");
  }
  else {
    client.print("OFF  ");
    client.println("<a href=\"/D3on\"\"><button>Turn On </button></a>");
  }
  client.println("<br><br>");
  client.println("Device 4 is");
  if (digitalRead(0)) {
    client.print("ON  ");
    client.println("<a href=\"/D4off\"\"><button>Turn Off </button></a>");
  }
  else {
    client.print("OFF  ");
    client.println("<a href=\"/D4on\"\"><button>Turn On </button></a>");
  }
  client.println("<br><br>");
  client.println("Device 5 is ");
  if (digitalRead(2)) {
    client.println("ON  ");
    client.println("<a href=\"/D5off\"\"><button>Turn Off </button></a>");
  }
  else {
    client.print("OFF  ");
    client.println("<a href=\"/D5on\"\"><button>Turn On </button></a>");
  }
  client.println("<br><br>");
  client.println("Device 6 is ");
  if (digitalRead(14)) {
    client.println("ON  ");
    client.println("<a href=\"/D6off\"\"><button>Turn Off </button></a>");
  }
  else {
    client.print("OFF  ");
    client.println("<a href=\"/D6on\"\"><button>Turn On </button></a>");
  }
  client.println("<br><br>");
  client.println("Device 7 is ");
  if (digitalRead(12)) {
    client.println("ON  ");
    client.println("<a href=\"/D7off\"\"><button>Turn Off </button></a>");
  }
  else {
    client.print("OFF  ");
    client.println("<a href=\"/D7on\"\"><button>Turn On </button></a>");
  }
  client.println("<br><br>");
  client.println("Device 8 is ");
  if (digitalRead(15)) {
    client.println("ON  ");
    client.println("<a href=\"/D8off\"\"><button>Turn Off </button></a>");
  }
  else {
    client.print("OFF  ");
    client.println("<a href=\"/D8on\"\"><button>Turn On </button></a>");
  }
  client.println("<br><br>");

  //  client.println("<a href=\"/alert\"\"><button>Alert</button></a>");
  client.println("<a href=\"/reset\"\"><button>Reset</button></a>");
  client.println("<br>");

  client.println("<body><form action=\"/display\"\" method='POST'>");
  client.println("<center><b>Enter message to be displayed</b><br><textarea type='text' name='message'cols='16' rows='2'></textarea><br>");
  client.println("<input type='submit' name='SUBMIT' value='Submit'></form>");

  client.println("</center>");
  client.println("</html>");
}
void alert(int n, int m)
{
  for (int i = 0; i < n; i++)
  {
    digitalWrite(13, HIGH);
    delay(500 * m);
    digitalWrite(13, LOW);
    delay(500);
  }
}
void reset()
{
  digitalWrite(16, LOW);//D1
  digitalWrite(5, LOW);//D2
  digitalWrite(4, LOW);//D3
  digitalWrite(0, LOW);//D4
  digitalWrite(2, LOW);//D5
  digitalWrite(14, LOW);//D6
  digitalWrite(12, LOW);//D7
  digitalWrite(15, LOW);//D8
}
