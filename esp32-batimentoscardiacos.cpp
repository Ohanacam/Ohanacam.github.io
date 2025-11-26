#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "esp_wifi.h"
#include "esp_eap_client.h"
#include <Preferences.h>

//  WIFI EDUROAM
const char* WIFI_SSID    = "eduroam";
const char* EAP_IDENTITY = "401691";
const char* EAP_USERNAME = "401691";
const char* EAP_PASSWORD = "ohana123";

// FIREBASE
String firebaseHost = "https://esp32c6-32158-default-rtdb.firebaseio.com";  


// MQTT 
const char* mqtt_server = "broker.mqtt.cool";
const char* mqtt_topic  = "esp32/display";

WiFiClient espClient;
PubSubClient client(espClient);

// DISPLAY 
#define TFT_MOSI  7
#define TFT_SCLK  6
#define TFT_CS    10
#define TFT_DC    11
#define TFT_RST   8
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// SENSOR CARDÍACO
#define SENSOR_PIN 5
Preferences preferences;
int limite = 0;

unsigned long ultimoBatimento = 0;
int bpm = 0;
bool batendo = false;


// DISPLAY
void showMessage(String msg) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 30);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.println(msg);
}

// WIFI EDUROAM
void connectWiFiEduroam() {
  WiFi.disconnect(true, true);
  delay(100);

  WiFi.mode(WIFI_STA);
  esp_eap_client_set_identity((const uint8_t*)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_eap_client_set_username((const uint8_t*)EAP_USERNAME, strlen(EAP_USERNAME));
  esp_eap_client_set_password((const uint8_t*)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wifi_sta_enterprise_enable();

  WiFi.begin(WIFI_SSID);

  Serial.print("Conectando ao Wi-Fi EDUROAM");
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() - t0 > 30000) ESP.restart();
  }
  Serial.println("\nWi-Fi conectado!");
}

// MQTT CALLBACK
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) message += (char)payload[i];
  showMessage(message);
}

// MQTT reconectar
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32C6_Client")) {
      Serial.println("Conectado!");
      client.subscribe(mqtt_topic);
    } else {
      Serial.printf("Falha (rc=%d). 5s...\n", client.state());
      delay(5000);
    }
  }
}

// ENVIO PARA FIREBASE
void sendToFirebase(int bpm) {
  HTTPClient http;
  String url = firebaseHost + "/bpm.json";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  String json = "{\"bpm\": " + String(bpm) + ", \"timestamp\": " + String(millis()) + "}";

  int httpCode = http.POST(json);

  Serial.print("Firebase retorno: ");
  Serial.println(httpCode);
  
  http.end();
}

//  SETUP
void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_PIN, INPUT);

  Serial.println("Inicializando...");

  // PREFERENCES
  preferences.begin("config", false);
  limite = preferences.getInt("limite", 400);  
  Serial.print("Limite carregado: ");
  Serial.println(limite);

  // WIFI
  connectWiFiEduroam();

  //  DISPLAY 
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.initR(INITR_MINI160x80);
  tft.setRotation(1);
  showMessage("Iniciando...");

  // MQTT 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.println("Sistema iniciado.");
}

//  LOOP 
void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  int sinal = analogRead(SENSOR_PIN);
  unsigned long agora = millis();

  // DETECTAR BATIMENTOS
  if (sinal > limite && !batendo) {
    batendo = true;
    unsigned long intervalo = agora - ultimoBatimento;
    ultimoBatimento = agora;

    if (intervalo > 300 && intervalo < 2000) {
      bpm = 60000 / intervalo;

      String msg = "BPM: " + String(bpm);
      showMessage(msg);

      client.publish(mqtt_topic, msg.c_str());

      Serial.print("Enviando BPM ao Firebase: ");
      Serial.println(bpm);
      sendToFirebase(bpm);
    }
  }

  if (sinal < limite) batendo = false;

  delay(20);
}
