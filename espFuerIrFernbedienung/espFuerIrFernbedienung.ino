#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial SUART(D2, D1); //SRX = D2 = GPIO-4; STX = D1 = GPIO-5

const char* ssid = "Altherminator";          //wlan name
const char* password = "passwort"; //wlan passwort ist "passwort"

int zeitNachDemHochfahrenInSekunden = 15;

char websideChar[12000];           //wenn error dann erhoehen oder senken
String eingehenderString = "";
int laengeString = 0;

//Tetstintervall zum Senden zu Website
int interval = 999999999999;
unsigned long previousMillis = 0;


IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// StaticJsonDocument <200> docRx;
// StaticJsonDocument <200> docTx;


void webpage(){
  server.send(200,"text/html", websideChar);
  // server.send(200, "text/html", outputTest);
}


//Nimmt den Quellcode der Website vom Arduino entgegen und speichert sie in websiteChar[]
void lesenSerial(){                 //"void lesenSerial" ist ein versuch meinerseits
 Serial.println(" ");
 delay((zeitNachDemHochfahrenInSekunden * 1000));
  long zeit = millis();
  
  Serial.println(millis() - zeit);
  Serial.print("Erwarte eingehende Webside als String...");
  delay(200);
  SUART.print("A");
  delay(300);
  SUART.println("");
  bool vergleich = true;


  while(vergleich){

    eingehenderString = SUART.readStringUntil(10);
    if(eingehenderString != "^"){
      
      Serial.println(eingehenderString);
      for(int i = laengeString; i < (laengeString + eingehenderString.length()); i++){
        websideChar[i] = eingehenderString.charAt((i - laengeString));
      }
      //eingehenderString = "^";
      laengeString = eingehenderString.length() + laengeString;
      if((millis() - zeit) >= 25000){
        vergleich = false;
      }
    }
    
  }
  String test = websideChar;
  Serial.println(test);
}


//Diese Methode scheint nicht genutzt zu werden, sollte überprüft werden
void lesen(){
  if(SUART.available() > 0){
    String ergebnis = "";
    ergebnis = SUART.readString();
    if(ergebnis.charAt(0) > 30){
      Serial.println(ergebnis);
    }
  }
}

void setup() {


  Serial.begin(115200);
  SUART.begin(9600);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(4000);

  lesenSerial();   

  
  server.on("/", webpage);


  server.begin();

  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Bereit");
}


void loop() {
  server.handleClient();                                      //sorgt dafür, dass man auf die webside zugreifen kann

  webSocket.loop();


    //alles hier drunter ist ein realitätsnaher test für die Zuhunft


   unsigned long now = millis();                       // read out the current "time" ("millis()" gives the time in ms since the Arduino started)
  if ((unsigned long)(now - previousMillis) >= interval) { // check if "interval" ms has passed since last time the clients were updated
    

  String datenJsonString = "";
  StaticJsonDocument<800> doc;
  JsonObject object = doc.to<JsonObject>();
  
  doc["grund"] = "test";
  doc["datenArr"] [0] ["!"]="1"; 
  doc["datenArr"] [1] ["!"]="2";
  doc["datenArr"] [2] ["!"]="3";
  doc["datenArr"] [3] ["!"]="4";

  serializeJson(doc, datenJsonString);
  Serial.println(datenJsonString);
  webSocket.broadcastTXT(datenJsonString);

  Serial.println("daten gesendet");

    //             // send JSON string to clients
    // String jsonString = "";                           // create a JSON string for sending data to the client
    // StaticJsonDocument<200> doc;                      // create a JSON container
    // JsonObject object = doc.to<JsonObject>();         // create a JSON Object
    // object["rand1"] = random(100);                    // write data into the JSON object -> I used "rand1" and "rand2" here, but you can use anything else
    // object["rand2"] = random(100);
    // serializeJson(doc, jsonString);                   // convert JSON object to string
    // // Serial.println(jsonString);                       // print JSON string to console for debug purposes (you can comment this out)
    // webSocket.broadcastTXT(jsonString);   
  previousMillis = now;                             // reset previousMillis
  }
}


void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {      // the parameters of this callback function are always the same -> num: id of the client who send the event, type: type of message, payload: actual data sent and length: length of payload
  switch (type) {                                     // switch on the type of information sent
    case WStype_DISCONNECTED:                         // if a client is disconnected, then type == WStype_DISCONNECTED
      Serial.println("Client " + String(num) + " disconnected");
      break;
    case WStype_CONNECTED:                            // if a client is connected, then type == WStype_CONNECTED
      Serial.println("Client " + String(num) + " connected");
      // optionally you can add code here what to do when connected
      break;
    case WStype_TEXT:                                 // if a client has sent data, then type == WStype_TEXT
      // try to decipher the JSON string received
      StaticJsonDocument<800> doc;                    // create a JSON container
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      else {

        // JSON string Erfolgreich übermittelt, daten können verarbeitet werden

        const char* grundChar = doc["grund"];           //Grund für datenübertrgung wird Gespeicehrt
        //^^ müssen const sein
        String grund = String(grundChar);

        Serial.println(grund);

        if(grund == "knoepfeUmbenennen"){
          Serial.println(grund);
          String namen[16];

          for (int i=0; i< 16; i++){                              //namen der Knöpfe Werden in einem String Array gespeichert 
            const char* nameChar= doc["datenArr"][i]["innerText"];
            namen[i]= String(nameChar);
            Serial.println(namen[i]);                             //namen werden ausgegeben, müssen noch weterverarbeitet werden
          }
         }else if(grund=="ausfuehren"){     //Index des Gedrückten Kopfes im Array auf der Website wird übergeben                                       
          Serial.println(grund);
          const int nummer= doc["datenArr"][0]; 
          Serial.println(nummer);
          SUART.write(nummer);

         }else if (grund == "geraetUmbenennen"){      //Array mit den Namen der Geräten wird übergeben 
          String namen [4];
          Serial.println(grund);
          for (int i=0; i< 4; i++){                              
            const char* nameChar= doc["datenArr"][i]["innerText"];
            namen[i]= String(nameChar);
            Serial.println(namen[i]);                             
          }
         } //Weitere Gründe einfügen
       }
      Serial.println("");
      break;
  }
}