#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

const int BUZZER_PIN = 16;
const int RED_LED_PIN = 0;
const int GREEN_LED_PIN = 2;
const int CLEAR_BUTTON = 14;
const int NEXT_BUTTON = 5;
const int PREV_BUTTON = 6;
const int SCROLL_UP_BUTTON = 7;
const int SCROLL_DOWN_BUTTON = 8;

const char* USERNAME = "1712";
const char* USER_PASSWORD = "admin";
const char* DEFAULT_MESSAGE = "No new messages.";
const int HISTORY_SIZE = 10;
const int MAX_LINES = 4;

struct Message {
  char text[80];
  time_t timestamp;
};

struct WiFiCredentials {
  char ssid[32];
  char password[32];
};

WiFiCredentials wifiCreds;
Message messageHistory[HISTORY_SIZE];
int messageIndex = 0;
int currentMessage = -1;
int scrollPosition = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);
ESP8266WebServer server(80);

void setup(void) {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(CLEAR_BUTTON, INPUT_PULLUP);
  pinMode(NEXT_BUTTON, INPUT_PULLUP);
  pinMode(PREV_BUTTON, INPUT_PULLUP);
  pinMode(SCROLL_UP_BUTTON, INPUT_PULLUP);
  pinMode(SCROLL_DOWN_BUTTON, INPUT_PULLUP);

  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, HIGH);

  lcd.init();
  lcd.backlight();
  displayDefaultMessage();

  EEPROM.begin(sizeof(WiFiCredentials) + sizeof(messageHistory));
  EEPROM.get(0, wifiCreds);
  EEPROM.get(sizeof(WiFiCredentials), messageHistory);

  WiFi.begin(wifiCreds.ssid, wifiCreds.password);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");
    for (int i = 0; i < 30; i++) {
      if (WiFi.status() == WL_CONNECTED) {
        break;
      }
      delay(1000);
      Serial.print(".");
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
    IPAddress myIP = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(myIP);
  } else {
    Serial.println("Failed to connect to WiFi, starting AP mode");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("SmartNoticeBoard", "12345678");
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP Address: ");
    Serial.println(myIP);
  }

  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/display", disp);
  server.on("/clear", handleClear);
  server.on("/history", showHistory);
  server.on("/wifi", handleWiFiConfig);
  server.on("/setwifi", handleSetWiFiConfig);

  const char *headerkeys[] = {"User-Agent", "Cookie"};
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  server.collectHeaders(headerkeys, headerkeyssize);

  server.begin();
  Serial.println("HTTP server started");

  digitalWrite(RED_LED_PIN, LOW);
}

void loop(void) {
  server.handleClient();
  if (digitalRead(CLEAR_BUTTON) == LOW) {
    handleClear();
    delay(500);
  }
  if (digitalRead(NEXT_BUTTON) == LOW) {
    showNextMessage();
    delay(500);
  }
  if (digitalRead(PREV_BUTTON) == LOW) {
    showPreviousMessage();
    delay(500);
  }
  if (digitalRead(SCROLL_UP_BUTTON) == LOW) {
    scrollUp();
    delay(500);
  }
  if (digitalRead(SCROLL_DOWN_BUTTON) == LOW) {
    scrollDown();
    delay(500);
  }
}

void disp() {
  if (server.hasArg("message")) {
    String text = server.arg("message");
    lcd.clear();
    alert();
    lcd.print(text);
    alert();
    server.send(204, "");
    Serial.print(text);

    strncpy(messageHistory[messageIndex].text, text.c_str(), sizeof(messageHistory[messageIndex].text));
    messageHistory[messageIndex].timestamp = now();
    messageIndex = (messageIndex + 1) % HISTORY_SIZE;
    EEPROM.put(sizeof(WiFiCredentials), messageHistory);
    EEPROM.commit();
  }
}

void handleClear() {
  lcd.clear();
  displayDefaultMessage();
  alert();
  server.send(200, "text/plain", "Screen cleared.");
}

void displayDefaultMessage() {
  lcd.setCursor(0, 0);
  lcd.print("Smart Notice Board");
  lcd.setCursor(0, 1);
  lcd.print("Open any Web Browser");
  lcd.setCursor(0, 2);
  lcd.print("Connect to IoT_WiFi");
  lcd.setCursor(0, 3);
  lcd.print("@ http://192.168.4.1");
}

void alert() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
}

bool isAuthenticated() {
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      return true;
    }
  }
  return false;
}

void handleLogin() {
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) {
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == USERNAME && server.arg("PASSWORD") == USER_PASSWORD) {
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);
      alert();
      return;
    }
    msg = "<font color=red><b>Wrong User ID/Password! try again.</b></font>";
  }
  String content = "<html><title>Login</title><body><form action='/login' method='POST'>";
  content += "<center><table border=4><tr><td>";
  content += "<center><h3><font color=\"#009900\"><b>ZPHS Cherlopalli</font></h3>";
  content += "<h3>This is login page to </h3>";
  content += "<font color=navy><h2>IoT based Smart Notice Board</font></h2><br></center>";
  content += "<center><table border=3><tr><td>";
  content += "<center><br><b>User ID:</b><input type='text' name='USERNAME' placeholder='User ID'><br><br>";
  content += "<b>Password:</b><input type='password' name='PASSWORD' placeholder='Password'><br><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'><br><br></form>" + msg + "<br></center></html>";
  content += "</td></tr></table></td></tr></table></center></body></html>";
  server.send(200, "text/html", content);
}

void handleRoot() {
  if (!isAuthenticated()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }

  String content = "<html><head><title>ZPHS Cherlopalli</title></head>";
  content += "<center><header><table border=4><tr><td>";
  content += "<font face= \"bahamas\" color = \"#cc0066\" size=\"6\">ZP High School CHERLOPALLI</font><br>";
  content += "<font face=\"bookman old\" color=\"008000\" size=\"5\">Settur(M), Anantapuram(D)</font><br>";
  content += "<font face= \"times new roman\" color = \"151b54\" size=\"5.5\">";
  content += "</center></header></td></tr><tr><td colspan=3>";
  content += "<h1><center><font color = \"navy\">IoT based Smart Notice Board</font></h1>";
  content += "<tr><td colspan=2>";
  content += "<body><form action='/display' method='POST'>";
  content += "<center><b>Enter message to be displayed</b><br><textarea type='text' name='message'cols='20' rows='4'></textarea><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form><br>";
  content += "<form action='/clear'><input type='submit' name='CLEAR' value='Clear Screen'></form>";
  content += "<form action='/history'><input type='submit' name='HISTORY' value='Show History'></form>";
  content += "<form action='/wifi'><input type='submit' name='WIFI' value='Configure WiFi'></form>";
  content += "</center></body></html>";
  server.send(200, "text/html", content);
}

void showHistory() {
  if (!isAuthenticated()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  String content = "<html><head><title>History</title></head><body><h1>Message History</h1>";
  for (int i = 0; i < HISTORY_SIZE; i++) {
    content += "<p>" + String(messageHistory[i].text) + "</p>";
  }
  content += "<form action='/'><input type='submit' value='Back'></form>";
  content += "</body></html>";
  server.send(200, "text/html", content);
}

void scrollUp() {
  scrollPosition = max(scrollPosition - 1, 0);
  displayMessage(currentMessage);
}

void scrollDown() {
  scrollPosition = min(scrollPosition + 1, max(0, strlen(messageHistory[currentMessage].text) / 20 - MAX_LINES + 1));
  displayMessage(currentMessage);
}

void showNextMessage() {
  currentMessage = (currentMessage + 1) % HISTORY_SIZE;
  scrollPosition = 0;
  displayMessage(currentMessage);
}

void showPreviousMessage() {
  currentMessage = (currentMessage - 1 + HISTORY_SIZE) % HISTORY_SIZE;
  scrollPosition = 0;
  displayMessage(currentMessage);
}

void displayMessage(int messageIndex) {
  lcd.clear();
  String text = String(messageHistory[messageIndex].text);
  int startIndex = scrollPosition * 20;
  int endIndex = min(startIndex + 80, text.length());
  lcd.setCursor(0, 0);
  lcd.print(text.substring(startIndex, endIndex));
}

void handleWiFiConfig() {
  if (!isAuthenticated()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  String content = "<html><head><title>WiFi Configuration</title></head><body>";
  content += "<h1>Configure WiFi</h1>";
  content += "<form action='/setwifi' method='POST'>";
  content += "SSID: <input type='text' name='ssid'><br>";
  content += "Password: <input type='password' name='password'><br>";
  content += "<input type='submit' value='Save'>";
  content += "</form>";
  content += "</body></html>";
  server.send(200, "text/html", content);
}

void handleSetWiFiConfig() {
  if (!isAuthenticated()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    ssid.toCharArray(wifiCreds.ssid, sizeof(wifiCreds.ssid));
    password.toCharArray(wifiCreds.password, sizeof(wifiCreds.password));
    EEPROM.put(0, wifiCreds);
    EEPROM.commit();
    String response = "<html><body><h1>WiFi Configuration Saved</h1>";
    response += "<p>SSID: " + ssid + "</p>";
    response += "<p>Password: " + password + "</p>";
    response += "<form action='/'><input type='submit' value='Back'></form>";
    response += "</body></html>";
    server.send(200, "text/html", response);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Invalid Parameters");
  }
}
