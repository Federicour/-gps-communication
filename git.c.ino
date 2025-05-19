#include <TinyGPS.h>
#include <SoftwareSerial.h>

SoftwareSerial Gsm(9, 10); // GSM module connections
SoftwareSerial GPS(3, 4);  // GPS module connections

TinyGPS gps;
char phone_no[] = "+91883058xxxx"; // Replace with your phone number
const int buttonPin = 5;
const int buzzerPin = 6;
bool isSMSsent = false;

void setup() {
  Serial.begin(9600);
  Gsm.begin(9600);
  GPS.begin(9600);

  sendATCommand("AT+CMGF=1\r", 1000); // Set SMS to text mode
  sendATCommand("AT+CNMI=2,2,0,0,0\r", 1000); // Enable SMS notifications 

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  GPS.listen();
  readGPSData();

  checkButtonAndSendSMS();
}

void readGPSData() {
  while (GPS.available()) {
    if (gps.encode(GPS.read())) {
      // Successfully encoded GPS data 
    }
  }
}

void checkButtonAndSendSMS() {
  if (digitalRead(buttonPin) == LOW && !isSMSsent) {
    isSMSsent = true;

    float flat, flon;
    gps.f_get_position(&flat, &flon);

    Gsm.listen();
    sendSMS(flat, flon);
  } else if (digitalRead(buttonPin) == HIGH) {
    isSMSsent = false;
  }
}

void sendATCommand(String command, int timeout) {
  Gsm.println(command);
  unsigned long startTime = millis();

  while (millis() - startTime < timeout) {
    if (Gsm.find("OK")) { 
      return; // Command successful
    }
    if (Gsm.find("ERROR")) {
      Serial.println("AT Command Error!"); 
      return; // Command failed
    }
  }
  Serial.println("AT Command Timeout!"); 
}

void sendSMS(float flat, float flon) {
  Gsm.print("AT+CMGS=\"");
  Gsm.print(phone_no);
  Gsm.println("\"\r"); 
  delay(100);

  Gsm.println("Alert! I need help at:");
  Gsm.print("Latitude: ");
  Gsm.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? "N/A" : String(flat, 6));
  Gsm.print("Longitude: ");
  Gsm.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? "N/A" : String(flon, 6));
  Gsm.print("http://maps.google.com/maps?q=loc:");
  Gsm.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
  Gsm.print(",");
  Gsm.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);

  delay(100);
  Gsm.println((char)26); // Send SMS
  delay(100);

  if (Gsm.find("OK")) {
    Serial.println("SMS Sent!");
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
  } else {
    Serial.println("SMS Send Failed!");
  }
}