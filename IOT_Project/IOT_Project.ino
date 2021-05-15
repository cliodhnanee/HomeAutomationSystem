#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

const int GREEN = 26, RED = 25, LCD_ROW = 2, LCD_COL = 16, SERVO_PIN = 2, ANALOG_IN = A7;
int pos = 0, value = 0, error = 0;
Servo myservo;  // create servo object to control a servo

// Create the lcd object address 0x3F and 16 columns x 2 rows
LiquidCrystal_I2C lcd (0x27, LCD_COL, LCD_ROW);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

WiFiServer server(80); // server port 80
const char* ssid = "wifi";
const char* password = "0877930549";

void setup() {

  Serial.begin(57600);
  Serial2.begin(115200);

  //Seting up WiFi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("");
  Serial.println(WiFi.localIP());   //displays IP address to serial monitor
  server.begin();

  //initalize leds
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);

  //Servo Motor
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(SERVO_PIN, 1000, 2000);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.print( "Welcome" );
  lcd.setCursor(0, 1);
  lcd.print( "Home Security");
  delay (100);

  //Fingerprint sensor
  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("\nFound fingerprint sensor!");
    delay(50);
  }
  else {
    Serial.println("\nDid not find fingerprint sensor :(");
    while (1);
  }

}

void loop()
{
  int input = 0;
  Serial.println("To enroll new  press 1, otherwise press 2");
  while (input == 0) {
    while (! Serial.available());
    input = Serial.parseInt();
  }

  if (input == 1) {
    enroll();
  }

  if (input == 2) {
    do {
      website();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Place finger");
      lcd.setCursor(0, 1);
      lcd.print("on sensor");
      getFingerID();
      
      if (error == 3) {
      website();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      lcd.setCursor(0, 1);
      lcd.print("Too many tries");
      delay(3000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Log-in via:");
      lcd.setCursor(0, 1);
      lcd.print(WiFi.localIP());
      delay(3000);
    }
    } while (input==2);
  }
}
