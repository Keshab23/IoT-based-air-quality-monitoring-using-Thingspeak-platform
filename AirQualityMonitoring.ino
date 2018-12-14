#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

String apiKey = "HMIZOTYY5X392L4M";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "AirQuality";     // replace with your wifi ssid and wpa2 key
const char *pass =  "arpanet123";
const char* server = "api.thingspeak.com";
WiFiClient client;
 const int smoke = A0;
 const int buzzer = 14;
 const int motorPin1 = 13;
LiquidCrystal_I2C lcd(0x27,16,2);
void setup() 
{{
       Serial.begin(115200);
       delay(10);
       Wire.begin(D2, D1);
       pinMode(smoke, OUTPUT);
       pinMode(buzzer, OUTPUT);
       pinMode(motorPin1, OUTPUT);
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, pass);
       lcd.begin();
       lcd.home();
       lcd.setCursor(0, 0);
       lcd.print(" MINOR PROJECT "); // Start Printing
       Serial.print(" MINOR PROJRCT ");
       delay(1000);
       lcd.setCursor(0, 1);
       lcd.print(" GROUP-11 ");
       Serial.print(" GROUP-11 ");
       delay(5000);
 }
 
while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
}
void loop() 
{

  int val = analogRead(smoke);
  int air_quality = val;
  lcd.setCursor (0, 0);
  lcd.print ("AQ is ");
  lcd.print (air_quality);
  lcd.print (" PPM          ");
  
if (air_quality<=100)
{

  lcd.setCursor (0,1);
  lcd.print("Fresh Air");
  digitalWrite(motorPin1, LOW );
  digitalWrite(buzzer,LOW  ); 
  
}
else if( air_quality>=100 && air_quality<=200 )
{
  lcd.setCursor (0,1);
  lcd.print("Poor Air           ");
  digitalWrite(motorPin1, HIGH ); 
  digitalWrite(buzzer,LOW  ); 
  LiquidCrystal_I2C lcd(0x27,16,2);
}
else if(air_quality>=200 )
{
  lcd.setCursor (0,1);
  lcd.print("Danger! Evacuate          ");
  digitalWrite(motorPin1, HIGH );
  digitalWrite(buzzer, HIGH); 
}


 if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(val);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Air Quality Value: ");
                             Serial.println(val);
                             
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
          Serial.println("Waiting...");
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(10000);
}
