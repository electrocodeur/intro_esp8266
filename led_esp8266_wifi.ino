#include <ESP8266WiFi.h>

const char* ssid = "SSID de votre box (souvent ecrite derriere)";
const char* password = "Mot de passe de votre WIFI";

int ledPin = 4;
WiFiServer server(80);

void setup()
{
  // initialisation de la communication série
  Serial.begin(9600);

  delay(100);

  // initialisation de la sortie pour la led
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connexion wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // connection  en cours ...
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Wifi connecter
  Serial.println("WiFi connecter");

  // Démmarrage du serveur.
  server.begin();
  Serial.println("Serveur demarrer !");

  // Affichage de l'adresse IP
  Serial.print("Utiliser cette adresse URL pour la connexion :");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop()
{
  WiFiClient client;


  // Vérification si le client est connecter.
  client = server.available();
  if (!client)
  {
    return;
  }

  // Attendre si le client envoie des données ...
  Serial.println("nouveau client");
  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH); // allumer la led
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW); // éteindre la led
    value = LOW;
  }

  // Réponse
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Etat de la led : ");

  if (value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Allumer </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Eteindre </button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client deconnecter");
  Serial.println("");

}
