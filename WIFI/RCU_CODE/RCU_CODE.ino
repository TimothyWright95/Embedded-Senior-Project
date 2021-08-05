#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here
//RCU
void setup()
{
  Serial.begin(9600);  
  Serial.setTimeout(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
 
    delay(1000);
 
  }
}
void loop() 
{
   String responce;
  String return_value;
  char receive_buffer[9] = {0};
  //read uart
  //if first char s
  //send
  //else
  //receive
  int numbytes = Serial.readBytes(receive_buffer, 9);
  if (numbytes > 0)
  {
    if (receive_buffer[0] == 's')
    {
      String to_send = receive_buffer;
      to_send.remove(0, 1);
      do
      {
        while(reset() != "SUCCESS");
        return_value = send(to_send);
      } while (return_value == "ERROR");
      Serial.write("s");
    }
    else if (receive_buffer[0] == 'r')
    {
      do
      {
        return_value = receive();
      } while (return_value == "ERROR");
      Serial.write(return_value.c_str());
    }
    else Serial.write("E");

  }
}


String send(String command)
{
  IPAddress host(192,168,1,1);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {
    return "ERROR";
  }
  String url = "/setrcu?i=" + command;
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host.toString() + "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) 
  {
    if (millis() - timeout > 5000)
    { 
      client.stop(); return "ERROR"; 
    } 
  } 
    
  while (client.available())
  { 
    String line = client.readStringUntil('\r'); 
  }
  client.stop();
  return "SUCCESS";
}
String reset()
{
  IPAddress host(192, 168, 1, 1);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    return "ERROR";
  }
  String url = "/setbs?i=0";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host.toString() + "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      client.stop(); return "ERROR";
    }
  }

  while (client.available())
  {
    String line = client.readStringUntil('\r');
  }
  client.stop();
  return "SUCCESS";
}
String receive()
{
  IPAddress host(192,168,1,1);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) 
  {
    return "ERROR";
  }
  String url = "/getbs";
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host.toString() + "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) 
  {
    if (millis() - timeout > 5000)
    { 
      client.stop(); return "ERROR"; 
    } 
  } 
  String line;  
  while (client.available())
  { 
     line = client.readStringUntil('\r'); 
  }
  client.stop();
  line.trim();
  return line;
}
  
