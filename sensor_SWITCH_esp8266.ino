#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "MOVISTAR_0582";
const char* password = "2MmJL963DHYH9qTCG9D6";

const int PIN_RELE = 2;   //Es el puerto GPIO2
String ESTADO_RELE = "OFF";

ESP8266WebServer server(80);


void info() {
  String info_mensaje = "<b>ESP8266</b><br>Comando <b>on</b>: encender<br>Comando <b>off</b>: apagar<br><br>";
  String str_estado = ESTADO_RELE;
  info_mensaje = info_mensaje + "Estado:" + str_estado + "<br>";
  server.send(200, "text/html", info_mensaje);
}

void estado_rele() {
  server.send(200, "text/plain", ESTADO_RELE);
}

void no_encontrado() {
  server.send(404, "text/plain", "ERROR EN LA PETICION :( ");
}

void on_rele() {
  server.send(200, "text/plain", "OK ON");
  ESTADO_RELE = "ON";
  digitalWrite(PIN_RELE, LOW);
}

void off_rele() {
  server.send(200, "text/plain", "OK OFF");
  ESTADO_RELE = "OFF";
  digitalWrite(PIN_RELE, HIGH);
}

void setup(void) {
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, HIGH);

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Esperamos a que se conecte
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectedo a ");
  Serial.println(ssid);
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  IPAddress sensor_ip = WiFi.localIP();

  // Declaramos un par de paths por si queremos consultar directamente el estado
  server.on("/", info);
  server.on("/estado", estado_rele);

  // Declaramos los paths para el control del rele
  server.on("/on", on_rele);
  server.on("/off", off_rele);

  server.onNotFound(no_encontrado);

  server.begin();
  Serial.println("Servidor HTTP activo");
}

void loop(void) {
  server.handleClient();
}
