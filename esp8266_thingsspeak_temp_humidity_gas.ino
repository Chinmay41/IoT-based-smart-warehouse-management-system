#include <SoftwareSerial.h>
#include <dht11.h>
#define RX 2
#define TX 3
#define dht_apin 4 // Analog Pin sensor is connected to
dht11 dhtObject;
String AP = "narzo 50A";       // AP NAME
String PASS = "12345678"; // AP PASSWORD
String API = "E24GK5Z00OXV3ON5";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;

//MQ2
int sensorValue = 0;
  
SoftwareSerial esp8266(RX,TX); 
  
void setup() {
  Serial.begin(115200);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}

void loop() {
  
 String getData = "GET /update?api_key="+ API +"&field1="+getTemperatureValue()+"&field2="+getHumidityValue()+"&field3="+getGasValue();
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");

//MQ 2
sensorValue = analogRead(0);
Serial.println("Gas concentration:");
Serial.println(sensorValue);
if (sensorValue > 400)
{
  Serial.println("Smoke detected");
}
else{
  Serial.println("Normal concentration");    
}

}


String getTemperatureValue(){

   dhtObject.read(dht_apin);
   Serial.print(" Temperature(C)= ");
   int temp = dhtObject.temperature;
   Serial.println(temp); 
   delay(50);
   return String(temp); 
  
}


String getHumidityValue(){

   dhtObject.read(dht_apin);
   Serial.print(" Humidity in %= ");
   int humidity = dhtObject.humidity;
   Serial.println(humidity);
   delay(50);
   return String(humidity); 
  
}

String getGasValue(){
  Serial.print("...................................");
  String myString = String(sensorValue);
  Serial.print(myString);
  return String(myString); 
  
}

void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
