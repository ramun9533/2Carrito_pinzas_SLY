#include <WiFi.h>
#include <DNSServer.h>
#include "data3.h"  // Asegúrate de que aquí están tus credenciales WiFi
#include "data.h"
#include "data1.h"

// Configuración del servidor web
WiFiServer server(80);

// Pines de los LEDs
const int numLEDs = 7;
// Evitar salidas 0 y 3 para PWM
static unsigned short int LEDPins[numLEDs] = {2, 0, 4, 5, 18, 19, 3}; 

// Variables para manejar la comunicación con el cliente
char linebuf[80];
int charcount = 0;

void setup() {
  Serial.begin(115200);

  // Configurar pines de los LEDs como salidas
  for (int i = 0; i < numLEDs; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }

  // Conexión a la red WiFi
  Serial.printf("Conectando a: %s\n", ssid);
  WiFi.begin(ssid, password);

  // Esperar a que se conecte
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado a WiFi!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Iniciar el servidor web
}

// Función para procesar solicitudes de control de LED
void processLEDRequest(const char* request, int ledIndex) {
  if (strstr(linebuf, request) != NULL) {
    if (strstr(request, "on") != nullptr) {
      digitalWrite(LEDPins[ledIndex], HIGH);
      Serial.printf("LED %d ON\n", ledIndex + 1);
    } else if (strstr(request, "off") != nullptr) {
      digitalWrite(LEDPins[ledIndex], LOW);
      Serial.printf("LED %d OFF\n", ledIndex + 1);
    }
  }
}

// Función para manipular el estado de los LEDs
void manipulacionLed() {
  for (int i = 1; i <= numLEDs; i++) {
    char requestOn[10];
    char requestOff[10];
    sprintf(requestOn, "GET /on%d", i);
    sprintf(requestOff, "GET /off%d", i);
    processLEDRequest(requestOn, i - 1);
    processLEDRequest(requestOff, i - 1);
  }
}

void loop() {
  WiFiClient client = server.available(); // Verificar clientes conectados

  if (client) {
    String currentLine = "";
    boolean currentLineIsBlank = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        linebuf[charcount] = c;

        if (charcount < sizeof(linebuf) - 1) charcount++;

        if (c == '\n') {
          currentLineIsBlank = true;
          if (currentLine.length() == 0) {
            web(client); // Llamar a la función web desde data.h
            break;
          } else {
            currentLine = "";
          }
          
          manipulacionLed();
          memset(linebuf, 0, sizeof(linebuf));
          charcount = 0;
        } else if (c != '\r') {
          currentLine += c;
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
