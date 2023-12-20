#include <Thread.h>
#include <ThreadController.h>
#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //This library you can add via Include Library > Manage Library > 
LiquidCrystal_I2C lcd(0x3F, A4, A5);

// ThreadController that will controll all threads
ThreadController control = ThreadController();

//His Thread (not pointer)
Thread hisThread = Thread();

// callback for hisThread
void disp(){
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Guide:Radha Devi");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SA BIO Science");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Student:D Arvind");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ZPHS CHERLOPALLI");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SETTUR(M),DMM(D)");
  delay(1000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("ECO FRIENDLY");
  lcd.setCursor(3, 1);
  lcd.print("SMART HOME");
}

void setup(){
  Serial.begin(9600);
  lcd.init();   // initializing the LCD
  
  // Configure hisThread
  hisThread.onRun(disp);
  //hisThread.setInterval(10);

  // Adds both threads to the controller
  control.add(&hisThread); // & to pass the pointer to it
 // control.run();
 
}

void loop(){
  // run ThreadController
  // this will check every thread inside ThreadController,
  // if it should run. If yes, he will run it;
  

  // Rest of code
 hisThread.run(); 
 Serial.println("Phaneendra");
 delay(1000);
}
