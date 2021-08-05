#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);
String rcu_to_bs = "nochange";
String bs_to_rcu = "nochange";

/* Variables */
bool Times[12][7];


/****************************************************/


void setup() {
// rcu_to_bs = '\n';
// bs_to_rcu = '\n';
  for( uint8_t i = 0; i < 12; i++)
  {
    for( uint8_t j = 0;j < 7;j++)
    {
      Times[i][j] = HIGH;
    }
  }
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  
  delay(100);
  
  server.on("/", handle_OnConnect);

 
  
  server.on("/INV", handle_invert);
  server.on("/setrcu",handle_setrcu);
  server.on("/setbs",handle_setbs);
  server.on("/getrcu",handle_getrcu);
  server.on("/getbs",handle_getbs);
  server.on("/getsched",handle_getsched);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.begin(9600);
  Serial.println("HTTP server started");

  
}
void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML()); 
}


void handle_getsched()///////////////////////////////////////////////////////////////////////////
{
  String tosend_string = "";
  for( uint8_t i = 0; i < 12; i++)
  {
    tosend_string += ':';
     for( uint8_t j = 0;j < 7;j++)
    {
      if(Times[i][j] == LOW)
      {
        String temp2 = String(j);
        tosend_string += temp2;
      }
    }
  }  
  Serial.write(tosend_string.c_str());
  server.send(200, "text/plain", tosend_string);
}




void handle_setrcu()
{
  rcu_to_bs = server.arg(0).c_str();
  Serial.print(rcu_to_bs + "\n\r");
  server.send(200, "text/plain", "");
}
void handle_setbs()
{
  bs_to_rcu = server.arg(0).c_str();
  Serial.print(bs_to_rcu + "\n\r");
  server.send(200, "text/plain", "");
}
void handle_getrcu()
{
  Serial.print("sending\n\r");
  server.send(200, "text/plain", rcu_to_bs);
}
void handle_getbs()
{
  Serial.print("sending\n\r");
  server.send(200, "text/plain", bs_to_rcu);
}


void handle_invert() {
  uint8_t i = atoi(server.arg(0).c_str());
  uint8_t j = atoi(server.arg(1).c_str());
  if(Times[i][j] == HIGH)
  {
    Times[i][j] = LOW;
  }
  else
  {
    Times[i][j] = HIGH;
  }

  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>scheduler</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 40px;background-color: #1abc9c;border: none;color: white;padding: 5px 5px;text-decoration: none;font-size: 15px;margin:auto;cursor: pointer;border-radius: 4px;text-align: center;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +=".tg  {border-collapse:collapse;border-spacing:0;border-color:#aabcfe;}\n";
  ptr +=".tg td{font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:#aabcfe;color:#669;background-color:#e8edff;}\n";
  ptr +=".tg th{font-family:Arial, sans-serif;font-size:14px;font-weight:normal;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:#aabcfe;color:#039;background-color:#b9c9fe;}\n";
  ptr +=".tg .tg-hmp3{background-color:#D2E4FC;text-align:left;vertical-align:top}\n";
  ptr +=".tg .tg-0lax{text-align:left;vertical-align:top;width:100px}\n";
  ptr +=".tg .tg-0pky{border-color:inherit;text-align:left;vertical-align:top}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>MSAC Web Scheduling Server</h1>\n";


  ptr +="<table class=\"tg\">\n";
  ptr +="<tr>\n";
  ptr +="<th class=\"tg-0lax\"></th>\n";
  ptr +="<th class=\"tg-0lax\">Sunday</th>\n";
  ptr +="<th class=\"tg-0lax\">Monday</th>\n";
  ptr +="<th class=\"tg-0lax\">Tuesday</th>\n";
  ptr +="<th class=\"tg-0lax\">Wednesday</th>\n";
  ptr +="<th class=\"tg-0lax\">Thursday</th>\n";
  ptr +="<th class=\"tg-0lax\">Friday</th>\n";
  ptr +="<th class=\"tg-0pky\">Saturday</th>\n";
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-hmp3\"><br>12AM</td>\n";
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[0][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=0&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=0&j=0\">ON</a>\n</td>\n";}
    
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[0][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=0&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=0&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[0][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=0&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=0&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[0][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=0&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=0&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[0][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=0&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=0&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[0][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=0&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=0&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[0][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=0&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=0&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";

  ptr +="<tr>\n";
    ptr +="<td class=\"tg-0lax\">2AM</td>\n";
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[1][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=1&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=1&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[1][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=1&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=1&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[1][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=1&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=1&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[1][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=1&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=1&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[1][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=1&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=1&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[1][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=1&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=1&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[1][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=1&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=1&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-hmp3\">4AM</td>\n";
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[2][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=2&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=2&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[2][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=2&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=2&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[2][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=2&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=2&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[2][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=2&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=2&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[2][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=2&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=2&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[2][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=2&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=2&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[2][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=2&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=2&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-0lax\">6AM</td>\n";
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[3][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=3&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=3&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[3][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=3&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=3&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[3][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=3&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=3&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[3][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=3&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=3&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[3][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=3&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=3&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[3][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=3&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=3&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[3][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=3&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=3&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-hmp3\">8AM</td>\n";
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[4][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=4&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=4&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[4][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=4&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=4&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[4][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=4&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=4&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[4][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=4&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=4&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[4][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=4&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=4&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[4][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=4&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=4&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[4][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=4&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=4&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-0lax\">10AM</td>\n";
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[5][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=5&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=5&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[5][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=5&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=5&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[5][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=5&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=5&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[5][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=5&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=5&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[5][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=5&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=5&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[5][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=5&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=5&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[5][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=5&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=5&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-hmp3\">12PM</td>\n";
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[6][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=6&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=6&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[6][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=6&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=6&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[6][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=6&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=6&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[6][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=6&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=6&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[6][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=6&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=6&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[6][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=6&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=6&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[6][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=6&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=6&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-0lax\">2PM</td>\n";
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[7][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=7&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=7&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[7][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=7&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=7&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[7][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=7&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=7&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[7][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=7&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=7&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[7][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=7&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=7&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[7][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=7&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=7&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[7][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=7&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=7&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-hmp3\">4PM</td>\n";
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[8][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=8&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=8&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[8][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=8&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=8&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[8][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=8&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=8&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[8][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=8&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=8&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[8][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=8&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=8&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[8][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=8&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=8&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[8][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=8&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=8&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-0lax\">6PM</td>\n";
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[9][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=9&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=9&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[9][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=9&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=9&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[9][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=9&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=9&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[9][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=9&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=9&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[9][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=9&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=9&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[9][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=9&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=9&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[9][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=9&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=9&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-hmp3\">8PM</td>\n";
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[10][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=10&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=10&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[10][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=10&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=10&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[10][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=10&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=10&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[10][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=10&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=10&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[10][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=10&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=10&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[10][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=10&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=10&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-hmp3\">\n";
    if(Times[10][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=10&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=10&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
  ptr +="<tr>\n";
    ptr +="<td class=\"tg-0lax\">10PM</td>\n";
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[11][0])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=11&j=0\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=11&j=0\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[11][1])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=11&j=1\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=11&j=1\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[11][2])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=11&j=2\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=11&j=2\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[11][3])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=11&j=3\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=11&j=3\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[11][4])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=11&j=4\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=11&j=4\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[11][5])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=11&j=5\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=11&j=5\">ON</a>\n</td>\n";}
    ptr +="<td class=\"tg-0lax\">\n";
    if(Times[11][6])
      {ptr +="<a class=\"button button-off\" href=\"/INV?i=11&j=6\">OFF</a>\n</td>\n";}
    else
      {ptr +="<a class=\"button button-on\" href=\"/INV?i=11&j=6\">ON</a>\n</td>\n";}
  ptr +="</tr>\n";
ptr +="</table>\n";

  

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
