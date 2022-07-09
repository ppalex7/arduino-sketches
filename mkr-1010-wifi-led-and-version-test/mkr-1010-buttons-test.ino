
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  WiFiDrv::pinMode(25, OUTPUT);
  WiFiDrv::pinMode(26, OUTPUT);
  WiFiDrv::pinMode(27, OUTPUT);

  
  WiFiDrv::digitalWrite(25, HIGH);  //GREEN
  WiFiDrv::digitalWrite(26, LOW);   //RED
  WiFiDrv::analogWrite(27, 127);    //BLUE
  delay(700);
  WiFiDrv::digitalWrite(25, LOW);   //GREEN
  WiFiDrv::digitalWrite(26, LOW);   //RED
  WiFiDrv::digitalWrite(27, LOW);   //BLUE
  delay(1000);
  WiFiDrv::digitalWrite(25, HIGH);  //GREEN
  WiFiDrv::digitalWrite(26, LOW);   //RED
  WiFiDrv::analogWrite(27, 127);    //BLUE
}

void loop() {
  Serial.println(WiFi.firmwareVersion());
  static int r = 0;
  digitalWrite(LED_BUILTIN, r);

  if (r == LOW) {
    r = HIGH;
  } else {
    r = LOW;
  }
  delay(10000);
}
