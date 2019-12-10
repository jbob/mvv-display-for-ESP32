# mvv display for ESP32
Zeigt die nächsten Abfahrten der MVV auf einem kleinem OLED Display an. Es handelt sich hierbei um kein Projekt der MVV.

## Geops API Sbahn München:
Diese API kommt unter anderem in der München Navigator APP oder auf s-bahn-muenchen-live.de zum Einsatz und liefert sehr genaue Echtzeitinformationen, allerdings ausschließlich für die Sbahn. Alle nachfolgenden Erkenntnisse stammen aus öffentlichen Quellen oder aus Mitschnitten und Analysen des Netzwerktraffics.

### Funktionsweise
Jeder Zug sendet ca. alle 10 Sekunden ein GPS-Signal, das neben der Position auch Informationen zum aktuellen Zuglauf und zum Status des Zuges enthält. Diese Daten werden serverseitig ausgewertet und die daraus berechneten Abfahrtszeiten sowie die Zugpositionen auf der MVV-Netzkarte über die API verbereitet. 

### Server
URL                             | Port     | Protokoll
--------------------------------| -------- | --------
wss://tralis.sbahnm.geops.de/ws | 443      | Websocket

### Kommandos
Die Kommandos die an den Server geschickt werden sind in der redis-websocket-api definiert. Die wichtigesten Kommandos für dieses Projekt mit ihren Keys sind hier aufgelistet: 

Kommando              | Funktion 
----------------------|-----------------------------------------------------
PING                  | Überprüfen der Verbindung
GET healthcheck       | Auskunft über den Serverstatus
GET timetable_8000261 | Liefert die aktuellen Abfahrtzeiten am Hauptbahnhof
SUB timetable_8000261 | Abboniert die Abfahrtzeiten am Hauptbahnhof
DEL timetable_8000261 | Deabboniert die Abfahrtzeiten am Hauptbahnhof

Während GET einmalig die nächsten Abfahrten liefert, bekommt der Client mit SUB immer automatisch eine Aktualisierung. Die Bahnhofsnummer ist nicht identisch mit der ID der MVG API und kann aktuell schneinbar nur durch Mitschnitt herausgefunden werden. Ideen? Der Server antwortet immer im JSON Format. Ein Beispielpaket eines Timetabeles einer Sbahn sieht folgendermaßen aus:


```json
{
   "timestamp":1576000939716.51,
   "content":{
      "ris_estimated_time":1576002060000.0,
      "created_at":1575990536037.232,
      "min_arrival_time":1576001823000.0,
      "train_type":2,
      "fzo_estimated_time":1576002222000.0,
      "next_stoppoints":[
         "MDA",
         "MKFS",
         "MMAL",
         "MAUG",
         "MOZ",
         "ML",
         "MMHG",
         "MMDN",
         "MHAB",
         "MHT",
         "MKA",
         "MMP",
         "MIT",
         "MRP",
         "MOP"
      ],
      "train_id":139923201210800,
      "no_stop_between":null,
      "line":{
         "text_color":"#ffffff",
         "color":"#8bbd4d",
         "id":2,
         "name":"S2"
      },
      "state":null,
      "new_to":null,
      "updated_at":1575998600946.968,
      "no_stop_till":null,
      "platform":"1",
      "formation":null,
      "at_station_ds100":"MDA",
      "ris_aimed_time":1576002060000.0,
      "to":[
         "M\u00fcnchen Ost"
      ],
      "has_fzo":true,
      "train_number":6015,
      "time":1576002060000.0,
      "call_id":37049115
   },
   "source":"timetable_8000261",
   "client_reference":null
}
```
Die ris_aimed_time stellet scheinbar die geplante Abfahrtszeit nach Fahrplan dar. Interessant wären die ris_estimated_time, min_arrival_time und fzo_estimated_time zu der aktuell weitere Erkenntnisse fehlen... Ideen?

Weitere sehr Interessante Infos sind train_type mit der Wagonanzahl  sowie at_station_ds100 mit der aktuellen Stationsposition.

## Testprogramm:
Das Testprogramm abonniert die Abfahrten des Hauptbahnhof und gibt diese auf der seriellen Schnittstelle aus. Hierfür wird die ArduinoWebockets Library benötigt: [https://github.com/gilmaimon/ArduinoWebsockets](https://github.com/gilmaimon/ArduinoWebsockets)

## Quellen:
[https://geops.ch/sbahnm-live](https://geops.ch/sbahnm-live)  
[https://geops.ch/blog/zuege-echtzeit](https://geops.ch/blog/zuege-echtzeit)  
[https://github.com/geops/redis-websocket-api](https://github.com/geops/redis-websocket-api)
