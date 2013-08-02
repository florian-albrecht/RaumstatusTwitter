//Einbinden der benötigten librarys
#include <SPI.h>
#include <Ethernet.h>
#include <Twitter.h>
//Zuweisung der MAC-Adresse für die Initialisierung
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
//Zuweisung der IP-Adresse für die Initialisierung
byte ip[] = {192, 168, 178, 250 };
//Twittertoken von http://arduino-tweet.appspot.com/
Twitter twitter("TOKENTOKEN");
//Die möglichen Tweets für auf und zu
char* auf[] = {
  "Spruch1 #offen",
  "Spruch2 #offen",
  "... #offen",
  NULL};//am Ende Null, um Ende zu erkennen
char* zu[] = {
  "Spruch1 #geschlossen",
  "Spruch2 #geschlossen",
  "... #geschlossen",
  NULL};//am Ende Null, um Ende zu erkennen
//tweet, der beim Setup getweetet wird
char start[] = "Guten morgen, wurde gerade neu gestartet. #reboot";
//Variablen
int lux = 0; 
int a = 0; 
int stat = 2; 
int laengezu = 0; 
int laengeauf = 0; 
int u = 0;

void setup()
{
  /*Funktion zur Bestimmung der Länge der Spruchliste wird aufgerufen und
  in  'laengezu' und 'laengeauf' gespeichert*/
  laengezu = laenge(zu);
  laengeauf = laenge(auf);
  //warten, damit der Arduino für die Initialisierung komplett hochgefahren ist
  delay(3000);
  //Initialisierung des Ethernetshieldes
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  //tweeten, dass der Arduino gerade gestarte wurde
  tweet(start);
}

void loop(){
  while(a < 5){
    lux += analogRead(A0);
    a++;
    delay(2000);
  }
  lux = lux/5;
  //Wenn Licht von hell nach dunkel oder andersherum, dann twittern und Status umsetzen
  if(lux < 150 && stat != 0){
    u = tweet(zu[random(0,laengezu)]);
    if(u == 200){
      stat = 0;
    }
  }
  if(lux > 150 && stat != 1){
    u = tweet(auf[random(0, laengeauf)]);
    if(u == 200){
      stat = 1;
    }
  }
  delay(1000);
  a=0;
  lux = 0;
}

//Tweet abschicken
int tweet(char text[]){
  //Serial.println("connecting ...");
  int status = 0;
  //tweet abschicken
  if(twitter.post(text)){
    //Status von Twitter abfragen
    status = twitter.wait(&Serial);
    //bei der Rückgabe 200 wurde der Tweet erfolgreich abgeschickt
    if(status == 200){
      Serial.println("Tweet erfolgreich abgeschickt.");
    } 
    else{
      //Fehlercode mitteilen
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } 
  else {
    Serial.println("Verbindung fehlgeschlagen!");
  }
  
  return status;
}

//bestimmen der Länge der Liste
int laenge(char * arr[]){
  int i = 0;
  while(arr[i] != NULL){
    i++;
  }
  return i;
}
