#include <SoftwareSerial.h>
#include <TinyGPS.h>

const int messageButton = 10;
const int callButton = 11;

TinyGPS gps;
SoftwareSerial sgps(4, 5);
SoftwareSerial sgsm(2, 3);

float gpslat, gpslon;
int state;
int callstate;

const char* phoneNumber = "Your mobile number with country code";

void setup(){
  sgps.begin(9600);
  sgsm.begin(9600);

  pinMode(messageButton, INPUT);
  pinMode(callButton, INPUT);

  // Send initialization commands to GSM module
  sgsm.println("AT");
  sgsm.println("AT+CMGF=1"); // Set SMS mode to text

}

void loop(){
  // Check if message button is pressed
  if (digitalRead(messageButton) == HIGH && state == 0) {
    // Read GPS data
    while (sgps.available()) {
      char c = sgps.read();
      if (gps.encode(c)) {
        // GPS data is valid, send SMS
        long latitude, longitude;
        gps.get_position(&latitude, &longitude);


        // Create Google Maps link with current location
        String mapsLink = "https://www.google.com/maps?q=" + String(latitude, 6) + "," + String(longitude, 6);

        // Send SMS with Google Maps link
        sgsm.println("AT+CMGS=\"" + String(phoneNumber) + "\"");
        sgsm.println("I am sending my current location through Google Maps. Follow this link: " + mapsLink);
        sgsm.println((char)26); // ASCII code for CTR+Z

        // Print message to serial monitor
        Serial.println("Sending SMS with Google Maps link...");

        state = 1;
      }
    }
  }

  if (digitalRead(messageButton) == LOW)
  {
    state = 0;
  }

  // Check if call button is pressed
  if (digitalRead(callButton) == HIGH && callstate == 0) {
    // Make call
    sgsm.println("ATD" + String(phoneNumber) + ";"); // Dial phone number

    // Print message to serial monitor
    Serial.println("Making call through GSM...");
    
    callstate = 1;
    // this condition call received while the call receiver does not end the call
  }
  if (digitalRead(callButton) == LOW){
    callstate = 0;
  }
      delay(100);
}
