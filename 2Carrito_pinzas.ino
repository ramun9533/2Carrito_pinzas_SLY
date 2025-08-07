Aquí está el código completo con comentarios detallados:

```cpp
// Inclusión de bibliotecas necesarias
#include <WiFi.h>          // Biblioteca para la conexión WiFi del ESP32
#include <DNSServer.h>     // Biblioteca para servidor DNS (para posibles funciones de portal cautivo)
#include "data3.h"         // Archivo personalizado que contiene las credenciales WiFi (ssid y password)
#include "data.h"          // Archivo que probablemente contiene la función web() para servir páginas HTML
#include "data1.h"         // Archivo adicional con posibles configuraciones o recursos

// Configuración del servidor web en el puerto 80 (HTTP estándar)
WiFiServer server(80);

// Configuración de los LEDs
const int numLEDs = 7;  // Número total de LEDs a controlar
// Array con los pines GPIO asignados a cada LED (nota: evitar pines 0 y 3 para PWM)
static unsigned short int LEDPins[numLEDs] = {2, 0, 4, 5, 18, 19, 3}; 

// Buffers y variables para manejo de la comunicación HTTP
char linebuf[80];        // Buffer para almacenar la línea recibida del cliente
int charcount = 0;       // Contador de caracteres recibidos

void setup() {
  // Inicialización de la comunicación serial para depuración
  Serial.begin(115200);

  // Configuración de todos los pines LED como salidas
  for (int i = 0; i < numLEDs; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }

  // Conexión a la red WiFi
  Serial.printf("Conectando a: %s\n", ssid);  // Muestra el SSID al que se conectará
  WiFi.begin(ssid, password);  // Intenta conectar con las credenciales

  // Espera hasta que la conexión WiFi esté establecida
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);  // Pequeña pausa entre intentos
    Serial.print(".");  // Retroalimentación visual de conexión
  }

  // Mensajes de confirmación de conexión
  Serial.println("\nConectado a WiFi!");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());  // Muestra la IP asignada

  server.begin();  // Inicia el servidor web
}

// Función para procesar solicitudes de control de LEDs
void processLEDRequest(const char* request, int ledIndex) {
  // Verifica si el request contiene el comando específico
  if (strstr(linebuf, request) != NULL) {
    // Comprueba si es un comando para encender
    if (strstr(request, "on") != nullptr) {
      digitalWrite(LEDPins[ledIndex], HIGH);  // Enciende el LED
      Serial.printf("LED %d ON\n", ledIndex + 1);  // Log por serial
    } 
    // Comprueba si es un comando para apagar
    else if (strstr(request, "off") != nullptr) {
      digitalWrite(LEDPins[ledIndex], LOW);  // Apaga el LED
      Serial.printf("LED %d OFF\n", ledIndex + 1);  // Log por serial
    }
  }
}

// Función principal para manejar el estado de los LEDs
void manipulacionLed() {
  // Itera a través de todos los LEDs
  for (int i = 1; i <= numLEDs; i++) {
    // Prepara los strings para los comandos ON y OFF de cada LED
    char requestOn[10];
    char requestOff[10];
    sprintf(requestOn, "GET /on%d", i);  // Formato: "GET /on1", "GET /on2", etc.
    sprintf(requestOff, "GET /off%d", i); // Formato: "GET /off1", "GET /off2", etc.
    
    // Procesa posibles solicitudes para cada LED
    processLEDRequest(requestOn, i - 1);  // i-1 porque el array es base 0
    processLEDRequest(requestOff, i - 1);
  }
}

void loop() {
  // Verifica si hay un cliente conectado (navegador haciendo petición)
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";  // Almacena la línea HTTP actual
    boolean currentLineIsBlank = true;  // Flag para línea vacía

    // Mientras el cliente siga conectado
    while (client.connected()) {
      if (client.available()) {  // Hay datos disponibles para leer
        char c = client.read();  // Lee un caracter
        Serial.write(c);  // Opcional: muestra la comunicación en el monitor serial
        
        // Almacena el caracter en el buffer
        linebuf[charcount] = c;
        if (charcount < sizeof(linebuf) - 1) charcount++;

        // Detecta fin de línea (nueva solicitud HTTP)
        if (c == '\n') {
          currentLineIsBlank = true;
          if (currentLine.length() == 0) {
            // Línea vacía indica fin de headers HTTP, llama a la función web()
            web(client);  // Definida probablemente en data.h
            break;
          } else {
            currentLine = "";  // Reinicia para la próxima línea
          }
          
          // Procesa posibles comandos para los LEDs
          manipulacionLed();
          // Limpia el buffer para la próxima solicitud
          memset(linebuf, 0, sizeof(linebuf));
          charcount = 0;
        } 
        // Manejo de caracteres que no son retorno de carro
        else if (c != '\r') {
          currentLine += c;  // Construye la línea actual
          currentLineIsBlank = false;
        }
      }
    }
    // Pequeña pausa antes de cerrar la conexión
    delay(1);
    client.stop();  // Cierra la conexión con el cliente
    Serial.println("Cliente desconectado");
  }
}
```
