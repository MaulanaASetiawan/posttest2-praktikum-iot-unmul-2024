#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const int btnOnOff = D3;
const int btn2 = D4;
const int btn3 = D5;
const int Led = D6;
const int Buzzer = D2;

int valBuzzer = 100;
int valLed = 0;
int btnPushCounter = 0;
int lastButtonState = 0;


const char* ssid = "Universitas Mulawarman";
const char* password = "";
ESP8266WebServer server(80);


void setup() {
  pinMode(Led, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(btnOnOff, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  Program();
  server.handleClient();
}

void handleRoot() {
  String str_valLed = String(valLed);
  String str_valBuzzer = String(valBuzzer);
  String response = "<html><body><h2>Led State: " + str_valLed + "</h2><br><h2>Buzzer State: " + str_valBuzzer + "</h2></body></html>";
  server.send(200, "text/html", response);
}

void Program() {
  int btnControl = digitalRead(btnOnOff);
  int btnState1 = digitalRead(btn2);
  int btnState2 = digitalRead(btn3);

  if (btnControl != lastButtonState) {
    if (btnControl == LOW) {
      btnPushCounter++;
    }
    delay(50);
  }
  lastButtonState = btnControl;

  if (btnPushCounter % 2 == 0) {
    if (!btnState1) {
      Serial.println(btnState1);
      if (0 <= valLed <= 249) {
        valLed += 3;
      }
      if (100 <= valBuzzer <= 2500) {
        valBuzzer += 10;
      }
    }

    if (!btnState2) {
      Serial.println(btnState2);
      if (0 <= valLed <= 249) {
        valLed -= 3;
      }
      if (100 <= valBuzzer <= 2500) {
        valBuzzer -= 10;
      }
    }
    tone(Buzzer, valBuzzer);
    analogWrite(Led, valLed);
    delay(100);
  } else {
    noTone(Buzzer);
    digitalWrite(Led, LOW);
  }
}