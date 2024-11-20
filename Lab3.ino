#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// Credenciales Wi-Fi
const char* ssid = "Shinigami 4.0";
const char* password = "Maria>.<270966";

// Dirección IP y puerto del broker Mosquitto
const char* mqtt_server = "192.168.18.79";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
Servo myServo;

// Pines de conexión
const int PIR_PIN = 4;    // Pin del sensor PIR (D4)
const int SERVO_PIN = 5;  // Pin del microServo (D5)
const char* topic = "sensor/pir";


void setup() {
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);
  pinMode(PIR_PIN, INPUT);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi");

  // Configuración de MQTT
  client.setServer(mqtt_server, mqtt_port);

  // Conectar a MQTT
  while (!client.connected()) {
    Serial.println("Conectando al broker MQTT...");
    Serial.println("");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado al broker MQTT");
    } else {
      Serial.print("Fallo en la conexión. Estado: ");
      Serial.print(client.state());
      Serial.println("");
      delay(2000);
    }
  }
}

void loop() {
  client.loop();

  int pirState = digitalRead(PIR_PIN);
  if (pirState == HIGH) {
    Serial.println("Movimiento detectado");
    client.publish(topic, "Movimiento detectado");
    myServo.write(90);  // Gira el servo 90 grados

    delay(2000); // Mantiene el motor abierto por 2 segundos
    myServo.write(0);  // Regresa el servo a la posición inicial
  }
  delay(100); // Delay para evitar múltiples detecciones
}