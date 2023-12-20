#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "ESP8266";
const char *password = "pass";

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
	server.send(200, "text/html", "<h1>You are connected</h1>");
  String content ="<html>";
  content +="<head><title>ACME</title></head>";
  content +="<meta http-equiv=\"refresh\" content=\"60\" />";
  content +="<body bgcolor = \"#ebf4fa\">";
  content +="<hr/><hr>";
  content +="<center>";
  content +="<font face= \"bahamas\" color = \"red\" size=\"6\">SREE VIDYANIKETHAN ENGINEERING COLLEGE</font>";
  content +="<br>";
  content +="<font color=\"red\" size=\"4\">(AUTONOMOUS)</font><br>";
  content +="<font face=\"bookman old\" color=\"008000\" size=\"5\">Sree Sainath Nagar, A.Rangampet, Tirupati - 517 102</font><br>";
  content +="<font face= \"times new roman\" color = \"151b54\" size=\"5.5\">";
  content +="Department of Electronics and Communication Engineering</font>";
  content +="<hr/><hr><br>";
  content += "<h3><center>IOT using ESP8266</h3><br>";
  content+="<center>";
  content +="Device 1 is ";
  if (digitalRead(0)){content +="ON  ";content +="<a href=\"/D1off\"\"><button>Turn Off </button></a>";}
  else{content +="OFF  ";content +="<a href=\"/D1on\"\"><button>Turn On </button></a>";}
  content+="<br><br>";
  content +="Device 2 is ";
  if (digitalRead(2)){content +="ON  ";content +="<a href=\"/D2off\"\"><button>Turn Off </button></a>";}
  else{content +="OFF  ";content +="<a href=\"/D2on\"\"><button>Turn On </button></a>";}
  content+="<br><br>";
  
}

void setup()
{
	delay(1000);
	Serial.begin(115200);
	Serial.println();
	Serial.print("Configuring access point...");
	WiFi.softAP(ssid, password);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("AP IP address: ");
	Serial.println(myIP);
	server.on("/", handleRoot);
  server.on("/D1on",on1);
  server.on("/D1off",off1);
  server.on("/D2on",on2);
  server.on("/D2off",off2);
	server.begin();
	Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();
}
void on1()
{
  digitalWrite(0,HIGH);server.send(204,"");
}
void off1()
{
  digitalWrite(0,LOW);server.send(204,"");
}
void on2()
{
  digitalWrite(2,HIGH);server.send(204,"");
}
void off2()
{
  digitalWrite(2,LOW);server.send(204,"");
}

