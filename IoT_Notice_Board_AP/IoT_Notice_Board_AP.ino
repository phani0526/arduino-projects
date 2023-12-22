#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>

int Buzzer = 16;
int red = 0;
int green = 2;
const char* ssid = "IoT_WiFi";
const char* password = "Internet@IoT";

//IPAddress ip(192, 168, 1, 112);  //config static IP
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 255, 0);
//LiquidCrystal_I2C lcd(0x27, 16, 2); //D1 SCL ; D2 SDA
LiquidCrystal_I2C lcd(0x27,20,4);
ESP8266WebServer server(80);

void setup(void)
{
  Serial.begin(115200);
  pinMode(Buzzer, OUTPUT);//D0
  pinMode(red, OUTPUT);//D3
  pinMode(green, OUTPUT);//D4
  digitalWrite(red, HIGH);//D3
  digitalWrite(green,HIGH);//D4
  lcd.init();   // initializing the LCD
  lcd.noBacklight();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);
  IPAddress myIP = WiFi.softAPIP();
//  #WiFi.begin(ssid, password);
//  #Serial.println("");
// #while (WiFi.status() != WL_CONNECTED) {
//  #  delay(500);  // Wait for connection
  Serial.print(myIP);
//  #}
  alert();
  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  lcd.print(" Smart Notice Board ");
  lcd.print("Open any Web Browser");
  lcd.print("Connect to  IoT_WiFi");
  lcd.print("@ http://192.168.4.1");

  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );
  server.on("/display", disp);
  server.begin();
  Serial.println("HTTP server started");
  Serial.println(WiFi.localIP()); 
  
  digitalWrite(red,LOW);
}
void disp()
{
  String text;
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  if (server.hasArg("message"))
  {
    text = server.arg("message");
    lcd.clear();
    alert();
    lcd.print(text);
    //lcd.scrollDisplayLeft();
    alert();
    server.send(204, "");
    Serial.print(text);
  }
}

void alert()
{
  digitalWrite(Buzzer, HIGH);
  delay(500);
  digitalWrite(Buzzer, LOW);
  server.send(204, "");
}
void reset()
{
  alert();
  alert();
  server.send(204, "");
}

void loop(void) {
  server.handleClient();
}

//Check if header is present and correct
bool is_authentified()
{
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie"))
  {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1)
    {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

void handleLogin()//login page, also called for disconnect
{
  String msg;
  if (server.hasHeader("Cookie"))
  {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT"))
  {
    Serial.println("Disconnection");
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD"))
  {
    if (server.arg("USERNAME") == "1712" &&  server.arg("PASSWORD") == "admin" ) {
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);
      alert();
      return;
    }
    msg = "<font color=red><b>Wrong User ID/Password! try again.</b></font>";
  }
  String content = "<html>";
  content += "<title>Login</title>";
  content += "<body><form action='/login' method='POST'>";
  content += "<br><br><br><br><br>";
  content += "<center><table border=4 color=navy><tr><td>";
  content += "<center><h3><font color=\"#009900\"><b>ZPHS Cherlopalli</font></h3>";
  content += "<h3>This is login page to </h3>";
  content += "<font color=navy><h2>IoT based Smart Notice Board</font></h2><br></center>";
  content += "<center><table border=3><tr><td>";
  content += "<center><br><b>User ID:</b><input type='text' name='USERNAME' placeholder='User ID'><br><br>";
  content += "<b>Password:</b><input type='password' name='PASSWORD' placeholder='Password'><br><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'><br><br></form>" + msg + "<br></center></html>";
  content += "</td></tr></table>";
  content += "<br><br>";
  content += "</td></tr></table>";
  server.send(200, "text/html", content);
}

//root page can be accessed only if authentification is ok
void handleRoot()
{
  String header;
  if (!is_authentified())
  {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  
  // Webpage
  String content = "<html>";
  content += "<head><title>ZPHS Cherlopalli</title></head>";
  //content += "<meta http-equiv=\"refresh\" content=\"30\" />";
  content += "<center><header>";
  content += "<table border=4><tr><td>";
  /*content += "<center><b><font color = \"navy\">Batch No.: 1712</font></b><br>D Sai Charan-13121A0439<br>Ch Maheswari-13121A0432<br>A Jahnavi-13121A0402<br>G Hemasekhar-13121A0458<br>D Phaneendra-13121A0445</center>";*/
  //content += "<td width=71.5%><center>";
  content += "<font face= \"bahamas\" color = \"#cc0066\" size=\"6\">ZP High School CHERLOPALLI</font><br>";
  content += "<font face=\"bookman old\" color=\"008000\" size=\"5\">Settur(M), Anantapuram(D)</font><br>";
  content += "<font face= \"times new roman\" color = \"151b54\" size=\"5.5\">";
  //content += "Department of Electronics and Communication Engineering</font>";
  content += "</center></header></td>";
  /*content += "<td><center><b><font color = \"navy\">Under the Guidance of:</font></b><br>Ms.M.Bharathi, M.Tech.,<br>Assistant Professor,<br>Dept. of ECE,<br>SVEC,<br>Tirupati.</center>";*/
  content += "</td></tr>";
  content += "<tr><td colspan=3>";
  content += "<h1><center><font color = \"navy\">IoT based Smart Notice Board</font></h1>";
  /*content += "<center><table border=3><tr>";
  content += "<td width=60%><br><center>";
  content += "<b>Device Controls</b><br><br>";

  content += "<a href=\"/reset\"\"><button>Reset</button></a><br><br>";
  content += "<a href=\"/alert\"\"><button>Alert</button></a>";
  
  content += "<br><br><table border=\"1\"><tr><td>";
  
  content += "</center></td></tr></table></ul></td>";
  content += "</tr>";*/
  content += "<tr><td colspan=2>";
  content += "<body><form action='/display' method='POST'>";
  content += "<center><b>Enter message to be displayed</b><br><textarea type='text' name='message'cols='16' rows='2'></textarea><br>";
  //content += "<input type='submit' name='SUBMIT' value='Submit'></form>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form>";
  content += "</center></td></tr></table>";
  
  content += "<br>To logout <a href=\"/login?DISCONNECT=YES\">click here</a></body></center></html>";
  content += "</tr></td></table>";
  content += "</center>";
  server.send(200, "text/html", content);
}
