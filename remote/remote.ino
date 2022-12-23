#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER_IP "192.168.4.1"

#ifndef STASSID
#define STASSID "flattruck"
#define STAPSK  "flattruck"
#endif

#define FORWARD   D7
#define REVERSE   D6
#define LEFT      D5
#define RIGHT     D3

bool prev_state[4] = {0,0,0,0};
bool state[4] = {0,0,0,0};

bool send_flag = false;

WiFiClient client;
HTTPClient http;

void setup() {
  delay(1000);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(FORWARD, INPUT_PULLUP);
  pinMode(REVERSE, INPUT_PULLUP);
  pinMode(LEFT,    INPUT_PULLUP);
  pinMode(RIGHT,   INPUT_PULLUP);

  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println();

  connect_to_wifi();
  
}

void loop() {
  

  // put your main code here, to run repeatedly:
  state[0] = digitalRead(FORWARD);
  state[1] = digitalRead(REVERSE);
  state[2] = digitalRead(LEFT);
  state[3] = digitalRead(RIGHT);

  //randomize state for testing
  //random_state();

  for (int i = 0; i < 4; i++) {
    //reverse 0 to 1 and 1 to 0
    state[i] = !state[i];
    if (state[i] != prev_state[i]) {
      

      send_flag = true;
      prev_state[i] = state[i];
    }
  }

  if (send_flag) {
    while (!send_request()) {
      if (WiFi.status() != WL_CONNECTED) {
        connect_to_wifi();
      }
    }
    send_flag = false;
  }
}

void connect_to_wifi() {
  digitalWrite(LED_BUILTIN, LOW);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void random_state(){
  for (int i = 0; i < 4; i++) {
    state[i] = random(0, 2);
  }
}

bool send_request() {
  String command = String(state[0]) + "," + String(state[1]) + "," + String(state[2]) + "," + String(state[3]);
  String url = "http://" + String(SERVER_IP) + "/?command=" + command;
  Serial.print("Sending request: ");
  Serial.println(url);
  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    return true;
  } else {
    return false;
  }
}
